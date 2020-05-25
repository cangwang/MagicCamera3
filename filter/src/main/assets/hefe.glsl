#version 300 es
 precision mediump float;
 
 in mediump vec2 textureCoordinate;
 
 uniform sampler2D inputImageTexture;
 uniform sampler2D inputImageTexture2;  //edgeBurn
 uniform sampler2D inputImageTexture3;  //hefeMap
 uniform sampler2D inputImageTexture4;  //hefeGradientMap
 uniform sampler2D inputImageTexture5;  //hefeSoftLight
 uniform sampler2D inputImageTexture6;  //hefeMetal
 
 uniform float strength;
 out vec4 glFragColor;

 void main()
{
    vec4 originColor = texture(inputImageTexture, textureCoordinate);
    vec3 texel = texture(inputImageTexture, textureCoordinate).rgb;
    vec3 edge = texture(inputImageTexture2, textureCoordinate).rgb;
    texel = texel * edge;
    
    texel = vec3(
                 texture(inputImageTexture3, vec2(texel.r, .16666)).r,
                 texture(inputImageTexture3, vec2(texel.g, .5)).g,
                 texture(inputImageTexture3, vec2(texel.b, .83333)).b);

    vec3 luma = vec3(.30, .59, .11);
    vec3 gradSample = texture(inputImageTexture4, vec2(dot(luma, texel), .5)).rgb;
    vec3 final = vec3(
                      texture(inputImageTexture5, vec2(gradSample.r, texel.r)).r,
                      texture(inputImageTexture5, vec2(gradSample.g, texel.g)).g,
                      texture(inputImageTexture5, vec2(gradSample.b, texel.b)).b
                      );

    vec3 metal = texture(inputImageTexture6, textureCoordinate).rgb;
    vec3 metaled = vec3(
                        texture(inputImageTexture5, vec2(metal.r, texel.r)).r,
                        texture(inputImageTexture5, vec2(metal.g, texel.g)).g,
                        texture(inputImageTexture5, vec2(metal.b, texel.b)).b
                        );

    metaled.rgb = mix(originColor.rgb, metaled.rgb, strength);

    glFragColor = vec4(metaled, 1.0);
}