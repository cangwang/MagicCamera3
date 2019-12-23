#version 300 es
in highp vec2 textureCoordinate;

precision highp float;

uniform sampler2D inputImageTexture;
uniform sampler2D grey1Frame;
uniform sampler2D grey2Frame;
uniform sampler2D curve;

const mediump vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);
out vec4 glFragColor;

void main()
{
	lowp float satura = 0.5;
	float GreyVal;
	lowp vec4 textureColor;
	lowp vec4 textureColorRes;
	
	highp float redCurveValue;
	highp float greenCurveValue;
	highp float blueCurveValue;
	
	vec4 grey1Color;
	vec4 grey2Color;
	
	float xCoordinate = textureCoordinate.x;
	float yCoordinate = textureCoordinate.y;
	
	textureColor = texture( inputImageTexture, vec2(xCoordinate, yCoordinate));
	textureColorRes = textureColor; 
	
	grey1Color = texture(grey1Frame, vec2(xCoordinate, yCoordinate));
	grey2Color = texture(grey2Frame, vec2(xCoordinate, yCoordinate));
	
	// step 1. saturation 
	lowp float luminance = dot(textureColor.rgb, luminanceWeighting);
	lowp vec3 greyScaleColor = vec3(luminance);
	
	textureColor = vec4(mix(greyScaleColor, textureColor.rgb, satura), textureColor.w); 
	
	// step 2. level, blur curve, rgb curve
	redCurveValue = texture(curve, vec2(textureColor.r, 0.0)).r;
	redCurveValue = texture(curve, vec2(redCurveValue, 1.0/2.0)).r;
	
	greenCurveValue = texture(curve, vec2(textureColor.g, 0.0)).g;
	greenCurveValue = texture(curve, vec2(greenCurveValue, 1.0/2.0)).g;
	
	blueCurveValue = texture(curve, vec2(textureColor.b, 0.0)).b;
	blueCurveValue = texture(curve, vec2(blueCurveValue, 1.0/2.0)).b;
	blueCurveValue = texture(curve, vec2(blueCurveValue, 1.0/2.0)).g;
	
	lowp vec4 base = vec4(redCurveValue, greenCurveValue, blueCurveValue, 1.0);
	
	redCurveValue = texture(curve, vec2(redCurveValue, 1.0)).r;
	greenCurveValue = texture(curve, vec2(greenCurveValue, 1.0)).r;
	blueCurveValue = texture(curve, vec2(blueCurveValue, 1.0)).r;
	lowp vec4 overlayer = vec4(redCurveValue, greenCurveValue, blueCurveValue, 1.0); 
	//glFragColor = base * (1.0 - grey1Color.r) + overlayer * grey1Color.r;
	base = (base - overlayer) * (1.0 - grey1Color.r) + overlayer;
	
	redCurveValue = texture(curve, vec2(base.r, 1.0)).g;
	greenCurveValue = texture(curve, vec2(base.g, 1.0)).g;
	blueCurveValue = texture(curve, vec2(base.b, 1.0)).g;
	overlayer = vec4(redCurveValue, greenCurveValue, blueCurveValue, 1.0);
	
	textureColor = (base - overlayer) * (1.0 - grey2Color.r) + overlayer;
	//base * (grey2Color.r) + overlayer * (1.0 - grey2Color.r);
	
	glFragColor = vec4(textureColor.r, textureColor.g, textureColor.b, 1.0);
}