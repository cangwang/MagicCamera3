#version 300 es

layout(location=0) in vec4 position;
layout(location=1) in vec4 inputTextureCoordinate;

uniform mat4 textureTransform;
out vec2 textureCoordinate;

void main() {
    gl_Position = position;
    textureCoordinate = (textureTransform *inputTextureCoordinate).xy;
}