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

#include "SphereRefractionFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(SphereRefractionFilter)

const std::string kSphereRefractionShaderString = SHADER_STRING
(
 
 uniform sampler2D colorMap;
 uniform highp vec2 center;
 uniform highp float radius;
 uniform highp float aspectRatio;
 uniform highp float refractiveIndex;
 
 varying highp vec2 vTexCoord;
 
 void main()
 {
     highp vec2 textureCoordinateToUse = vec2(vTexCoord.x, (vTexCoord.y * aspectRatio + 0.5 - 0.5 * aspectRatio));
     highp float distanceFromCenter = distance(center, textureCoordinateToUse);
     lowp float checkForPresenceWithinSphere = step(distanceFromCenter, radius);
     
     distanceFromCenter = distanceFromCenter / radius;
     
     highp float normalizedDepth = radius * sqrt(1.0 - distanceFromCenter * distanceFromCenter);
     highp vec3 sphereNormal = normalize(vec3(textureCoordinateToUse - center, normalizedDepth));
     
     highp vec3 refractedVector = refract(vec3(0.0, 0.0, -1.0), sphereNormal, refractiveIndex);
     
     gl_FragColor = texture2D(colorMap, (refractedVector.xy + 1.0) * 0.5) * checkForPresenceWithinSphere;
 }

);


SphereRefractionFilter* SphereRefractionFilter::create() {
    SphereRefractionFilter* ret = new (std::nothrow) SphereRefractionFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool SphereRefractionFilter::init() {
    if (!initWithFragmentShaderString(kSphereRefractionShaderString)) return false;

    
    setPositionX(0.5);
    registerProperty("positionX", _position.x, "The position of x about which to apply the distortion, with a default of 0.5", [this](float& positionX){
        setPositionX(positionX);
    });
    
    setPositionY(0.5);
    registerProperty("positionY", _position.y, "The position of y about which to apply the distortion, with a default of 0.5", [this](float& positionY){
        setPositionY(positionY);
    });

    setRadius(0.25);
    registerProperty("radius", _radius, "The radius of the distortion, ranging from 0.0 to 1.0, with a default of 0.25", [this](float& radius){
        setRadius(radius);
    });
    
    setRefractiveIndex(0.71);
    registerProperty("refractiveIndex", _refractiveIndex, "The index of refraction for the sphere, with a default of 0.71", [this](float& refractiveIndex){
        setRefractiveIndex(refractiveIndex);
    });
    
    return true;
}

bool SphereRefractionFilter::proceed(bool bUpdateTargets/* = true*/) {
    _filterProgram->setUniformValue("center", _position);
    _filterProgram->setUniformValue("radius", _radius);
    _filterProgram->setUniformValue("refractiveIndex", _refractiveIndex);
    
    float aspectRatio = 1.0;
    Framebuffer* firstInputFramebuffer = _inputFramebuffers.begin()->second.frameBuffer;
    aspectRatio = firstInputFramebuffer->getHeight() / (float)(firstInputFramebuffer->getWidth());
    _filterProgram->setUniformValue("aspectRatio", aspectRatio);
    
    return Filter::proceed(bUpdateTargets);
}

void SphereRefractionFilter::setPositionX(float x) {
    _position.x = x;
}

void SphereRefractionFilter::setPositionY(float y) {
    _position.y = y;
}

void SphereRefractionFilter::setRadius(float radius) {
    _radius = radius;
}

void SphereRefractionFilter::setRefractiveIndex(float refractiveIndex) {
    _refractiveIndex = refractiveIndex;
}

