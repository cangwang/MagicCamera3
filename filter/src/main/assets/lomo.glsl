#version 300 es
precision mediump float;
 
 in mediump vec2 textureCoordinate;
 
 uniform sampler2D inputImageTexture;
 uniform sampler2D inputImageTexture2;
 uniform sampler2D inputImageTexture3;
 
 uniform float strength;
 out vec4 glFragColor;

 void main()
 {
     vec4 originColor = vec4(0.2,0.6,0.9,1.0);

     vec3 texel;
     vec2 tc = (2.0 * textureCoordinate) - 1.0;
     float d = dot(tc, tc);
     vec2 lookup = vec2(d, originColor.r);
     texel.r = texture(inputImageTexture3, lookup).r;
     lookup.y = originColor.g;
     texel.g = texture(inputImageTexture3, lookup).g;
     lookup.y = originColor.b;
     texel.b	= texture(inputImageTexture3, lookup).b;
     
     texel.rgb = mix(originColor.rgb, texel.rgb, strength);

     glFragColor = vec4(texel,1.0);
 }