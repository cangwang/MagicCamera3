#version 300 es
in highp vec2 textureCoordinate; 
precision highp float; 

uniform sampler2D inputImageTexture; 
uniform sampler2D curve;
out vec4 glFragColor;

void main()
{ 
	lowp vec4 textureColor;
	lowp vec4 textureColorRes; 
	lowp vec4 textureColorOri; 
	vec4 grey1Color; 
	vec4 layerColor; 
	mediump float satVal = 115.0 / 100.0; 

    float xCoordinate = textureCoordinate.x;
	float yCoordinate = textureCoordinate.y; 
	
	highp float redCurveValue; 
	highp float greenCurveValue; 
	highp float blueCurveValue; 

	textureColor = texture( inputImageTexture, vec2(xCoordinate, yCoordinate)); 
    textureColorRes = textureColor; 
	textureColorOri = textureColor; 

	// step1. screen blending 
	textureColor = 1.0 - ((1.0 - textureColorOri) * (1.0 - textureColorOri)); 
	textureColor = (textureColor - textureColorOri) + textureColorOri; 

	// step2. curve 
	redCurveValue = texture(curve, vec2(textureColor.r, 0.0)).r; 
	greenCurveValue = texture(curve, vec2(textureColor.g, 0.0)).g; 
    blueCurveValue = texture(curve, vec2(textureColor.b, 0.0)).b; 

	// step3. saturation 
	highp float G = (redCurveValue + greenCurveValue + blueCurveValue); 
	G = G / 3.0; 

    redCurveValue = ((1.0 - satVal) * G + satVal * redCurveValue); 
	greenCurveValue = ((1.0 - satVal) * G + satVal * greenCurveValue); 
	blueCurveValue = ((1.0 - satVal) * G + satVal * blueCurveValue); 

	textureColor = vec4(redCurveValue, greenCurveValue, blueCurveValue, 1.0); 

    glFragColor = vec4(textureColor.r, textureColor.g, textureColor.b, 1.0);
}
