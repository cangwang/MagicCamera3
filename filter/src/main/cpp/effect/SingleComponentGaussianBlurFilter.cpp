#include <cmath>
#include "SingleComponentGaussianBlurFilter.hpp"
#include "../source/util.h"

NS_GI_BEGIN

    REGISTER_FILTER_CLASS(SingleComponentGaussianBlurFilter)

    SingleComponentGaussianBlurFilter::SingleComponentGaussianBlurFilter()
            :_hBlurFilter(nullptr)
            ,_vBlurFilter(nullptr)
    {
    }

    SingleComponentGaussianBlurFilter::~SingleComponentGaussianBlurFilter() {
        if (_hBlurFilter) {
            _hBlurFilter->release();
            _hBlurFilter = nullptr;
        }

        if (_vBlurFilter) {
            _vBlurFilter->release();
            _vBlurFilter = nullptr;
        }

    }

    SingleComponentGaussianBlurFilter* SingleComponentGaussianBlurFilter::create(int radius/* = 4*/, float sigma/* = 2.0*/) {
        auto* ret = new (std::nothrow) SingleComponentGaussianBlurFilter();
        if (ret && !ret->init(radius, sigma)) {
            delete ret;
            ret = 0;
        }
        return ret;
    }

    bool SingleComponentGaussianBlurFilter::init(int radius, float sigma) {
        if (!FilterGroup::init()) {
            return false;
        }

        _hBlurFilter = SingleComponentGaussianBlurMonoFilter::create(SingleComponentGaussianBlurMonoFilter::HORIZONTAL, radius, sigma);
        _vBlurFilter = SingleComponentGaussianBlurMonoFilter::create(SingleComponentGaussianBlurMonoFilter::VERTICAL, radius, sigma);
        _hBlurFilter->addTarget(_vBlurFilter);
        addFilter(_hBlurFilter);

        registerProperty("radius", 4, "", [this](int& radius){
            setRadius(radius);
        });

        registerProperty("sigma", 2.0, "", [this](float& sigma){
            setSigma(sigma);
        });

        return true;
    }

    void SingleComponentGaussianBlurFilter::setRadius(int radius) {
        _hBlurFilter->setRadius(radius);
        _vBlurFilter->setRadius(radius);
    }

    void SingleComponentGaussianBlurFilter::setSigma(float sigma) {
        _hBlurFilter->setSigma(sigma);
        _vBlurFilter->setSigma(sigma);
    }

NS_GI_END
