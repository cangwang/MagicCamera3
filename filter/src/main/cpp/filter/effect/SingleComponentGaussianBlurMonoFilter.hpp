#ifndef SingleComponentGaussianBlurMonoFilter_hpp
#define SingleComponentGaussianBlurMonoFilter_hpp

#include "../source/macros.hpp"
#include "GaussianBlurMonoFilter.hpp"

NS_GI_BEGIN

class SingleComponentGaussianBlurMonoFilter: public GaussianBlurMonoFilter {
public:
    static SingleComponentGaussianBlurMonoFilter* create(Type type = HORIZONTAL, int radius = 4,
            float sigma = 2.0);

protected:
    SingleComponentGaussianBlurMonoFilter(Type type = HORIZONTAL);

private:
    std::string _generateOptimizedVertexShaderString(int radius, float sigma) override ;
    std::string _generateOptimizedFragmentShaderString(int radius,float sigma) override ;
};

NS_GI_END

#endif
