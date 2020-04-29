#ifndef CannyEdgeDetectionFilter_hpp
#define CannyEdgeDetectionFilter_hpp

#include "../source/macros.hpp"
#include "FilterGroup.hpp"
#include "GrayscaleFilter.hpp"
#include "SingleComponentGaussianBlurFilter.hpp"
#include "DirectionalSobelEdgeDetectionFilter.hpp"
#include "DirectionalNonMaximumSuppressionFilter.hpp"
#include "WeakPixelInclusionFilter.hpp"

NS_GI_BEGIN
class CannyEdgeDetectionFilter : public FilterGroup  {
public:
    static CannyEdgeDetectionFilter* create();
    bool init();

protected:
    CannyEdgeDetectionFilter();
    ~CannyEdgeDetectionFilter();

    GrayscaleFilter* _grayscaleFilter;
    SingleComponentGaussianBlurFilter* _blurFilter;
    DirectionalSobelEdgeDetectionFilter* _edgeDetectionFilter;
    DirectionalNonMaximumSuppressionFilter* _nonMaximumSuppressionFilter;
    WeakPixelInclusionFilter* _weakPixelInclusionFilter;
};


NS_GI_END

#endif