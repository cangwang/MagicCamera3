#version 300 es
precision mediump float;
 
 in mediump vec2 textureCoordinate;
 
 uniform sampler2D inputImageTexture;
 uniform sampler2D inputImageTexture2;  //process
 uniform sampler2D inputImageTexture3;  //blowout
 uniform sampler2D inputImageTexture4;  //contrast
 uniform sampler2D inputImageTexture5;  //luma
 uniform sampler2D inputImageTexture6;  //screen
 out vec4 glFragColor;
 
 mat3 saturateMatrix = mat3(
                            1.105150, -0.044850,-0.046000,
                            -0.088050,1.061950,-0.089200,
                            -0.017100,-0.017100,1.132900);
 
 vec3 luma = vec3(.3, .59, .11);
 
 uniform float strength;

 void main()
 {
     vec4 originColor = texture(inputImageTexture, textureCoordinate);
     vec3 texel = texture(inputImageTexture, textureCoordinate).rgb;
     
     vec2 lookup;
     lookup.y = 0.5;
     lookup.x = texel.r;
     texel.r = texture(inputImageTexture2, lookup).r;
     lookup.x = texel.g;
     texel.g = texture(inputImageTexture2, lookup).g;
     lookup.x = texel.b;
     texel.b = texture(inputImageTexture2, lookup).b;
     
     texel = saturateMatrix * texel;
     
     
     vec2 tc = (2.0 * textureCoordinate) - 1.0;
     float d = dot(tc, tc);
     vec3 sampled;
     lookup.y = 0.5;
     lookup.x = texel.r;
     sampled.r = texture(inputImageTexture3, lookup).r;
     lookup.x = texel.g;
     sampled.g = texture(inputImageTexture3, lookup).g;
     lookup.x = texel.b;
     sampled.b = texture(inputImageTexture3, lookup).b;
     float value = smoothstep(0.0, 1.0, d);
     texel = mix(sampled, texel, value);
     
     lookup.x = texel.r;
     texel.r = texture(inputImageTexture4, lookup).r;
     lookup.x = texel.g;
     texel.g = texture(inputImageTexture4, lookup).g;
     lookup.x = texel.b;
     texel.b = texture(inputImageTexture4, lookup).b;
     
     
     lookup.x = dot(texel, luma);
     texel = mix(texture(inputImageTexture5, lookup).rgb, texel, .5);
     
     lookup.x = texel.r;
     texel.r = texture(inputImageTexture6, lookup).r;
     lookup.x = texel.g;
     texel.g = texture(inputImageTexture6, lookup).g;
     lookup.x = texel.b;
     texel.b = texture(inputImageTexture6, lookup).b;
     
     texel = mix(originColor.rgb, texel.rgb, strength);

     glFragColor = vec4(texel, 1.0);
 }