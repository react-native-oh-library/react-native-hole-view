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

#include "drawNode.h"
#include "util.h"


namespace rnoh {

DrawNode::DrawNode() : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_CUSTOM)) {
    userCallback_ = new UserCallback();
    userAniUserData = new AnimateUserData();
    // 设置自定义回调。注册onDraw
    userCallback_->callback = [this](ArkUI_NodeCustomEvent *event) {
        auto type = OH_ArkUI_NodeCustomEvent_GetEventType(event);
        switch (type) {
        case ARKUI_NODE_CUSTOM_EVENT_ON_DRAW:
            OnDraw(event);
            break;
        default:
            break;
        }
    };
    
    userAniUserData->updateCallback = [this](ArkUI_AnimatorOnFrameEvent *event) {
        float value = OH_ArkUI_AnimatorOnFrameEvent_GetValue(event);
        // 重置path
        OH_Drawing_PathReset(mPath);
        OH_Drawing_PathSetFillType(mPath, PATH_FILL_TYPE_EVEN_ODD);

        // path根据解析的hole添加
        if (parsedHoles.size()) {
            int index = 0;
            int oldHoleMaxIndex = parsedOldHoles.size();
            for (auto pHole : parsedHoles) {
                auto hole = pHole->getHole();
                float x, y, width, height;
                float borderTopLeftRadius;
                float borderTopRightRadius;
                float borderBottomLeftRadius;
                float borderBottomRightRadius;

                // 之前有老的hole, 需要动画过渡
                if (index < oldHoleMaxIndex) {
                    auto oldHole = parsedOldHoles.at(index)->getHole();
                    x = oldHole->x + (hole->x - oldHole->x) * value;
                    y = oldHole->y + (hole->y - oldHole->y) * value;
                    width = oldHole->width + (hole->width - oldHole->width) * value;
                    height = oldHole->height + (hole->height - oldHole->height) * value;
                    borderTopLeftRadius = oldHole->borderTopLeftRadius +
                                          (hole->borderTopLeftRadius - oldHole->borderTopLeftRadius) * value;
                    borderTopRightRadius = oldHole->borderTopRightRadius +
                                           (hole->borderTopRightRadius - oldHole->borderTopRightRadius) * value;
                    borderBottomLeftRadius = oldHole->borderBottomLeftRadius +
                                             (hole->borderBottomLeftRadius - oldHole->borderBottomLeftRadius) * value;
                    borderBottomRightRadius =
                        oldHole->borderBottomRightRadius +
                        (hole->borderBottomRightRadius - oldHole->borderBottomRightRadius) * value;
                    x = vpToPx(x);
                    y = vpToPx(y);
                    width = vpToPx(width);
                    height = vpToPx(height);
                    borderTopLeftRadius = vpToPx(borderTopLeftRadius);
                    borderTopRightRadius = vpToPx(borderTopRightRadius);
                    borderBottomLeftRadius = vpToPx(borderBottomLeftRadius);
                    borderBottomRightRadius = vpToPx(borderBottomRightRadius);
                } else {
                    x = vpToPx(hole->x);
                    y = vpToPx(hole->y);
                    width = vpToPx(hole->width);
                    height = vpToPx(hole->height);
                    borderTopLeftRadius = static_cast<float>(vpToPx(hole->borderTopLeftRadius));
                    borderTopRightRadius = static_cast<float>(vpToPx(hole->borderTopRightRadius));
                    borderBottomLeftRadius = static_cast<float>(vpToPx(hole->borderBottomLeftRadius));
                    borderBottomRightRadius = static_cast<float>(vpToPx(hole->borderBottomRightRadius));
                }
                addRoundRectToPath(x, y, width, height, borderTopLeftRadius, borderTopRightRadius,
                                   borderBottomLeftRadius, borderBottomRightRadius);

                index++;
            }
            markDirty();
        }
    };


    userAniUserData->finishCallback = [this](ArkUI_AnimatorEvent *event) {
        isPlaying = false;
        if (finishCb != NULL) {
            finishCb();
        }
    };

    eventReceiver = [](ArkUI_NodeCustomEvent *event) {
        if (OH_ArkUI_NodeCustomEvent_GetEventTargetId(event) == 77) {
            auto *userData = reinterpret_cast<UserCallback *>(OH_ArkUI_NodeCustomEvent_GetUserData(event));
            if (userData != nullptr && userData->callback != nullptr) {
                userData->callback(event);
            }
        }
    };

    maybeThrow(NativeNodeApi::getInstance()->addNodeCustomEventReceiver(m_nodeHandle, eventReceiver));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeCustomEvent(m_nodeHandle, ARKUI_NODE_CUSTOM_EVENT_ON_DRAW, 77,
                                                                     userCallback_));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_TOUCH_EVENT, 0, this));
    ArkUINodeRegistry::getInstance().registerTouchHandler(this, this);

    if (context == nullptr) {
        context = OH_ArkUI_GetContextByNode(getArkUINodeHandle());
    }


    // 设置触摸模式 transparent
    ArkUI_NumberValue hitTestModeValue[] = {{.i32 = static_cast<int32_t>(ArkuiHitTestMode::TRANSPARENT)}};
    ArkUI_AttributeItem hitTestModeItem = {.value = hitTestModeValue,
                                           .size = sizeof(hitTestModeValue) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_HIT_TEST_BEHAVIOR, &hitTestModeItem));

    OH_Drawing_PenSetWidth(m_Pen, 0);
    OH_Drawing_PenSetAlpha(m_Pen, 0);
    OH_Drawing_PenSetAntiAlias(m_Pen, true);
    OH_Drawing_BrushSetAntiAlias(m_Brush, true);
}

void DrawNode::addRoundRectToPath(float x, float y, float width, float height, float borderTopLeftRadius,
                                  float borderTopRightRadius, float borderBottomLeftRadius,
                                  float borderBottomRightRadius) {
    OH_Drawing_RectDestroy(holeRect);
    OH_Drawing_RoundRectDestroy(holeRound);
    holeRect = OH_Drawing_RectCreate(x, y, x + width, y + height);
    holeRound = OH_Drawing_RoundRectCreate(holeRect, 0, 0);

    radiiTopLeft.x = borderTopLeftRadius;
    radiiTopLeft.y = borderTopLeftRadius;
    radiiTopRight.x = borderTopRightRadius;
    radiiTopRight.y = borderTopRightRadius;
    radiiBottomLeft.x = borderBottomLeftRadius;
    radiiBottomLeft.y = borderBottomLeftRadius;
    radiiBottomRight.x = borderBottomRightRadius;
    radiiBottomRight.y = borderBottomRightRadius;

    OH_Drawing_RoundRectSetCorner(holeRound, CORNER_POS_TOP_LEFT, radiiTopLeft);
    OH_Drawing_RoundRectSetCorner(holeRound, CORNER_POS_TOP_RIGHT, radiiTopRight);
    OH_Drawing_RoundRectSetCorner(holeRound, CORNER_POS_BOTTOM_LEFT, radiiBottomLeft);
    OH_Drawing_RoundRectSetCorner(holeRound, CORNER_POS_BOTTOM_RIGHT, radiiBottomRight);
    OH_Drawing_PathAddRoundRect(mPath, holeRound, OH_Drawing_PathDirection::PATH_DIRECTION_CW);
}


DrawNode::~DrawNode() {
    NativeNodeApi::getInstance()->removeNodeCustomEventReceiver(m_nodeHandle, eventReceiver);
    NativeNodeApi::getInstance()->unregisterNodeCustomEvent(m_nodeHandle, ARKUI_NODE_CUSTOM_EVENT_ON_DRAW);
    ArkUINodeRegistry::getInstance().unregisterTouchHandler(this);

    // 释放callback资源
    delete userCallback_;
    userCallback_ = nullptr;

    delete userAniUserData;
    userAniUserData = nullptr;

    // 释放drawing资源
    OH_Drawing_PathDestroy(mPath);
    OH_Drawing_PenDestroy(m_Pen);
    OH_Drawing_RectDestroy(holeRect);
    OH_Drawing_RoundRectDestroy(holeRound);
    OH_Drawing_RegionDestroy(m_region);
    // 释放animator资源
    OH_ArkUI_AnimatorOption_Dispose(animateOpt);
    OH_ArkUI_Curve_DisposeCurve(curve);
}

void DrawNode::insertChild(ArkUINode &child, std::size_t index) {
    maybeThrow(NativeNodeApi::getInstance()->insertChildAt(m_nodeHandle, child.getArkUINodeHandle(),
                                                           static_cast<int32_t>(index)));
}


void DrawNode::onTouchEvent(ArkUI_UIInputEvent *event) {
    auto eventX = vpToPx(OH_ArkUI_PointerEvent_GetX(event));
    auto eventY = vpToPx(OH_ArkUI_PointerEvent_GetY(event));
    auto eventType = OH_ArkUI_UIInputEvent_GetAction(event);
    bool inHole = !OH_Drawing_PathContains(mPath, eventX, eventY);

    if (eventType == UI_TOUCH_EVENT_ACTION_DOWN) {
        OH_ArkUI_PointerEvent_SetStopPropagation(event, inHole ? false : true);
    } else if (eventType == UI_TOUCH_EVENT_ACTION_UP) {
        
    } else if (eventType == UI_TOUCH_EVENT_ACTION_MOVE) {
        
    }
    
}


void DrawNode::OnDraw(ArkUI_NodeCustomEvent *event) {

    // 从event 拿到canvas指针
    auto *drawContext = OH_ArkUI_NodeCustomEvent_GetDrawContextInDraw(event);
    auto *ctx = reinterpret_cast<OH_Drawing_Canvas *>(OH_ArkUI_DrawContext_GetCanvas(drawContext));

    OH_Drawing_CanvasAttachPen(ctx, m_Pen);
    OH_Drawing_CanvasAttachBrush(ctx, m_Brush);
    // 重置path
    auto canvas_height = OH_Drawing_CanvasGetHeight(ctx);
    auto canvas_width = OH_Drawing_CanvasGetWidth(ctx);

    // 绘制path最外层轮廓
    OH_Drawing_PathMoveTo(mPath, 0, 0);
    OH_Drawing_PathLineTo(mPath, canvas_width, 0);
    OH_Drawing_PathLineTo(mPath, canvas_width, canvas_height);
    OH_Drawing_PathLineTo(mPath, 0, canvas_height);
    OH_Drawing_PathClose(mPath);
    OH_Drawing_CanvasClipPath(ctx, mPath, INTERSECT, true);
    OH_Drawing_CanvasDrawPath(ctx, mPath);
}

// } // namespace sign
} // namespace rnoh
