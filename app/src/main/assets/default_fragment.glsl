#version 300 es
#extension GL_OES_EGL_image_external_essl3 : require

precision mediump float;

in mediump vec2 textureCoordinate;

uniform samplerExternalOES inputImageTexture;
uniform vec2 singleStepOffset;
uniform mediump float params;

const highp vec3 W = vec3(0.299,0.587,0.114);
//创建20个点取数据
vec2 blurCoordinates[20];

out vec4 glFragColor;

//强光处理
float hardLight(float color)
{
	if(color <= 0.5)
		color = color * color * 2.0;
	else  //叠加混合模式
		color = 1.0 - ((1.0 - color)*(1.0 - color) * 2.0);
	return color;
}

void main(){

    vec3 centralColor = texture(inputImageTexture, textureCoordinate).rgb;
    if(params != 0.0){
        //取出20个点的绿色通道值
        blurCoordinates[0] = textureCoordinate.xy + singleStepOffset * vec2(0.0, -10.0);
        blurCoordinates[1] = textureCoordinate.xy + singleStepOffset * vec2(0.0, 10.0);
        blurCoordinates[2] = textureCoordinate.xy + singleStepOffset * vec2(-10.0, 0.0);
        blurCoordinates[3] = textureCoordinate.xy + singleStepOffset * vec2(10.0, 0.0);
        blurCoordinates[4] = textureCoordinate.xy + singleStepOffset * vec2(5.0, -8.0);
        blurCoordinates[5] = textureCoordinate.xy + singleStepOffset * vec2(5.0, 8.0);
        blurCoordinates[6] = textureCoordinate.xy + singleStepOffset * vec2(-5.0, 8.0);
        blurCoordinates[7] = textureCoordinate.xy + singleStepOffset * vec2(-5.0, -8.0);
        blurCoordinates[8] = textureCoordinate.xy + singleStepOffset * vec2(8.0, -5.0);
        blurCoordinates[9] = textureCoordinate.xy + singleStepOffset * vec2(8.0, 5.0);
        blurCoordinates[10] = textureCoordinate.xy + singleStepOffset * vec2(-8.0, 5.0);
        blurCoordinates[11] = textureCoordinate.xy + singleStepOffset * vec2(-8.0, -5.0);
        blurCoordinates[12] = textureCoordinate.xy + singleStepOffset * vec2(0.0, -6.0);
        blurCoordinates[13] = textureCoordinate.xy + singleStepOffset * vec2(0.0, 6.0);
        blurCoordinates[14] = textureCoordinate.xy + singleStepOffset * vec2(6.0, 0.0);
        blurCoordinates[15] = textureCoordinate.xy + singleStepOffset * vec2(-6.0, 0.0);
        blurCoordinates[16] = textureCoordinate.xy + singleStepOffset * vec2(-4.0, -4.0);
        blurCoordinates[17] = textureCoordinate.xy + singleStepOffset * vec2(-4.0, 4.0);
        blurCoordinates[18] = textureCoordinate.xy + singleStepOffset * vec2(4.0, -4.0);
        blurCoordinates[19] = textureCoordinate.xy + singleStepOffset * vec2(4.0, 4.0);

        float sampleColor = centralColor.g * 20.0;
        sampleColor += texture(inputImageTexture, blurCoordinates[0]).g;
        sampleColor += texture(inputImageTexture, blurCoordinates[1]).g;
        sampleColor += texture(inputImageTexture, blurCoordinates[2]).g;
        sampleColor += texture(inputImageTexture, blurCoordinates[3]).g;
        sampleColor += texture(inputImageTexture, blurCoordinates[4]).g;
        sampleColor += texture(inputImageTexture, blurCoordinates[5]).g;
        sampleColor += texture(inputImageTexture, blurCoordinates[6]).g;
        sampleColor += texture(inputImageTexture, blurCoordinates[7]).g;
        sampleColor += texture(inputImageTexture, blurCoordinates[8]).g;
        sampleColor += texture(inputImageTexture, blurCoordinates[9]).g;
        sampleColor += texture(inputImageTexture, blurCoordinates[10]).g;
        sampleColor += texture(inputImageTexture, blurCoordinates[11]).g;
        sampleColor += texture(inputImageTexture, blurCoordinates[12]).g * 2.0;
        sampleColor += texture(inputImageTexture, blurCoordinates[13]).g * 2.0;
        sampleColor += texture(inputImageTexture, blurCoordinates[14]).g * 2.0;
        sampleColor += texture(inputImageTexture, blurCoordinates[15]).g * 2.0;
        sampleColor += texture(inputImageTexture, blurCoordinates[16]).g * 2.0;
        sampleColor += texture(inputImageTexture, blurCoordinates[17]).g * 2.0;
        sampleColor += texture(inputImageTexture, blurCoordinates[18]).g * 2.0;
        sampleColor += texture(inputImageTexture, blurCoordinates[19]).g * 2.0;

        sampleColor = sampleColor / 48.0;
        //用原图绿色通道值减去SampleColor,加上0.5（即128），得到PS中的高反差保留
        float highPass = centralColor.g - sampleColor + 0.5;
        //进行5次强光处理（叠加混合模式）
        for(int i = 0; i < 5;i++)
        {
            highPass = hardLight(highPass);
        }
        //计算原图的灰度值
        float luminance = dot(centralColor, W);
        //将灰度值作为阈值，用于排除非皮肤部分，根据灰度值计算，将
        float alpha = pow(luminance, params);

        vec3 smoothColor = centralColor + (centralColor-vec3(highPass))*alpha*0.1;
        //以灰度值作为透明度将原图与混合后结果进行滤色、柔光等混合，并调节饱和度
        glFragColor = vec4(mix(smoothColor.rgb, max(smoothColor, centralColor), alpha), 1.0);
    }else{
        glFragColor = vec4(centralColor.rgb,1.0);
    }
}