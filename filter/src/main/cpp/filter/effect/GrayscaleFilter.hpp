#ifndef GrayscaleFilter_hpp
#define GrayscaleFilter_hpp

#include "../source/macros.hpp"
#include "Filter.hpp"

NS_GI_BEGIN

class GrayscaleFilter : public Filter {
public:
    static GrayscaleFilter* create();
    bool init();

    virtual bool proceed(bool updateTargets = true) override ;

protected:
    GrayscaleFilter() {};
};


NS_GI_END

#endif