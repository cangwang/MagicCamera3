#version 300 es
#extension GL_OES_EGL_image_external_essl3 : require

precision highp float;
//从vertex中输入
in highp vec2 textureCoordinate;
//取出OES相机中的纹理
uniform samplerExternalOES inputImageTexture;
//输出色值
 out vec4 fragColor;

void main() {
    fragColor = texture(inputImageTexture,textureCoordinate);
}