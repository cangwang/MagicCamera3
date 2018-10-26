varying highp vec2 textureCoordinate;
uniform smapler2D inputImageTexture;

 void main()
 {
    gl_FragColor= texture2D(inputImageTexture,textureCoordinate)
 }