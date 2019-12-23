 #version 300 es
in highp vec2 textureCoordinate;
precision highp float; 
  
uniform sampler2D inputImageTexture;
uniform sampler2D curve;
uniform sampler2D greyFrame;
uniform sampler2D layerImage;

 out vec4 glFragColor;

void main()
{ 
	lowp vec4 textureColor; 
	vec4 greyColor;
	vec4 layerColor;
	
	float xCoordinate = textureCoordinate.x;
	float yCoordinate = textureCoordinate.y;
	
	highp float redCurveValue;
	highp float greenCurveValue; 
	highp float blueCurveValue;
	
	textureColor = texture( inputImageTexture, vec2(xCoordinate, yCoordinate));
	
	greyColor = texture(greyFrame, vec2(xCoordinate, yCoordinate));
	layerColor = texture(layerImage, vec2(xCoordinate, yCoordinate));

	// step1 curve
	redCurveValue = texture(curve, vec2(textureColor.r, 0.0)).r; 
	greenCurveValue = texture(curve, vec2(textureColor.g, 0.0)).g;
	blueCurveValue = texture(curve, vec2(textureColor.b, 0.0)).b; 

    // step2 curve with mask 
	textureColor = vec4(redCurveValue, greenCurveValue, blueCurveValue, 1.0);

	redCurveValue = texture(curve, vec2(textureColor.r, 0.0)).a;
	greenCurveValue = texture(curve, vec2(textureColor.g, 0.0)).a; 
    blueCurveValue = texture(curve, vec2(textureColor.b, 0.0)).a; 

	lowp vec4 textureColor2 = vec4(redCurveValue, greenCurveValue, blueCurveValue, 1.0); 

	// step3 screen with 60% 
	lowp vec4 base = vec4(mix(textureColor.rgb, textureColor2.rgb, 1.0 - greyColor.r), textureColor.a); 
	lowp vec4 overlayer = vec4(layerColor.r, layerColor.g, layerColor.b, 1.0);

    // screen blending 
	textureColor = 1.0 - ((1.0 - base) * (1.0 - overlayer));
	textureColor = (textureColor - base) * 0.6 + base;
	
	redCurveValue = texture(curve, vec2(textureColor.r, 1.0)).r; 
	greenCurveValue = texture(curve, vec2(textureColor.g, 1.0)).g;
	blueCurveValue = texture(curve, vec2(textureColor.b, 1.0)).b; 
	textureColor = vec4(redCurveValue, greenCurveValue, blueCurveValue, 1.0); 

	glFragColor = vec4(textureColor.r, textureColor.g, textureColor.b, 1.0);
}