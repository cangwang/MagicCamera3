#ifndef GaussianBlurMonoFilter_hpp
#define GaussianBlurMonoFilter_hpp

#include "../source/macros.hpp"
#include "FilterGroup.hpp"

NS_GI_BEGIN

    class GaussianBlurMonoFilter : public Filter {
    public:
        enum Type {HORIZONTAL, VERTICAL};

        static GaussianBlurMonoFilter* create(Type type = HORIZONTAL,
                                              int radius = 4, float sigma = 2.0);
        bool init(int radius, float sigma);

        void setRadius(int radius);
        void setSigma(float sigma);

        virtual bool proceed(bool bUpdateTargets = true) override ;

    protected:
        GaussianBlurMonoFilter(Type type = HORIZONTAL);
        Type _type;
        int _radius;
        float _sigma;

    private:

        virtual std::string _generateOptimizedVertexShaderString(int radius, float sigma);
        virtual std::string _generateOptimizedFragmentShaderString(int radius, float sigma);
    };


NS_GI_END

#endif