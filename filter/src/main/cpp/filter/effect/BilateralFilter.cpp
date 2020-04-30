#include "BilateralFilter.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(BilateralMonoFilter)

const std::string kBilateralBlurVertexShaderString = SHADER_STRING(
  ##version 300 es
  in vec4 postion;
  in vec4 texCoord;

  uniform float texelSpacingU;
  uniform float texelSpacingV;

  out vec2 blurCoordinates[9];

  void main(){
      gl_Position = position;
      vec2 texelSpacing = vec2(texelSpacingU, texelSpacingV);

      //3*3的卷积核
      blurCoordinates[0] = texCoord.xy + texelSpacing * float((0 - ((GAUSSIAN_SAMPLES - 1) / 2)));
      blurCoordinates[1] = texCoord.xy + texelSpacing * float((1 - ((GAUSSIAN_SAMPLES - 1) / 2)));
      blurCoordinates[2] = texCoord.xy + texelSpacing * float((2 - ((GAUSSIAN_SAMPLES - 1) / 2)));
      blurCoordinates[3] = texCoord.xy + texelSpacing * float((3 - ((GAUSSIAN_SAMPLES - 1) / 2)));
      blurCoordinates[4] = texCoord.xy + texelSpacing * float((4 - ((GAUSSIAN_SAMPLES - 1) / 2)));
      blurCoordinates[5] = texCoord.xy + texelSpacing * float((5 - ((GAUSSIAN_SAMPLES - 1) / 2)));
      blurCoordinates[6] = texCoord.xy + texelSpacing * float((6 - ((GAUSSIAN_SAMPLES - 1) / 2)));
      blurCoordinates[7] = texCoord.xy + texelSpacing * float((7 - ((GAUSSIAN_SAMPLES - 1) / 2)));
      blurCoordinates[8] = texCoord.xy + texelSpacing * float((8 - ((GAUSSIAN_SAMPLES - 1) / 2)));
  }
);

    const std::string kBilateralBlurFragmentShaderString = SHADER_STRING
    (
            ##version 300 es

            uniform sampler2D colorMap;

            const lowp int GAUSSIAN_SAMPLES = 9;

            in highp vec2 blurCoordinates[GAUSSIAN_SAMPLES];

            uniform mediump float distanceNormalizationFactor;
            out vec4 glFragColor;

            void main()
            {
                lowp vec4 centralColor;
                lowp float gaussianWeightTotal;
                lowp vec4 sum;
                lowp vec4 sampleColor;
                lowp float distanceFromCentralColor;
                lowp float gaussianWeight;

                centralColor = texture2D(colorMap, blurCoordinates[4]);
                gaussianWeightTotal = 0.18;
                sum = centralColor * 0.18;

                //3*3的卷积核
                sampleColor = texture(colorMap, blurCoordinates[0]);
                distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
                gaussianWeight = 0.05 * (1.0 - distanceFromCentralColor);
                gaussianWeightTotal += gaussianWeight;
                sum += sampleColor * gaussianWeight;

                sampleColor = texture(colorMap, blurCoordinates[1]);
                distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
                gaussianWeight = 0.09 * (1.0 - distanceFromCentralColor);
                gaussianWeightTotal += gaussianWeight;
                sum += sampleColor * gaussianWeight;

                sampleColor = texture(colorMap, blurCoordinates[2]);
                distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
                gaussianWeight = 0.12 * (1.0 - distanceFromCentralColor);
                gaussianWeightTotal += gaussianWeight;
                sum += sampleColor * gaussianWeight;

                sampleColor = texture(colorMap, blurCoordinates[3]);
                distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
                gaussianWeight = 0.15 * (1.0 - distanceFromCentralColor);
                gaussianWeightTotal += gaussianWeight;
                sum += sampleColor * gaussianWeight;

                sampleColor = texture(colorMap, blurCoordinates[5]);
                distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
                gaussianWeight = 0.15 * (1.0 - distanceFromCentralColor);
                gaussianWeightTotal += gaussianWeight;
                sum += sampleColor * gaussianWeight;

                sampleColor = texture(colorMap, blurCoordinates[6]);
                distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
                gaussianWeight = 0.12 * (1.0 - distanceFromCentralColor);
                gaussianWeightTotal += gaussianWeight;
                sum += sampleColor * gaussianWeight;

                sampleColor = texture(colorMap, blurCoordinates[7]);
                distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
                gaussianWeight = 0.09 * (1.0 - distanceFromCentralColor);
                gaussianWeightTotal += gaussianWeight;
                sum += sampleColor * gaussianWeight;

                sampleColor = texture(colorMap, blurCoordinates[8]);
                distanceFromCentralColor = min(distance(centralColor, sampleColor) * distanceNormalizationFactor, 1.0);
                gaussianWeight = 0.05 * (1.0 - distanceFromCentralColor);
                gaussianWeightTotal += gaussianWeight;
                sum += sampleColor * gaussianWeight;

                gl_FragColor = sum / gaussianWeightTotal;
            }
    );


BilateralMonoFilter::BilateralMonoFilter(GPUImage::BilateralMonoFilter::Type type)
:_type(type)
,_texelSpacingMultiplier(4.0)
,_distanceNormalizationFactor(8.0){

}

BilateralMonoFilter* BilateralMonoFilter::create(GPUImage::BilateralMonoFilter::Type type) {
    auto ret = new (std::nothrow) BilateralMonoFilter(type);
    if (!ret || !ret->init()) {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool BilateralMonoFilter::init() {
    return initWithShaderString(kBilateralBlurVertexShaderString,
                                kBilateralBlurFragmentShaderString);
}

bool BilateralMonoFilter::proceed(bool bUpdateTargets) {
    Framebuffer* inputFramebuffer = _inputFramebuffers.begin()->second.frameBuffer;
    RotationMode inputRotation = _inputFramebuffers.begin()->second.rotationMode;

    //横竖屏判断
    if (rotationSwapsSize(inputRotation))
    {
        //设置传参
        if (_type == HORIZONTAL) {
            _filterProgram->setUniformValue("texelSpacingU", (float)0.0);
            _filterProgram->setUniformValue("texelSpacingV", (float)(_texelSpacingMultiplier / _framebuffer->getWidth()));
        } else {
            _filterProgram->setUniformValue("texelSpacingU", (float)(_texelSpacingMultiplier / _framebuffer->getHeight()));
            _filterProgram->setUniformValue("texelSpacingV", (float)0.0);
        }
    } else {
        if (_type == HORIZONTAL) {
            _filterProgram->setUniformValue("texelSpacingU", (float)(_texelSpacingMultiplier / _framebuffer->getWidth()));
            _filterProgram->setUniformValue("texelSpacingV", (float)0.0);
        } else {
            _filterProgram->setUniformValue("texelSpacingU", (float)0.0);
            _filterProgram->setUniformValue("texelSpacingV", (float)(_texelSpacingMultiplier / _framebuffer->getHeight()));
        }
    }

    _filterProgram->setUniformValue("distanceNormalizationFactor",_distanceNormalizationFactor);

    return Filter::proceed(bUpdateTargets);
}


void BilateralMonoFilter::setTexelSpacingMuliplier(float multiplier) {
    _texelSpacingMultiplier = multiplier;
}

void BilateralMonoFilter::setDistanceNormalizationFactor(float value) {
    _distanceNormalizationFactor = value;
}

REGISTER_FILTER_CLASS(BilateralFilter)

BilateralFilter::BilateralFilter()
:_hBlurFilter(nullptr)
,_vBlurFilter(nullptr){

}

BilateralFilter::~BilateralFilter() {
    if(_hBlurFilter) {
        _hBlurFilter->release();
        _hBlurFilter = nullptr;
    }

    if (_vBlurFilter) {
        _vBlurFilter->release();
        _vBlurFilter = nullptr;
    }
}

BilateralFilter* BilateralFilter::create() {
    auto ret = new (std::nothrow) BilateralFilter();
    if (ret && !ret->init()) {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool BilateralFilter::init() {
    if(!FilterGroup::init()) {
        return false;
    }

    //横竖分别卷积一次
    _hBlurFilter = BilateralMonoFilter::create(BilateralMonoFilter::HORIZONTAL);
    _vBlurFilter = BilateralMonoFilter::create(BilateralMonoFilter::VERITICAL);
    _hBlurFilter->addTarget(_vBlurFilter);
    addFilter(_hBlurFilter);

    registerProperty("texelSpacingMuliplier",  4.0, "The texel spacing multiplier.",
            [this](float& texelSpacingMultiplier) {
        setTexelSpacingMultiplier(texelSpacingMultiplier);
    });

    registerProperty("distanceNormalizationFactor", 8.0, "The distance normalization factor.",
            [this](float& distanceNormalizationFactor){
        setDistanceNormalizationFactor(distanceNormalizationFactor);
    });

    return true;
}

void BilateralFilter::setTexelSpacingMultiplier(float multiplier) {
    _hBlurFilter->setTexelSpacingMuliplier(multiplier);
    _vBlurFilter->setTexelSpacingMuliplier(multiplier);
}

void BilateralFilter::setDistanceNormalizationFactor(float value) {
    _hBlurFilter->setDistanceNormalizationFactor(value);
    _vBlurFilter->setDistanceNormalizationFactor(value);
}

NS_GI_END