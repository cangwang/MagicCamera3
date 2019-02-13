#version 300 es
precision mediump float;
 
 in vec2 textureCoordinate;
 uniform sampler2D inputImageTexture;
 out vec4 glFragColor;

 void main()
 {
    glFragColor = texture(inputImageTexture,textureCoordinate);
 }