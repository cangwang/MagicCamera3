 #version 300 es
precision mediump float;
 
 in mediump vec2 textureCoordinate;
 
 uniform sampler2D inputImageTexture;
 uniform sampler2D inputImageTexture2; //blowout;
 uniform sampler2D inputImageTexture3; //overlay;
 uniform sampler2D inputImageTexture4; //map
 
 uniform float strength;
 out vec4 glFragColor;
 
 void main()
 {
     vec4 originColor = texture(inputImageTexture, textureCoordinate);
     vec4 texel = texture(inputImageTexture, textureCoordinate);
     vec3 bbTexel = texture(inputImageTexture2, textureCoordinate).rgb;
     
     texel.r = texture(inputImageTexture3, vec2(bbTexel.r, texel.r)).r;
     texel.g = texture(inputImageTexture3, vec2(bbTexel.g, texel.g)).g;
     texel.b = texture(inputImageTexture3, vec2(bbTexel.b, texel.b)).b;
     
     vec4 mapped;
     mapped.r = texture(inputImageTexture4, vec2(texel.r, .16666)).r;
     mapped.g = texture(inputImageTexture4, vec2(texel.g, .5)).g;
     mapped.b = texture(inputImageTexture4, vec2(texel.b, .83333)).b;
     mapped.a = 1.0;
     
     mapped.rgb = mix(originColor.rgb, mapped.rgb, strength);

     glFragColor = mapped;
 }