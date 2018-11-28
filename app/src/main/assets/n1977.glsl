 #version 300 es
precision mediump float;

 in mediump vec2 textureCoordinate;

 uniform sampler2D inputImageTexture;
 uniform sampler2D inputImageTexture2;
  out vec4 gl_FragColor;

 void main()
 {

     vec3 texel = texture(inputImageTexture, textureCoordinate).rgb;

     texel = vec3(
                  texture(inputImageTexture2, vec2(texel.r, .16666)).r,
                  texture(inputImageTexture2, vec2(texel.g, .5)).g,
                  texture(inputImageTexture2, vec2(texel.b, .83333)).b);

     gl_FragColor = vec4(texel, 1.0);
 }
