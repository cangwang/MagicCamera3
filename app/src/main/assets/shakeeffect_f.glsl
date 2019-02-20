#version 300 es
precision mediump float;
 in vec2 textureCoordinate;
 uniform sampler2D inputImageTexture;
 uniform float uTextureCoordOffset;
 out vec4 glFragColor;

 void main()
 {
    vec4 blue = texture(inputImageTexture,textureCoordinate);
    vec4 green = texture(inputImageTexture, vec2(textureCoordinate.x + uTextureCoordOffset, textureCoordinate.y+ uTextureCoordOffset));
    vec4 red = texture(inputImageTexture,vec2(textureCoordinate.x-uTextureCoordOffset,textureCoordinate.y-uTextureCoordOffset));
    glFragColor = vec4(red.r,green.g,blue.b,blue.a);
 }