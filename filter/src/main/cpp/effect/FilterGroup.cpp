/*
 * GPUImage-x
 *
 * Copyright (C) 2017 Yijin Wang, Yiqian Wang
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <assert.h>
#include "FilterGroup.hpp"
#include <algorithm>
#include "../source/Context.hpp"

NS_GI_BEGIN

REGISTER_FILTER_CLASS(FilterGroup)

FilterGroup::FilterGroup()
:_terminalFilter(0)
{
}

FilterGroup::~FilterGroup() {
    removeAllFilters();
    _terminalFilter = 0;
}

FilterGroup* FilterGroup::create() {
    FilterGroup* ret = new (std::nothrow) FilterGroup();
    if (ret && ret->init()) {
        //ret->autorelease();
    } else {
        delete ret;
        ret = 0;
    }
    return ret;
}

FilterGroup* FilterGroup::create(std::vector<Filter*> filters) {
    FilterGroup* ret = new (std::nothrow) FilterGroup();
    if (ret && ret->init(filters)) {
        //ret->autorelease();
    } else {
        delete ret;
        ret = 0;
    }
    return ret;
}

bool FilterGroup::init() {
    return true;
}


bool FilterGroup::init(std::vector<Filter*> filters) {
    if (0 == filters.size()) return true;
    _filters = filters;

    for (auto const& filter : filters ) {
        Ref* ref = dynamic_cast<Ref*>(filter);
        if (ref) {
            ref->retain();
        }
    }

    setTerminalFilter(_predictTerminalFilter(filters[filters.size() - 1]));
    return true;
}

bool FilterGroup::hasFilter(const Filter* filter) const {
    auto it = std::find(_filters.begin(), _filters.end(), filter);
    return it != _filters.end();
}

void FilterGroup::addFilter(Filter* filter) {
    if (hasFilter(filter)) return;

    _filters.push_back(filter);

    Ref* ref = dynamic_cast<Ref*>(filter);
    if (ref) {
        ref->retain();
    }

    setTerminalFilter(_predictTerminalFilter(filter));
}

void FilterGroup::removeFilter(Filter* filter) {
    auto itr = std::find(_filters.begin(), _filters.end(), filter);
    if (itr != _filters.end()) {
        Ref* ref = dynamic_cast<Ref*>(*itr);
        if (ref) {
            ref->release();
        }
        _filters.erase(itr);
    }
}

void FilterGroup::removeAllFilters() {
    for (auto const& filter : _filters ) {
        Ref* ref = dynamic_cast<Ref*>(filter);
        if (ref) {
            ref->release();
        }
    }
    _filters.clear();
}

Filter* FilterGroup::_predictTerminalFilter(Filter* filter) {
    if (filter->getTargets().size() == 0)
        return filter;
    else
        return _predictTerminalFilter(dynamic_cast<Filter*>(filter->getTargets().begin()->first));
}

Source* FilterGroup::addTarget(Target* target) {
    if (_terminalFilter)
        return _terminalFilter->addTarget(target);
    else
        return 0;
}

//#if GI_TARGET_PLATFORM == GI_PLATFORM_IOS
//Source* FilterGroup::addTarget(id<GPUImageTarget> target) {
//    if (_terminalFilter)
//        return _terminalFilter->addTarget(target);
//    else
//        return 0;
//}
//#endif

void FilterGroup::removeTarget(Target* target) {
    if (_terminalFilter)
        _terminalFilter->removeTarget(target);
}

void FilterGroup::removeAllTargets() {
    if (_terminalFilter)
        _terminalFilter->removeAllTargets();
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
    if (Context::getInstance()->isCapturingFrame && this == Context::getInstance()->captureUpToFilter) {
        Context::getInstance()->captureUpToFilter = _terminalFilter;
    }

    for(auto& filter : _filters){
        if (filter->isPrepared()) {
            filter->update(frameTime);
            filter->unPrepare();
        }
    }
}

void FilterGroup::updateTargets(float frameTime) {
    if (_terminalFilter)
        _terminalFilter->updateTargets(frameTime);
}

void FilterGroup::setFramebuffer(Framebuffer* fb, RotationMode outputRotation/* = RotationMode::NoRotation*/) {
    //if (_terminalFilter)
    //    _terminalFilter->setFramebuffer(fb);
}

Framebuffer* FilterGroup::getFramebuffer() const {
    //if (_terminalFilter)
    //    return _terminalFilter->getFramebuffer();
    return 0;
}

void FilterGroup::setInputFramebuffer(Framebuffer* framebuffer, RotationMode rotationMode/* = NoRotation*/, int texIdx/* = 0*/) {
    for (auto& filter : _filters) {
        filter->setInputFramebuffer(framebuffer, rotationMode, texIdx);
    }
}

bool FilterGroup::isPrepared() const {
//    for (auto& filter : _filters) {
//        if (!filter->isPrepared())
//            return false;
//    }
    return true;
}

void FilterGroup::unPrepare() {
    //for (auto& filter : _filters) {
    //    filter->unPrepeared();
    //}
}

NS_GI_END
