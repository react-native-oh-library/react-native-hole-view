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
#include "RNOH/generated/components/BaseRNHoleViewComponentInstance.h"
#include "RNOH/arkui/CustomNode.h"
#include "RNOH/arkui/StackNode.h"
#include "RNOH/arkui/TextNode.h"
#include "drawNode.h"
#include "ParsedHole.h"
#include "react/renderer/components/_react_native_oh_tpl_react_native_hole_view/Props.h"

#include <react/renderer/graphics/Color.h>
#include "NativeAnimateApi.h"
#include <arkui/native_animate.h>


namespace rnoh {

class RNHoleViewComponentInstance : public BaseRNHoleViewComponentInstance, public CustomNodeDelegate {

public:
    RNHoleViewComponentInstance(Context ctx);

    void handleCommand(const std::string &commandName, const folly::dynamic &args) override;
    
    void onPropsChanged(SharedConcreteProps const &props) override;
    
    void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;
    

    ArkUINode &getLocalRootArkUINode() override;

private:
    DrawNode m_drawNode{};

};


} // namespace rnoh
