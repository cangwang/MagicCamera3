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

#ifndef ScratchCardFilter_hpp
#define ScratchCardFilter_hpp

#include <src/main/cpp/glm/detail/type_mat.hpp>
#include <src/main/cpp/glm/vec2.hpp>
#include <src/main/cpp/glm/vec3.hpp>
#include <src/main/cpp/glm/detail/type_mat4x4.hpp>
#include "../source/macros.h"
#include "Filter.hpp"
#include <src/main/cpp/utils/ByteFlowLock.h>

#define TRIANGLE_NUM  43
#define EFFECT_RADIUS 0.03

NS_GI_BEGIN
using namespace glm;

class ScratchCardFilter : public Filter {
public:
    static ScratchCardFilter* create();
    bool init();
    virtual bool proceed(bool bUpdateTargets = true) override;

    void setTemperature(float temperature);
    void setTint(float tint);
    void setTouchLocation(float x, float y) override ;

protected:
    ScratchCardFilter() {};
    ~ScratchCardFilter();
    void calculateMesh(vec2 pre, vec2 cur);
    void updateMVPMatrix(mat4 &mvpMatrix,
            int angleX, int angleY, float ratio);
    void setSaturation(float saturation);
    void getWidthAndHeight();

//    bool preProceed(bool bUpdateTargets = true) override ;
//    void proceedBefore() override ;
//    void proceedAfter() override ;

    float _temperature;
    float _tint;

    GLuint m_TextureId;
    GLint m_SamplerLoc;
    GLint m_MVPMatLoc;
    GLuint m_VaoId;
    GLuint m_VboIds[2];
    mat4 m_MVPMatrix;

    int m_AngleX;
    int m_AngleY;
    float m_ScaleX;
    float m_ScaleY;
    vec3 m_pVtxCoords[TRIANGLE_NUM * 3];
    vec2 m_pTexCoords[TRIANGLE_NUM * 3];

    std::vector<vec4> m_PointVector;

    vec2 m_CurTouchPoint;
    vec2 m_PreTouchPoint;
    bool m_bReset;
    MySyncLock m_Lock;

    float _saturation;

    float filterWidth;
    float filterHeight;
};

NS_GI_END

#endif /* WhiteBalanceFilter_hpp */
