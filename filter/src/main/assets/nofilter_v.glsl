#version 300 es

in vec4 position;
in vec4 inputTextureCoordinate;
uniform mat4 mvpMatrix;

out vec2 textureCoordinate;

void main() {
    gl_Position = mvpMatrix * position;
    textureCoordinate = inputTextureCoordinate.xy;
}