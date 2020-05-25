#version 300 es
precision mediump float;
 
 in vec2 textureCoordinate;
 uniform sampler2D inputImageTexture;
 out vec4 glFragColor;

 void main()
 {
    highp float y;
    if(textureCoordinate.y>=0.0 &&textureCoordinate.y<=0.33){
       y = textureCoordinate.y + 0.33;
       glFragColor = texture(inputImageTexture,vec2(textureCoordinate.x,y));
    }else if(textureCoordinate.y>0.33 &&textureCoordinate.y<=0.66){
       glFragColor = texture(inputImageTexture,textureCoordinate);
    }else{
       y = textureCoordinate.y - 0.33;
       glFragColor = texture(inputImageTexture,vec2(textureCoordinate.x,y));
    }
 }