#version 300 es
precision mediump float;
 
 in vec2 textureCoordinate;
 uniform sampler2D inputImageTexture;
 //控制曝光程度
 uniform float uAdditionalColor;
 out vec4 gl_FragColor;

 void main()
 {
    vec4 color = texture(inputImageTexture,textureCoordinate);
    gl_FragColor = vec4(color.r + uAdditionalColor,color.g+uAdditionalColor,color.b+uAdditionalColor,color.a);
 }