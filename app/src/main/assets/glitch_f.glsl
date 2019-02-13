#version 300 es
precision highp float;
 
 in vec2 textureCoordinate;
 uniform sampler2D inputImageTexture;
 //这是个二阶向量，x是横向偏移的值，y是阈值
 uniform vec2 uScanLineJitter;
 //颜色偏移的值
 uniform float uColorDrift;
 out vec4 glFragColor;

 float nrand(in float x,in float y){
    //fract(x) = x - floor(x);
    //dot是向量点乘，sin就是正弦函数
    return fract(sin(dot(vec2(x,y) ,vec2(12.9898,78.233))) * 43758.5453);
 }

 void main()
 {
    float u = textureCoordinate.x;
    float v = textureCoordinate.y;
    //用y计算0~1的随机值，再取值-1~1的数
    float jitter = nrand(v ,0.0) * 2.0 - 1.0;
    float drift = uColorDrift;
    //计算向左或向右偏移
    //step是gl自带函数，意思是，如果第一个参数大于第二个参数，那么返回0，否则返回1
    float offsetParam = step(uScanLineJitter.y,abs(jitter));
    //如果offset为0就不偏移，如果为1，就偏移jtter*uScanLineJitter.x的位置
    jitter = jitter * offsetParam * uScanLineJitter.x;
    //这里计算最终的像素值，纹理坐标是0到1之间的数，如果小于0，那么图像就捅到屏幕右边去，如果超过1，那么就捅到屏幕左边去。
    vec4 color1 = texture(inputImageTexture,fract(vec2(u + jitter ,v)));
    vec4 color2 = texture(inputImageTexture,fract(vec2(u + jitter + v * drift ,v)));
    glFragColor = vec4(color1.r ,color2.g ,color1.b ,1.0);
 }