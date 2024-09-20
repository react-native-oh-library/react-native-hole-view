/**
 * MIT License
 *
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "RNOH/generated/BaseReactNativeOhTplReactNativeHoleViewPackage.h"
#include "RNHoleViewComponentInstance.h"

namespace rnoh {


class HoleViewComponentInstanceFactoryDelegate : public ComponentInstanceFactoryDelegate {
public:
    ComponentInstance::Shared create(ComponentInstance::Context ctx) override {
        if (ctx.componentName == "RNHoleView") {
            return std::make_shared<RNHoleViewComponentInstance>(ctx);
        }
        return nullptr;
    }
};

class RNOHHoleViewPackage : public BaseReactNativeOhTplReactNativeHoleViewPackage {
public:
    RNOHHoleViewPackage(Package::Context ctx) : BaseReactNativeOhTplReactNativeHoleViewPackage(ctx){};


    ComponentInstanceFactoryDelegate::Shared createComponentInstanceFactoryDelegate() override {
        return std::make_shared<HoleViewComponentInstanceFactoryDelegate>();
    }

};

} // namespace rnoh
