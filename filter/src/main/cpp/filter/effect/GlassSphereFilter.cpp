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

#include "GlassSphereFilter.hpp"

USING_NS_GI

REGISTER_FILTER_CLASS(GlassSphereFilter)

const std::string kGlassSphereFragmentShaderString = SHADER_STRING
(
 
 uniform sampler2D colorMap;
 varying highp vec2 vTexCoord;
 
 uniform highp vec2 center;
 uniform highp float radius;
 uniform highp float aspectRatio;
 uniform highp float refractiveIndex;

 const highp vec3 lightPosition = vec3(-0.5, 0.5, 1.0);
 const highp vec3 ambientLightPosition = vec3(0.0, 0.0, 1.0);

 
 void main()
 {
     highp vec2 textureCoordinateToUse = vec2(vTexCoord.x, (vTexCoord.y * aspectRatio + 0.5 - 0.5 * aspectRatio));
     
     highp float distanceFromCenter = distance(center, textureCoordinateToUse);
     lowp float checkForPresenceWithinSphere = step(distanceFromCenter, radius);
     
     distanceFromCenter = distanceFromCenter / radius;
     
     highp float normalizedDepth = radius * sqrt(1.0 - distanceFromCenter * distanceFromCenter);
     highp vec3 sphereNormal = normalize(vec3(textureCoordinateToUse - center, normalizedDepth));
     
     highp vec3 refractedVector = 2.0 * refract(vec3(0.0, 0.0, -1.0), sphereNormal, refractiveIndex);
     refractedVector.xy = -refractedVector.xy;
     
     highp vec3 finalSphereColor = texture2D(colorMap, (refractedVector.xy + 1.0) * 0.5 + center - vec2(0.5)).rgb;
     
     // Grazing angle lighting
     highp float lightingIntensity = 2.5 * (1.0 - pow(clamp(dot(ambientLightPosition, sphereNormal), 0.0, 1.0), 0.25));
     finalSphereColor += lightingIntensity;
     
     // Specular lighting
     //lightingIntensity  = clamp(dot(normalize(lightPosition), sphereNormal), 0.0, 1.0);
     //lightingIntensity  = pow(lightingIntensity, 15.0);
     //finalSphereColor += vec3(0.8, 0.8, 0.8) * lightingIntensity;
     
     gl_FragColor = vec4(finalSphereColor, 1.0) * checkForPresenceWithinSphere;
 }
 );



GlassSphereFilter* GlassSphereFilter::create() {
    GlassSphereFilter* ret = new (std::nothrow) GlassSphereFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool GlassSphereFilter::init() {
    if (!initWithFragmentShaderString(kGlassSphereFragmentShaderString)) return false;
    
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
