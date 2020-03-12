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
class CannyEdgeDetecionFilter : public FilterGroup  {
public:
    static CannyEdgeDetecionFilter* create();
    bool init();

protected:
    CannyEdgeDetecionFilter();
    ~CannyEdgeDetecionFilter();


};


NS_GI_END

#endif