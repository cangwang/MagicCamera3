#version 300 es
#extension GL_OES_EGL_image_external_essl3 : require

precision highp float;

uniform samplerExternalOES sTexture;

in vec2 vTexCoord;

layout(location=0) out vec4 fragColor;

void main() {
    fragColor = texture(sTexture, vTexCoord);
}