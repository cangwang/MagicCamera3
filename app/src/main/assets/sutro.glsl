 #version 300 es
precision mediump float;
 
 in mediump vec2 textureCoordinate;
 
 uniform sampler2D inputImageTexture;
 uniform sampler2D inputImageTexture2; //sutroMap;
 uniform sampler2D inputImageTexture3; //sutroMetal;
 uniform sampler2D inputImageTexture4; //softLight
 uniform sampler2D inputImageTexture5; //sutroEdgeburn
 uniform sampler2D inputImageTexture6; //sutroCurves 
 uniform float strength;
 out vec4 glFragColor;
 
 void main()
 {
     vec4 originColor = texture(inputImageTexture, textureCoordinate);
     vec3 texel = texture(inputImageTexture, textureCoordinate).rgb;
     
     vec2 tc = (2.0 * textureCoordinate) - 1.0;
     float d = dot(tc, tc);
     vec2 lookup = vec2(d, texel.r);
     texel.r = texture(inputImageTexture2, lookup).r;
     lookup.y = texel.g;
     texel.g = texture(inputImageTexture2, lookup).g;
     lookup.y = texel.b;
     texel.b	= texture(inputImageTexture2, lookup).b;
     
     vec3 rgbPrime = vec3(0.1019, 0.0, 0.0);
     float m = dot(vec3(.3, .59, .11), texel.rgb) - 0.03058;
     texel = mix(texel, rgbPrime + m, 0.32);
     
     vec3 metal = texture(inputImageTexture3, textureCoordinate).rgb;
     texel.r = texture(inputImageTexture4, vec2(metal.r, texel.r)).r;
     texel.g = texture(inputImageTexture4, vec2(metal.g, texel.g)).g;
     texel.b = texture(inputImageTexture4, vec2(metal.b, texel.b)).b;
     
     texel = texel * texture(inputImageTexture5, textureCoordinate).rgb;
     
     texel.r = texture(inputImageTexture6, vec2(texel.r, .16666)).r;
     texel.g = texture(inputImageTexture6, vec2(texel.g, .5)).g;
     texel.b = texture(inputImageTexture6, vec2(texel.b, .83333)).b;
     
     texel.rgb = mix(originColor.rgb, texel.rgb, strength);

     glFragColor = vec4(texel, 1.0);
 }