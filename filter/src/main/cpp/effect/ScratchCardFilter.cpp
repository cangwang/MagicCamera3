/*
 * GPUImage-x
 *
 * Copyright (C) 2017 Yijin Wang, Yiqian Wang
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//#include <src/main/cpp/Common/Platform.h>
#include <src/main/cpp/source/Context.hpp>
#include "ScratchCardFilter.hpp"

#include "glm/gtc/matrix_transform.hpp"

USING_NS_GI

/**
 * 求二元一次方程的系数
 * y1 = k * x1 + b => k = (y1 - b) / x1
 * y2 = k * x2 + b => y2 = ((y1 - b) / x1) * x2 + b
 */
vec2 binaryEquationGetKB(float x1, float y1, float x2, float y2) {
    float k = (y1 - y2) / (x1 - x2);
    float b = (x1 * y2 - x2 * y1) / (x1 - x2);
    return vec2(k, b);
}

/**
 * 一元二次方程求根
 * ax² + bx + c = 0
 */
vec2 quadEquationGetX(float a, float b, float c) {
    vec2 xArr;
    float result = pow(b, 2.0f) - 4 * a * c;
    if (result > 0) {
        xArr.x = (-b + sqrt(result)) / (2 * a);
        xArr.y = (-b - sqrt(result)) / (2 * a);
    } else if (result == 0) {
        xArr.y = xArr.x = (-b / (2 * a));
    }
    return xArr;
}


/**
 * 求圆和直线之间的交点
 * 直线方程：y = kx + b
 * 圆的方程：(x - m)² + (x - n)² = r²
 * x1, y1 = 线坐标1, x2, y2 = 线坐标2, m, n = 圆坐标, r = 半径
 */
vec4 getInsertPointBetweenCircleAndLine(float x1, float y1, float x2, float y2, float m, float n,
                                        float r) {
    vec2 kbArr = binaryEquationGetKB(x1, y1, x2, y2);
    float k = kbArr.x;
    float b = kbArr.y;

    float aX = 1 + k * k;
    float bX = 2 * k * (b - n) - 2 * m;
    float cX = m * m + (b - n) * (b - n) - r * r;

    vec4 insertPoints;
    vec2 xArr = quadEquationGetX(aX, bX, cX);
    insertPoints.x = xArr.x;
    insertPoints.y = k * xArr.x + b;
    insertPoints.z = xArr.y;
    insertPoints.w = k * xArr.y + b;

    return insertPoints;
}


glm::vec3 texCoordToVertexCoord(glm::vec2 texCoord) {
    return glm::vec3(2 * texCoord.x - 1, 1 - 2 * texCoord.y, 0);
}

REGISTER_FILTER_CLASS(ScratchCardFilter)

const std::string kScratchCardFragmentShaderString = SHADER_STRING
(
        precision highp float;
        in vec2 vTexCoord;

        uniform sampler2D colorMap;
        layout(location = 0) out vec4 gl_FragColor;

        void main()
        {
            gl_FragColor = texture(colorMap, vTexCoord);
        }
);


ScratchCardFilter* ScratchCardFilter::create() {
    ScratchCardFilter* ret = new (std::nothrow) ScratchCardFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

ScratchCardFilter::~ScratchCardFilter() {
    if (_filterProgram != NULL) {
        glDeleteBuffers(2, m_VboIds);
        glDeleteVertexArrays(1, &m_VaoId);
    }
    m_SamplerLoc = GL_NONE;
    m_MVPMatLoc = GL_NONE;

    m_TextureId = GL_NONE;
    m_VaoId = GL_NONE;

    m_AngleX = 0;
    m_AngleY = 0;

    m_ScaleX = 1.0f;
    m_ScaleY = 1.0f;

    m_bReset = true;
}

bool ScratchCardFilter::init() {
    if (!initWithFragmentShaderString(kScratchCardFragmentShaderString)) return false;
    setTemperature(5000.0);
    registerProperty("temperature", 5000.0, "Adjustment of color temperature (in degrees Kelvin) in terms of what an image was effectively shot in. This means higher Kelvin values will warm the image, while lower values will cool it.", [this](float& temperature){
        setTemperature(temperature);
    });

    setTint(0.0);
    registerProperty("tint", _tint, "adjust tint to compensate", [this](float& tint){
        setTint(tint);
    });
    return true;
}

void ScratchCardFilter::setTemperature(float temperature) {
    _temperature = temperature < 5000 ? 0.0004 * (temperature - 5000.0) : 0.00006 * (temperature - 5000.0);
}

void ScratchCardFilter::setTint(float tint) {
    _tint = tint / 100.0;
}

static const GLfloat imageVertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f,  1.0f,
        1.0f,  1.0f,
};

bool ScratchCardFilter::proceed(bool bUpdateTargets/* = true*/) {
    ScopedSyncLock lock(&m_Lock);

    Context::getInstance()->setActiveShaderProgram(_filterProgram);
    _framebuffer->active();
    filterWidth = _framebuffer->getWidth();
    filterHeight = _framebuffer->getHeight();
    CHECK_GL(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
    CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));

    if(m_VboIds[0] == GL_NONE)
    {
        // Generate VBO Ids and load the VBOs with data
        glGenBuffers(2, m_VboIds);
        glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_pVtxCoords), m_pVtxCoords, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_pTexCoords), m_pTexCoords, GL_DYNAMIC_DRAW);
    }

    if(m_VaoId == GL_NONE)
    {
        // Generate VAO Id
        glGenVertexArrays(1, &m_VaoId);
        glBindVertexArray(m_VaoId);

        glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

        glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

        glBindVertexArray(GL_NONE);
    }

    // Use the program object
    CHECK_GL(glEnable(GL_STENCIL_TEST));
    CHECK_GL(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));//当片段的模板值不为 1 时，片段通过测试进行渲染
    CHECK_GL(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));//若模板测试和深度测试都通过了，将片段对应的模板值替换为1
    CHECK_GL(glStencilMask(0xFF));

    //渲染已经叠加的滤镜
    for (std::map<int, InputFrameBufferInfo>::const_iterator it = _inputFramebuffers.begin(); it != _inputFramebuffers.end(); ++it) {
        int texIdx = it->first;
        Framebuffer* fb = it->second.frameBuffer;
        CHECK_GL(glActiveTexture(GL_TEXTURE0 + texIdx));
        CHECK_GL(glBindTexture(GL_TEXTURE_2D, fb->getTexture()));
        _filterProgram->setUniformValue(
                texIdx == 0 ? "colorMap" : str_format("colorMap%d", texIdx),
                texIdx);
        // texcoord attribute
        GLuint filterTexCoordAttribute = _filterProgram->getAttribLocation(texIdx == 0 ? "texCoord" : str_format("texCoord%d", texIdx));
        CHECK_GL(glEnableVertexAttribArray(filterTexCoordAttribute));
        CHECK_GL(glVertexAttribPointer(filterTexCoordAttribute, 2, GL_FLOAT, 0, 0, _getTexureCoordinate(it->second.rotationMode)));
    }
    CHECK_GL(glVertexAttribPointer(_filterPositionAttribute, 2, GL_FLOAT, 0, 0, imageVertices));
    CHECK_GL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

    for (std::map<int, InputFrameBufferInfo>::const_iterator it = _inputFramebuffers.begin(); it != _inputFramebuffers.end(); ++it) {
        int texIdx = it->first;
        Framebuffer* fb = it->second.frameBuffer;
        CHECK_GL(glActiveTexture(GL_TEXTURE0 + texIdx));
        CHECK_GL(glBindTexture(GL_TEXTURE_2D, fb->getTexture()));
        CHECK_GL(glActiveTexture(GL_TEXTURE0));
    }

//    if (m_PointVector.size() > 1) {
//        CHECK_GL(glBindVertexArray(m_VaoId));
//        for (int i = 0; i < m_PointVector.size(); ++i) {
//            vec4 pre_cur_point = m_PointVector[i];
//            calculateMesh(vec2(pre_cur_point.x, pre_cur_point.y),
//                          vec2(pre_cur_point.z, pre_cur_point.w));
//            CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]));
//            CHECK_GL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_pVtxCoords), m_pVtxCoords));
//            CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]));
//            CHECK_GL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_pTexCoords), m_pTexCoords));
//            CHECK_GL(glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_NUM * 3));
//        }
//        CHECK_GL(glBindVertexArray(GL_NONE));
//    }

    CHECK_GL(glDisable(GL_STENCIL_TEST));

    CHECK_GL(glActiveTexture(GL_TEXTURE0));

    _framebuffer->inactive();

    return Source::proceed(bUpdateTargets);
}

void ScratchCardFilter::calculateMesh(vec2 pre, vec2 cur) {
    vec2 imgSize(filterWidth,filterHeight);
    vec2 p0 = pre * imgSize, p1 = cur * imgSize;
    vec2 v0, v1, v2, v3;
    float r = static_cast<float>(EFFECT_RADIUS * imgSize.x);
    float x0 = p0.x, y0 = p0.y;
    float x1 = p1.x, y1 = p1.y;
    if (p0.y == p1.y) //1. 平行于 x 轴的
    {
        v0 = vec2(p0.x, p0.y - r) / imgSize;
        v1 = vec2(p0.x, p0.y + r) / imgSize;
        v2 = vec2(p1.x, p1.y - r) / imgSize;
        v3 = vec2(p1.x, p1.y + r) / imgSize;

    } else if (p0.x == p1.x) { //2. 平行于 y 轴的
        v0 = vec2(p0.x - r, p0.y) / imgSize;
        v1 = vec2(p0.x + r, p0.y) / imgSize;
        v2 = vec2(p1.x - r, p1.y) / imgSize;
        v3 = vec2(p1.x + r, p1.y) / imgSize;

    } else { //3. 其他 case
        float A0 = (y1 - y0) * y0 + (x1 - x0) * x0;
        float A1 = (y0 - y1) * y1 + (x0 - x1) * x1;
        // y = a0 * x + c0,  y = a1 * x + c1
        float a0 = -(x1 - x0) / (y1 - y0);
        float c0 = A0 / (y1 - y0);

        float a1 = -(x0 - x1) / (y0 - y1);
        float c1 = A1 / (y0 - y1);

        float x0_i = 0;
        float y0_i = a0 * x0_i + c0;

        float x1_i = 0;
        float y1_i = a1 * x1_i + c1;

        vec4 v0_v1 = getInsertPointBetweenCircleAndLine(x0, y0, x0_i, y0_i, x0, y0, r);

        v0 = vec2(v0_v1.x, v0_v1.y) / imgSize;
        v1 = vec2(v0_v1.z, v0_v1.w) / imgSize;

        vec4 v2_v3 = getInsertPointBetweenCircleAndLine(x1, y1, x1_i, y1_i, x1, y1, r);

        v2 = vec2(v2_v3.x, v2_v3.y) / imgSize;
        v3 = vec2(v2_v3.z, v2_v3.w) / imgSize;

    }

    // 矩形 3 个三角形
    m_pTexCoords[0] = v0;
    m_pTexCoords[1] = v1;
    m_pTexCoords[2] = v2;
    m_pTexCoords[3] = v0;
    m_pTexCoords[4] = v2;
    m_pTexCoords[5] = v3;
    m_pTexCoords[6] = v1;
    m_pTexCoords[7] = v2;
    m_pTexCoords[8] = v3;

    int index = 9;
    float step = static_cast<float>(PI / 10);
    // 2 个圆，一共 40 个三角形
    for (int i = 0; i < 20; ++i) {
        float x = r * cos(i * step);
        float y = r * sin(i * step);

        float x_ = r * cos((i + 1) * step);
        float y_ = r * sin((i + 1) * step);

        x += x0;
        y += y0;
        x_ += x0;
        y_ += y0;

        m_pTexCoords[index + 6 * i + 0] = vec2(x, y) / imgSize;
        m_pTexCoords[index + 6 * i + 1] = vec2(x_, y_) / imgSize;
        m_pTexCoords[index + 6 * i + 2] = vec2(x0, y0) / imgSize;

        x = r * cos(i * step);
        y = r * sin(i * step);

        x_ = r * cos((i + 1) * step);
        y_ = r * sin((i + 1) * step);

        x += x1;
        y += y1;
        x_ += x1;
        y_ += y1;

        m_pTexCoords[index + 6 * i + 3] = vec2(x, y) / imgSize;
        m_pTexCoords[index + 6 * i + 4] = vec2(x_, y_) / imgSize;
        m_pTexCoords[index + 6 * i + 5] = vec2(x1, y1) / imgSize;
    }

    for (int i = 0; i < TRIANGLE_NUM * 3; ++i) {
        m_pVtxCoords[i] = texCoordToVertexCoord(m_pTexCoords[i]);
    }

}

void ScratchCardFilter::setTouchLocation(float x, float y) {
    if(filterWidth * filterHeight != 0)
    {
        ScopedSyncLock lock(&m_Lock);
        if(x == -1) m_bReset = true;

        if(m_bReset)
        {
            if(x != -1)
            {
                m_CurTouchPoint = vec2(x / filterWidth, y / filterHeight);
                m_bReset = false;
            }
        } else {
            m_PreTouchPoint = m_CurTouchPoint;
            m_CurTouchPoint = vec2(x / filterWidth, y / filterHeight);
            if(m_CurTouchPoint == m_PreTouchPoint)
                return;
            m_PointVector.emplace_back(m_PreTouchPoint.x, m_PreTouchPoint.y, m_CurTouchPoint.x, m_CurTouchPoint.y);
        }
    }
}
