#version 300 es
precision mediump float;
in mediump vec2 textureCoordinate;
uniform sampler2D inputImageTexture;     //当前输入纹理
uniform sampler2D inputTextureLast; //上一次纹理
uniform sampler2D lookupTable;      // 颜色查找表纹理

//线性混合比例
const lowp vec3 blendValue = vec3(0.1,0.3,0.6);

out vec4 gl_FragColor;

vec4 getLutColor(vec4 textureColor,sample2D lookupTexture){
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

    highp vec2 texPos2;
    texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
    texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);

    lowp vec4 newColor1 = texture(lookupTexture,texPos1);
    lowp vec4 newColor2 = texture(lookupTexture,texPos2);

    lowp vec4 newColor = mix(newColor1,newColor2,fract(blueColor));
    vec4 color = vec4(newColor.rgb,textureColor.w);
    return color;
}

void main(){
    // 当前纹理颜色
    vec4 currentColor = texture(inputImageTexture,textureCoordinate);
     // 上一轮纹理颜色
    vec4 lastColor = texture(inputTextureLast,textureCoordinate);
    // lut映射的颜色值
    vec4 lutColor = getLutColor(currentColor,lookupTable);
    // 将lut映射之后的纹理与上一轮的纹理进行线性混合
    gl_FragColor = vec4(mix(lastColor.rgb,lutColor.rgb,blend),currentColor.w);
}