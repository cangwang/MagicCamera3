#version 300 es
precision mediump float;
 //每个点的xy坐标
 in vec2 textureCoordinate;
 //对应纹理
 uniform sampler2D inputImageTexture;
 uniform float uTextureCoordOffset;
 out vec4 glFragColor;

 void main()
 {
    //直接采样蓝色色值
    vec4 blue = texture(inputImageTexture,textureCoordinate);
     //从效果看，绿色和红色色值特别明显，所以需要对其色值偏移。绿色和红色需要分开方向，不然重叠一起会混色。
     //坐标向左上偏移，然后再采样色值
    vec4 green = texture(inputImageTexture, vec2(textureCoordinate.x + uTextureCoordOffset, textureCoordinate.y + uTextureCoordOffset));
     //坐标向右下偏移，然后再采样色值
    vec4 red = texture(inputImageTexture,vec2(textureCoordinate.x - uTextureCoordOffset,textureCoordinate.y - uTextureCoordOffset));
     //RG两个经过偏移后分别采样，B沿用原来的色值，透明度为1，组合最终输出
    glFragColor = vec4(red.r,green.g,blue.b,blue.a);
 }