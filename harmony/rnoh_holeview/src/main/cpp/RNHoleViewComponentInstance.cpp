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

#include "RNHoleViewComponentInstance.h"

namespace rnoh {


RNHoleViewComponentInstance::RNHoleViewComponentInstance(Context ctx) : BaseRNHoleViewComponentInstance(ctx) {
    // 设置动画执行完成回调
    m_drawNode.setFinishCb([this]() { m_eventEmitter->onAnimationFinished({}); });
}

void RNHoleViewComponentInstance::handleCommand(const std::string &commandName, const folly::dynamic &args) {}

void RNHoleViewComponentInstance::onChildInserted(ComponentInstance::Shared const &childComponentInstance,
                                                  std::size_t index) {
    DLOG(INFO) << "child insert";
    CppComponentInstance::onChildInserted(childComponentInstance, index);
    m_drawNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index + 1);
}

void RNHoleViewComponentInstance::onPropsChanged(const SharedConcreteProps &props) {

    m_drawNode.setBrushColor(props->backgroundColor);

    // 如果有 oldHoles 清空，如果有parsedHole, 拷贝到oldHole
    m_drawNode.parsedOldHoles.clear();
    m_drawNode.parsedHoles.clear();

    if (props->holes.size()) {
        for (auto hole : props->holes) {
            DLOG(INFO) << "=== porps hole y: " << hole.y;
            m_drawNode.parsedHoles.emplace_back(std::make_shared<ParsedHole>(hole));
        }
    }

    if (m_props != nullptr && m_props->holes.size()) {
        for (auto h : m_props->holes) {
            m_drawNode.parsedOldHoles.emplace_back(std::make_shared<ParsedHole>(h));
        }
    }

    DLOG(INFO) << "duration: " << props->animation.duration << " timingFunction: " << props->animation.timingFunction;

    bool shouldAnimate = false;
    int newHolesSize = m_drawNode.parsedHoles.size();
    int oldHolesSize = m_drawNode.parsedOldHoles.size();
    if (newHolesSize && oldHolesSize) {
        for (int i = 0; i < newHolesSize; i++) {
            if (i < oldHolesSize && m_drawNode.parsedHoles.at(i) != m_drawNode.parsedOldHoles.at(i)) {
                shouldAnimate = true;
            }
        }
    }
    bool hasDurationTime = props->animation.duration && props->animation.duration > 0;
    bool hasTimingFunction = props->animation.timingFunction.length() > 0;
    if (hasDurationTime && hasTimingFunction && shouldAnimate) {
        m_drawNode.setAnimation(props->animation);
        m_drawNode.animatePlay();
    } else {
        m_drawNode.buildPath();
        m_drawNode.markDirty();
    }

}

ArkUINode &RNHoleViewComponentInstance::getLocalRootArkUINode() { return m_drawNode; }



} // namespace rnoh
