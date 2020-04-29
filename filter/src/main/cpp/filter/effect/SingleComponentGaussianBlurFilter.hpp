#ifndef SingleComponentGaussianBlurFilter_hpp
#define SingleComponentGaussianBlurFilter_hpp

#include "../source/macros.hpp"
#include "FilterGroup.hpp"
#include "SingleComponentGaussianBlurMonoFilter.hpp"

NS_GI_BEGIN

    class SingleComponentGaussianBlurFilter : public FilterGroup {
    public:
        virtual ~SingleComponentGaussianBlurFilter();

        static SingleComponentGaussianBlurFilter* create(int radius = 4, float sigma = 2.0);
        bool init(int radius, float sigma);
        void setRadius(int radius);
        void setSigma(float sigma);

    protected:
        SingleComponentGaussianBlurFilter();

    private:
        SingleComponentGaussianBlurMonoFilter* _hBlurFilter;
        SingleComponentGaussianBlurMonoFilter* _vBlurFilter;
    };


NS_GI_END

#endif /* GaussianBlurFilter_hpp */