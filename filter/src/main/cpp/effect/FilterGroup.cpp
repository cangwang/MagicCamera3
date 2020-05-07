#include <cassert>
#include "FilterGroup.hpp"
#include <algorithm>
#include "../source/Context.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(FilterGroup)

FilterGroup::FilterGroup()
:_terminalFilter(nullptr){

}

FilterGroup::~FilterGroup() {
    removeAllTargets();
    _terminalFilter = nullptr;
}

FilterGroup* FilterGroup::create() {
    auto ret = new (std::nothrow) FilterGroup();
    if (ret && ret->init()) {

    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

FilterGroup * FilterGroup::create(std::vector<GPUImage::Filter *> filters) {
    auto ret = new (std::nothrow) FilterGroup();
    if (ret && ret->init(filters)) {

    } else {
        delete ret;
        ret = nullptr;
    }
    return ret;
}

bool FilterGroup::init() {
    return true;
}

bool FilterGroup::init(std::vector<GPUImage::Filter *> filters) {
    if (filters.empty()) return false;
    _filters = filters;

    for (auto const& filter:filters) {
        Ref* ref = dynamic_cast<Ref*>(filter);
        if (ref) {
            ref->retain();
        }
    }

    setTerminalFilter(_predictTerminalFilter(filters[filters.size() - 1]));
    return true;
}

bool FilterGroup::hasFilter(const GPUImage::Filter *filter) const {
    auto it = std::find(_filters.begin(), _filters.end(),filter);
    return it != _filters.end();
}

void FilterGroup::addFilter(GPUImage::Filter *filter) {
    if(hasFilter(filter)) return;

    _filters.push_back(filter);

    Ref* ref = dynamic_cast<Ref*>(filter);
    if (ref) {
        ref->retain();
    }

    setTerminalFilter(_predictTerminalFilter(filter));
}

void FilterGroup::removeFilter(GPUImage::Filter *filter) {
    auto it = std::find(_filters.begin(), _filters.end(), filter);
    if (it != _filters.end()) {
        Ref* ref = dynamic_cast<Ref*>(filter);
        if (ref) {
            ref->release();
        }
        _filters.erase(it);
    }
}

void FilterGroup::removeAllFilters() {
    for (auto const& filter : _filters) {
        Ref* ref = dynamic_cast<Ref*>(filter);
        if (ref) {
            ref->release();
        }
    }
    _filters.clear();
}

Filter* FilterGroup::_predictTerminalFilter(GPUImage::Filter *filter) {
    if(filter->getTargets().empty()) {
        return filter;
    } else {
        return _predictTerminalFilter((Filter *) filter->getTargets().begin()->first);
    }
}

Source* FilterGroup::addTarget(GPUImage::Target *target) {
    if (_terminalFilter) {
        return _terminalFilter->addTarget(target);
    } else
        return nullptr;
}

void FilterGroup::removeTarget(GPUImage::Target *target) {
    if(_terminalFilter) {
        _terminalFilter->removeTarget(target);
    }
}

void FilterGroup::removeAllTargets() {
    if(_terminalFilter) {
        _terminalFilter->removeAllTargets();
    }
}

bool FilterGroup::hasTarget(const Target* target) const {
        if (_terminalFilter)
            return _terminalFilter->hasTarget(target);
        else
            return false;
}

std::map<Target*, int>& FilterGroup::getTargets() {
        assert(_terminalFilter);
        return _terminalFilter->getTargets();
}

bool FilterGroup::proceed(bool bUpdateTargets/* = true*/) {
        return true;
}

void FilterGroup::update(float frameTime) {
    proceed();
    if(Context::getInstance()->isCapturingFrame &&
        this == Context::getInstance()->captureUpToFilter) {
        Context::getInstance()->captureUpToFilter = _terminalFilter;
    }

    for(auto& filter : _filters) {
        if(filter->isPrepared()) {
            filter->update(frameTime);
            filter->unPrepare();
        }
    }
}

void FilterGroup::updateTargets(float frameTime) {
    if(_terminalFilter) {
        _terminalFilter->updateTargets(frameTime);
    }
}

void FilterGroup::setFramebuffer(GPUImage::Framebuffer *fb,
                                 GPUImage::RotationMode outputRotation) {

}

Framebuffer* FilterGroup::getFramebuffer() const {
    return nullptr;
}

void FilterGroup::setInputFramebuffer(GPUImage::Framebuffer *framebuffer,
                                      GPUImage::RotationMode rotationMode, int textId) {
    for (auto& filter : _filters) {
        filter->setInputFramebuffer(framebuffer, rotationMode, textId);
    }
}

bool FilterGroup::isPrepared() const {
    return true;
}

void FilterGroup::unPrepare() {

}


NS_GI_END
