 #version 300 es
 precision mediump float;

 in mediump vec2 textureCoordinate;

 uniform sampler2D inputImageTexture;
 uniform sampler2D inputImageTexture2;
 out vec4 glFragColor;

 void main()
 {
     vec3 texel = texture(inputImageTexture, textureCoordinate).rgb;

     vec2 lookup;
     lookup.y = .5;

     lookup.x = texel.r;
     texel.r = texture(inputImageTexture2, lookup).r;

     lookup.x = texel.g;
     texel.g = texture(inputImageTexture2, lookup).g;

     lookup.x = texel.b;
     texel.b = texture(inputImageTexture2, lookup).b;

     glFragColor = vec4(texel, 1.0);
 }
