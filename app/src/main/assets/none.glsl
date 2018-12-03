#version 300 es

precision highp float;
//从vertex中输入
in highp vec2 textureCoordinate;

uniform sampler2D inputImageTexture;
//输出色值
out vec4 gl_FragColor;

void main() {
    gl_FragColor = texture(inputImageTexture,textureCoordinate);
}