 #version 300 es
precision mediump float;
 
in mediump vec2 textureCoordinate;
 
uniform sampler2D inputImageTexture;
uniform sampler2D inputImageTexture2; //map
uniform sampler2D inputImageTexture3; //vigMap

uniform float strength;
 out vec4 gl_FragColor;
 
void main()
{
    vec4 originColor = texture(inputImageTexture, textureCoordinate);
    vec3 texel = texture(inputImageTexture, textureCoordinate).rgb;
     
    texel = vec3(
                  texture(inputImageTexture2, vec2(texel.r, .16666)).r,
                  texture(inputImageTexture2, vec2(texel.g, .5)).g,
                  texture(inputImageTexture2, vec2(texel.b, .83333)).b);
     
    vec2 tc = (2.0 * textureCoordinate) - 1.0;
    float d = dot(tc, tc);
    vec2 lookup = vec2(d, texel.r);
    texel.r = texture(inputImageTexture3, lookup).r;
    lookup.y = texel.g;
    texel.g = texture(inputImageTexture3, lookup).g;
    lookup.y = texel.b;
    texel.b	= texture(inputImageTexture3, lookup).b;
     
    texel.rgb = mix(originColor.rgb, texel.rgb, strength);

    gl_FragColor = vec4(texel, 1.0);
}