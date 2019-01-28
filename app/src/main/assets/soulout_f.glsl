#version 300 es
precision mediump float;
 
 in vec2 textureCoordinate;
 uniform sampler2D inputImageTexture;
 uniform float uAlpha;
 out vec4 gl_FragColor;

 void main()
 {
    gl_FragColor = vec4(texture(inputImageTexture,textureCoordinate).rgb,uAlpha);
 }