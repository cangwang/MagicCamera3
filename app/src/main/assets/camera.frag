#version 300 es
//加入opengles扩展库
#extension GL_OES_EGL_image_external_essl3 : require

precision highp float;
//来自摄像头预览的外部纹理
uniform samplerExternalOES sTexture;

in vec2 vTexCoord;

out vec4 fragColor;

void main() {
    fragColor = texture(sTexture, vTexCoord);
}