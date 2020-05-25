#version 300 es
in highp vec2 textureCoordinate;

precision highp float;

uniform sampler2D inputImageTexture;
uniform sampler2D curve;

out vec4 glFragColor;

void main()
{
	highp vec4 textureColor;
	highp vec4 textureColorRes;
	highp float satVal = 65.0 / 100.0;
	
	float xCoordinate = textureCoordinate.x;
	float yCoordinate = textureCoordinate.y;
	
	highp float redCurveValue;
	highp float greenCurveValue;
	highp float blueCurveValue;
	
	textureColor = texture( inputImageTexture, vec2(xCoordinate, yCoordinate));
	textureColorRes = textureColor;
	
	redCurveValue = texture(curve, vec2(textureColor.r, 0.0)).r;
	greenCurveValue = texture(curve, vec2(textureColor.g, 0.0)).g;
	blueCurveValue = texture(curve, vec2(textureColor.b, 0.0)).b;
	
	highp float G = (redCurveValue + greenCurveValue + blueCurveValue);
	G = G / 3.0;
	
	redCurveValue = ((1.0 - satVal) * G + satVal * redCurveValue);
	greenCurveValue = ((1.0 - satVal) * G + satVal * greenCurveValue);
	blueCurveValue = ((1.0 - satVal) * G + satVal * blueCurveValue);
	redCurveValue = (((redCurveValue) > (1.0)) ? (1.0) : (((redCurveValue) < (0.0)) ? (0.0) : (redCurveValue)));
	greenCurveValue = (((greenCurveValue) > (1.0)) ? (1.0) : (((greenCurveValue) < (0.0)) ? (0.0) : (greenCurveValue)));
	blueCurveValue = (((blueCurveValue) > (1.0)) ? (1.0) : (((blueCurveValue) < (0.0)) ? (0.0) : (blueCurveValue)));
	
	redCurveValue = texture(curve, vec2(redCurveValue, 0.0)).a;
	greenCurveValue = texture(curve, vec2(greenCurveValue, 0.0)).a;
	blueCurveValue = texture(curve, vec2(blueCurveValue, 0.0)).a;
	
	highp vec4 base = vec4(redCurveValue, greenCurveValue, blueCurveValue, 1.0);
	highp vec4 overlayer = vec4(250.0/255.0, 227.0/255.0, 193.0/255.0, 1.0);
	
	textureColor = overlayer * base;
	base = (textureColor - base) * 0.850980 + base;
	textureColor = base; 
	
	glFragColor = vec4(textureColor.r, textureColor.g, textureColor.b, 1.0);
}
  