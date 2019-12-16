#version 300 es
precision mediump float;
in mediump vec2 textureCoordinate;
uniform sampler2D inputImageTexture;     //当前输入纹理
uniform sampler2D inputTextureLast; //上一次纹理
uniform sampler2D lookupTable;      // 颜色查找表纹理

out vec4 glFragColor;

//固定的Lut纹理对换计算
vec4 getLutColor(vec4 textureColor,sampler2D lookupTexture){
    float blueColor = textureColor.b * 63.0;

    mediump vec2 quad1;
    quad1.y = floor(floor(blueColor)/8.0);
    quad1.x = floor(blueColor) - quad1.y*8.0;

    mediump vec2 quad2;
    quad2.y = floor(ceil(blueColor) /8.0);
    quad2.x = ceil(blueColor) - quad2.y*8.0;

    highp vec2 texPos1;
    texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
    texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);
    texPos1.y = 1.0-texPos1.y;

    highp vec2 texPos2;
    texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
    texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);
    texPos2.y = 1.0-texPos2.y;

    lowp vec4 newColor1 = texture(lookupTexture,texPos1);
    lowp vec4 newColor2 = texture(lookupTexture,texPos2);

    lowp vec4 newColor = mix(newColor1,newColor2,fract(blueColor));
    return newColor;
}

void main(){
    //上一帧纹理
    vec4 lastFrame = texture(inputTextureLast,textureCoordinate);
    //此帧对应的Lut转换纹理
    vec4 currentFrame = getLutColor(texture(inputImageTexture,textureCoordinate),lookupTable);
    //上一帧和此帧混色处理
    glFragColor = vec4(0.95 * lastFrame.r  +  0.05* currentFrame.r,currentFrame.g * 0.2 + lastFrame.g * 0.8, currentFrame.b,1.0);
}