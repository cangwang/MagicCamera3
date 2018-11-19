#version 300 es

out highp vec2 textureCoordinate;
uniform sampler2D inputImageTexture;
out vec4 fragColor;

void main() {
    fragColor = texture(inputImageTexture,textureCoordinate);
}