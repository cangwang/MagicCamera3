#ifndef BilateralFilter_hpp
#define BilateralFilter_hpp

#include "../source/macros.hpp"
#include "FilterGroup.hpp"
#include "Filter.hpp"

NS_GI_BEGIN
    class BilateralMonoFilter : public Filter {
    public:
        enum Type {
            HORIZONTAL,
            VERITICAL
        };
        static BilateralMonoFilter* create(Type type = HORIZONTAL);
        bool init();

        virtual bool proceed(bool bUpdateTargets = true) override ;
        void setTexelSpacingMuliplier(float multiplier);
        void setDistanceNormalizationFactor(float value);

    protected:
        BilateralMonoFilter(Type type);
        Type _type;
        float _texelSpacingMultiplier;
        float _distanceNormalizationFactor;
    };

    class BilateralFilter : public FilterGroup {
    public:
        virtual ~BilateralFilter();
        static BilateralFilter* create();
        bool init();

        void setTexelSpacingMultiplier(float multiplier);
        void setDistanceNormalizationFactor(float value);

    protected:
        BilateralFilter();

    private:
        BilateralMonoFilter* _hBlurFilter;
        BilateralMonoFilter* _vBlurFilter;
    };

NS_GI_END

#endif