#version 300 es
 precision mediump float;

 in mediump vec2 textureCoordinate;

 uniform sampler2D inputImageTexture;
 uniform sampler2D inputImageTexture2; //earlyBirdCurves
 uniform sampler2D inputImageTexture3; //earlyBirdOverlay
 uniform sampler2D inputImageTexture4; //vig
 uniform sampler2D inputImageTexture5; //earlyBirdBlowout
 uniform sampler2D inputImageTexture6; //earlyBirdMap
 
 out vec4 gl_FragColor;

 const mat3 saturate = mat3(
                            1.210300,
                            -0.089700,
                            -0.091000,
                            -0.176100,
                            1.123900,
                            -0.177400,
                            -0.034200,
                            -0.034200,
                            1.265800);
 const vec3 rgbPrime = vec3(0.25098, 0.14640522, 0.0);
 const vec3 desaturate = vec3(.3, .59, .11);

 void main()
 {

     vec3 texel = texture(inputImageTexture, textureCoordinate).rgb;


     vec2 lookup;
     lookup.y = 0.5;

     lookup.x = texel.r;
     texel.r = texture(inputImageTexture2, lookup).r;

     lookup.x = texel.g;
     texel.g = texture(inputImageTexture2, lookup).g;

     lookup.x = texel.b;
     texel.b = texture(inputImageTexture2, lookup).b;

     float desaturatedColor;
     vec3 result;
     desaturatedColor = dot(desaturate, texel);


     lookup.x = desaturatedColor;
     result.r = texture(inputImageTexture3, lookup).r;
     lookup.x = desaturatedColor;
     result.g = texture(inputImageTexture3, lookup).g;
     lookup.x = desaturatedColor;
     result.b = texture(inputImageTexture3, lookup).b;

     texel = saturate * mix(texel, result, .5);

     vec2 tc = (2.0 * textureCoordinate) - 1.0;
     float d = dot(tc, tc);

     vec3 sampled;
     lookup.y = .5;

     /*
      lookup.x = texel.r;
      sampled.r = texture(inputImageTexture4, lookup).r;

      lookup.x = texel.g;
      sampled.g = texture(inputImageTexture4, lookup).g;

      lookup.x = texel.b;
      sampled.b = texture(inputImageTexture4, lookup).b;

      float value = smoothstep(0.0, 1.25, pow(d, 1.35)/1.65);
      texel = mix(texel, sampled, value);
      */

     //---

     lookup = vec2(d, texel.r);
     texel.r = texture(inputImageTexture4, lookup).r;
     lookup.y = texel.g;
     texel.g = texture(inputImageTexture4, lookup).g;
     lookup.y = texel.b;
     texel.b	= texture(inputImageTexture4, lookup).b;
     float value = smoothstep(0.0, 1.25, pow(d, 1.35)/1.65);

     //---

     lookup.x = texel.r;
     sampled.r = texture(inputImageTexture5, lookup).r;
     lookup.x = texel.g;
     sampled.g = texture(inputImageTexture5, lookup).g;
     lookup.x = texel.b;
     sampled.b = texture(inputImageTexture5, lookup).b;
     texel = mix(sampled, texel, value);


     lookup.x = texel.r;
     texel.r = texture(inputImageTexture6, lookup).r;
     lookup.x = texel.g;
     texel.g = texture(inputImageTexture6, lookup).g;
     lookup.x = texel.b;
     texel.b = texture(inputImageTexture6, lookup).b;

     gl_FragColor = vec4(texel, 1.0);
 }
