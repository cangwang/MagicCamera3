#version 300 es
precision mediump float;
 
 uniform mat4 uMvpMatrix;
 in vec4 position;
 in vec4 inputTextureCoordinate;
 out vec2 textureCoordinate;

 void main()
 {
    gl_Position = uMvpMatrix * vec4(position.xy,0.1,1.0);
//    gl_Position = vec4(position.xy,0.1,1.0);
    textureCoordinate = inputTextureCoordinate.xy;
 }