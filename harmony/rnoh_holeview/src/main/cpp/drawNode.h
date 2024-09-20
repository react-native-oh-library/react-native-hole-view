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
#include "RNOH/arkui/ArkUINode.h"
#include "RNOH/arkui/ArkUINodeRegistry.h"
#include "RNOH/arkui/NativeNodeApi.h"
#include "RNOH/arkui/CustomNode.h"
#include "RNOHCorePackage/ComponentInstances/ScrollViewComponentInstance.h"
#include "arkui/native_node.h"
#include "arkui/native_interface.h"
#include "arkui/native_type.h"
#include "RNOH/ComponentInstance.h"
#include <native_drawing/drawing_brush.h>
#include <native_drawing/drawing_color.h>
#include <native_drawing/drawing_path.h>
#include <native_drawing/drawing_pen.h>
#include <native_drawing/drawing_rect.h>
#include <native_drawing/drawing_region.h>
#include <native_drawing/drawing_types.h>
#include <native_drawing/drawing_color_space.h>
#include <native_drawing/drawing_round_rect.h>
#include <native_drawing/drawing_round_rect.h>
#include <native_drawing/drawing_canvas.h>
#include "NativeAnimateApi.h"
#include "ParsedHole.h"
#include "util.h"
#include "react/renderer/components/_react_native_oh_tpl_react_native_hole_view/Props.h"

namespace rnoh {

struct AnimateUserData {
    int32_t data;
    std::function<void(ArkUI_AnimatorOnFrameEvent *event)> updateCallback;
    std::function<void(ArkUI_AnimatorEvent *event)> finishCallback;
};

class DrawNode : public ArkUINode, TouchEventHandler {
private:
    void OnDraw(ArkUI_NodeCustomEvent *event);

    UserCallback *userCallback_ = nullptr;

    void (*eventReceiver)(ArkUI_NodeCustomEvent *);
    ArkUI_ContextHandle context = nullptr;
    ArkUI_AnimatorHandle animatorHandle = nullptr;
    
    ArkUI_AnimatorOption *animateOpt = nullptr;
    AnimateUserData *userAniUserData = nullptr;
    ArkUI_CurveHandle curve = nullptr;
    OH_Drawing_Rect *holeRect = OH_Drawing_RectCreate(0, 0, 0, 0);
    OH_Drawing_RoundRect *holeRound = OH_Drawing_RoundRectCreate(holeRect, 0, 0);
    bool isPlaying = false;

    OH_Drawing_Point2D radiiTopLeft{};
    OH_Drawing_Point2D radiiTopRight{};
    OH_Drawing_Point2D radiiBottomLeft{};
    OH_Drawing_Point2D radiiBottomRight{};

    std::function<void()> finishCb = NULL;
    OH_Drawing_Region * m_region = OH_Drawing_RegionCreate();

    
    bool downEventInHole;

public:
    DrawNode();
    ~DrawNode() override;
    OH_Drawing_Path *mPath = OH_Drawing_PathCreate();
    OH_Drawing_Pen *m_Pen = OH_Drawing_PenCreate();
    OH_Drawing_Brush *m_Brush = OH_Drawing_BrushCreate();

    std::vector<std::shared_ptr<ParsedHole>> parsedHoles{};
    std::vector<std::shared_ptr<ParsedHole>> parsedOldHoles{};

    void onTouchEvent(ArkUI_UIInputEvent *event) override;
    void insertChild(ArkUINode &child, std::size_t index);


    void addRoundRectToPath(float x, float y, float width, float height, float borderTopLeftRadius,
                            float borderTopRightRadius, float borderBottomLeftRadius, float borderBottomRightRadius);

    void buildPath() {
        if (!parsedHoles.size()) {
            return;
        }
        OH_Drawing_PathReset(mPath);
        OH_Drawing_PathSetFillType(mPath, PATH_FILL_TYPE_EVEN_ODD);
        float x, y, width, height;
        float borderTopLeftRadius;
        float borderTopRightRadius;
        float borderBottomLeftRadius;
        float borderBottomRightRadius;
        for (auto pHole : parsedHoles) {
            auto hole = pHole->getHole();
            x = vpToPx(hole->x);
            y = vpToPx(hole->y);
            width = vpToPx(hole->width);
            height = vpToPx(hole->height);
            borderTopLeftRadius = static_cast<float>(vpToPx(hole->borderTopLeftRadius));
            borderTopRightRadius = static_cast<float>(vpToPx(hole->borderTopRightRadius));
            borderBottomLeftRadius = static_cast<float>(vpToPx(hole->borderBottomLeftRadius));
            borderBottomRightRadius = static_cast<float>(vpToPx(hole->borderBottomRightRadius));
            addRoundRectToPath(x, y, width, height, borderTopLeftRadius, borderTopRightRadius, borderBottomLeftRadius,
                               borderBottomRightRadius);
        }
    }

    void setFinishCb(std::function<void()> cb) {
        finishCb = cb;
    }

    ArkUI_ExpectedFrameRateRange frameRateRange = {.min = 60, .max = 120, .expected = 120};

    void setAnimation(const facebook::react::RNHoleViewAnimationStruct &animation) {
        if (animateOpt != nullptr) {
            OH_ArkUI_AnimatorOption_Dispose(animateOpt);
        }
        animateOpt = OH_ArkUI_AnimatorOption_Create(0);
        if (animation.duration && animation.duration > 0) {
            OH_ArkUI_AnimatorOption_SetDuration(animateOpt, animation.duration);
            OH_ArkUI_AnimatorOption_SetExpectedFrameRateRange(animateOpt, &frameRateRange);
        }
        if (animation.timingFunction.length()) {
            if (curve != nullptr) {
                OH_ArkUI_Curve_DisposeCurve(curve);
            }
            ArkUI_AnimationCurve curveType = ARKUI_CURVE_LINEAR;
            if (animation.timingFunction == "EASE_IN") {
                curveType = ARKUI_CURVE_EASE_IN;
            }
            if (animation.timingFunction == "EASE_OUT") {
                curveType = ARKUI_CURVE_EASE_OUT;
            }
            if (animation.timingFunction == "EASE_IN_OUT") {
                curveType = ARKUI_CURVE_EASE_IN_OUT;
            }
            curve = OH_ArkUI_Curve_CreateCurveByType(curveType);
            OH_ArkUI_AnimatorOption_SetCurve(animateOpt, curve);
        }
        OH_ArkUI_AnimatorOption_RegisterOnFrameCallback(
            animateOpt, userAniUserData, [](ArkUI_AnimatorOnFrameEvent *event) {
                auto *userData = reinterpret_cast<AnimateUserData *>(OH_ArkUI_AnimatorOnFrameEvent_GetUserData(event));
                if (userData != nullptr && userData->updateCallback) {
                    userData->updateCallback(event);
                }
            });
        OH_ArkUI_AnimatorOption_RegisterOnFinishCallback(animateOpt, userAniUserData, [](ArkUI_AnimatorEvent *event) {
            auto *userData = reinterpret_cast<AnimateUserData *>(OH_ArkUI_AnimatorEvent_GetUserData(event));
            if (userData != nullptr && userData->finishCallback) {
                userData->finishCallback(event);
            }
        });
        if (animatorHandle) {
            NativeAnimateApi::getInstance()->disposeAnimator(animatorHandle);
        }
        animatorHandle = NativeAnimateApi::getInstance()->createAnimator(context, animateOpt);
    }

    // 开始动画
    void animatePlay() {
        if (animatorHandle != nullptr) {
            DLOG(INFO) << "animate play!";
            isPlaying = true;
            OH_ArkUI_Animator_Play(animatorHandle);
        }
    }

    void setBrushColor(facebook::react::SharedColor color) {
        facebook::react::ColorComponents colorComp = colorComponentsFromColor(color);
        DLOG(INFO) << "argb: " << colorComp.alpha << " " << colorComp.red << " " << colorComp.green << " "
                   << colorComp.blue;
        OH_Drawing_BrushSetColor(m_Brush, OH_Drawing_ColorSetArgb(colorComp.alpha * 255, colorComp.red * 255,
                                                                  colorComp.green * 255, colorComp.blue * 255));
    }
};

} // namespace rnoh