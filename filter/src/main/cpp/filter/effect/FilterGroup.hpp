#ifndef FilterGroup_hpp
#define FilterGroup_hpp

#include "../source/macros.hpp"
#include "../source/Source.hpp"
#include "../source/Target.hpp"
#include <vector>
#include "../source/Filter.hpp"

NS_GI_BEGIN

class FilterGroup: public Filter {
public:
    virtual ~FilterGroup();

    static FilterGroup* create();
    static FilterGroup* create(std::vector<Filter*> filters);

    bool init();
    bool init(std::vector<Filter*> filters);
    bool hasFilter(const Filter* filter) const ;
    void addFilter(Filter* filter);
    void removeFilter(Filter* filter);
    void removeAllFilters();

    void setTerminalFilter(Filter* filter) {
        _terminalFilter = filter;
    }

    virtual Source* addTarget(Target* target) override ;

    virtual void removeTarget(Target* target) override ;
    virtual void removeAllTargets() override ;
    virtual bool hasTarget(const Target* target) const override ;
    virtual std::map<Target*, int>& getTargets() override ;
    virtual bool proceed(bool bUpdateTargets = true) override ;
    virtual void update(float frameTime) override ;
    virtual void updateTargets(float frameTime) override;
    virtual void setFramebuffer(Framebuffer* fb,
            RotationMode outputRotation = NoRotation) override ;
    virtual Framebuffer* getFramebuffer() const override ;
    virtual void setInputFramebuffer(Framebuffer* framebuffer,
            RotationMode rotationMode = NoRotation, int textId = 0) override ;
    virtual bool isPrepared() const override ;
    virtual void unPrepare() override ;

protected:
    std::vector<Filter*> _filters;
    Filter* _terminalFilter;

    FilterGroup();
    static Filter* _predictTerminalFilter(Filter* filter);
};


NS_GI_END