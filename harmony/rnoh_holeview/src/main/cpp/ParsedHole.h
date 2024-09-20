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

#ifndef HARMONY_PATHSTRINGBUILDER_H
#define HARMONY_PATHSTRINGBUILDER_H


#include <string>
#include "react/renderer/components/_react_native_oh_tpl_react_native_hole_view/Props.h"
#include "util.h"
#include <react/renderer/graphics/Point.h>

namespace rnoh {
using namespace std;
using namespace facebook::react;

static const float DEFAULT_SPECIFIC_BORDER_RADIUS_VALUE = -1.f;
static const float DEFAULT_BORDER_RADIUS_VALUE = 0.f;

class ParsedHole {
public:
    ~ParsedHole() {
        delete _hole;
        _hole = nullptr;
    }

    ParsedHole(RNHoleViewHolesStruct const *hole) {
        _hole = new RNHoleViewHolesStruct();
        _hole->x = hole->x;
        _hole->y = hole->y;
        _hole->width = hole->width;
        _hole->height = hole->height;
        _hole->borderTopLeftRadius = hole->borderTopLeftRadius;
        _hole->borderTopRightRadius = hole->borderTopRightRadius;
        _hole->borderBottomLeftRadius = hole->borderBottomLeftRadius;
        _hole->borderBottomRightRadius = hole->borderBottomRightRadius;
    }

    bool operator==(RNHoleViewHolesStruct const &hole) {
        return _hole->x == hole.x && _hole->y == hole.y && _hole->width == hole.width && _hole->height == hole.height &&
               _hole->borderTopLeftRadius == hole.borderTopLeftRadius &&
               _hole->borderTopRightRadius == hole.borderTopRightRadius &&
               _hole->borderBottomLeftRadius == hole.borderBottomLeftRadius &&
               _hole->borderBottomRightRadius == hole.borderBottomRightRadius;
    }

    ParsedHole(RNHoleViewHolesStruct const &hole) {
        _hole = new RNHoleViewHolesStruct();
        _hole->x = hole.x;
        _hole->y = hole.y;
        _hole->width = hole.width;
        _hole->height = hole.height;
        auto borderRadius = hole.borderRadius ? hole.borderRadius : DEFAULT_BORDER_RADIUS_VALUE;

        auto borderTopLeftRadius =
            hole.borderTopLeftRadius == DEFAULT_SPECIFIC_BORDER_RADIUS_VALUE ? borderRadius : hole.borderTopLeftRadius;
        auto borderTopRightRadius = hole.borderTopRightRadius == DEFAULT_SPECIFIC_BORDER_RADIUS_VALUE
                                        ? borderRadius
                                        : hole.borderTopRightRadius;
        auto borderBottomLeftRadius = hole.borderBottomLeftRadius == DEFAULT_SPECIFIC_BORDER_RADIUS_VALUE
                                          ? borderRadius
                                          : hole.borderBottomLeftRadius;
        auto borderBottomRightRadius = hole.borderBottomRightRadius == DEFAULT_SPECIFIC_BORDER_RADIUS_VALUE
                                           ? borderRadius
                                           : hole.borderBottomRightRadius;
        bool isRTL = hole.isRTL;
        if (hole.borderTopStartRadius != DEFAULT_SPECIFIC_BORDER_RADIUS_VALUE) {
            isRTL ? borderTopRightRadius = hole.borderTopStartRadius : borderTopLeftRadius = hole.borderTopStartRadius;
        }
        if (hole.borderTopEndRadius != DEFAULT_SPECIFIC_BORDER_RADIUS_VALUE) {
            isRTL ? borderTopLeftRadius = hole.borderTopEndRadius : borderTopRightRadius = hole.borderTopEndRadius;
        }
        if (hole.borderBottomStartRadius != DEFAULT_SPECIFIC_BORDER_RADIUS_VALUE) {
            isRTL ? borderBottomRightRadius = hole.borderBottomStartRadius
                  : borderBottomLeftRadius = hole.borderBottomStartRadius;
        }
        if (hole.borderBottomEndRadius != DEFAULT_SPECIFIC_BORDER_RADIUS_VALUE) {
            isRTL ? borderBottomLeftRadius = hole.borderBottomEndRadius
                  : borderBottomRightRadius = hole.borderBottomEndRadius;
        }
        _hole->borderTopLeftRadius = borderTopLeftRadius;
        _hole->borderTopRightRadius = borderTopRightRadius;
        _hole->borderBottomLeftRadius = borderBottomLeftRadius;
        _hole->borderBottomRightRadius = borderBottomRightRadius;
    }
    RNHoleViewHolesStruct *getHole() { return _hole; }

private:
    facebook::react::RNHoleViewHolesStruct *_hole;
};

class PathStringBuilder {
public:
    std::string &getPathString() { return _path; }

    PathStringBuilder &moveTo(float x, float y) {
        _path += "M" + to_px_string(x) + " " + to_px_string(y) + " ";
        return *this;
    }

    PathStringBuilder &lineTo(float x, float y) {
        _path += "L" + to_px_string(x) + " " + to_px_string(y) + " ";
        return *this;
    }

    PathStringBuilder &close() {
        _path += " Z";
        return *this;
    }

    PathStringBuilder &quadTo(float ctrlX, float ctrlY, float endX, float endY) {
        _path += "Q" + to_px_string(ctrlX) + " " + to_px_string(ctrlY) + " " + to_px_string(endX) + " " +
                 to_px_string(endY) + " ";
        return *this;
    }

    PathStringBuilder &cubicTo(float ctrlX1, float ctrlY1, float ctrlX2, float ctrlY2, float endX, float endY) {
        _path += "C" + to_px_string(ctrlX1) + " " + to_px_string(ctrlY1) + " " + to_px_string(ctrlX2) + " " +
                 to_px_string(ctrlY2) + " " + to_px_string(endX) + " " + to_px_string(endY) + " ";
        return *this;
    }
    PathStringBuilder &arcTo(float rx, float ry, float x_axis_rotate, int arc_flag, int sweep_flag, float x, float y) {
        _path += "A" + to_px_string(rx) + " " + to_px_string(ry) + " " + to_string(x_axis_rotate) + " " +
                 to_string(arc_flag) + " " + to_string(sweep_flag) + " " + to_px_string(x) + " " + to_px_string(y) +
                 " ";
        return *this;
    }

    PathStringBuilder &radiusRect(RNHoleViewHolesStruct const *hole) {
        Point p1{static_cast<Float>(hole->x), static_cast<Float>(hole->y + hole->borderTopLeftRadius)};
        Point p2{static_cast<Float>(hole->x + hole->borderTopLeftRadius), static_cast<Float>(hole->y)};
        Point p3{static_cast<Float>(hole->x + hole->width - hole->borderTopRightRadius), static_cast<Float>(hole->y)};
        Point p4{static_cast<Float>(hole->x + hole->width), static_cast<Float>(hole->y + hole->borderTopRightRadius)};
        Point p5{static_cast<Float>(hole->x + hole->width),
                 static_cast<Float>(hole->y + hole->height - hole->borderBottomRightRadius)};
        Point p6{static_cast<Float>(hole->x + hole->width - hole->borderBottomRightRadius),
                 static_cast<Float>(hole->y + hole->height)};
        Point p7{static_cast<Float>(hole->x + hole->borderBottomLeftRadius),
                 static_cast<Float>(hole->y + hole->height)};
        Point p8{static_cast<Float>(hole->x),
                 static_cast<Float>(hole->y + hole->height - hole->borderBottomLeftRadius)};
        moveTo(p1.x, p1.y)
            .arcTo(hole->borderTopLeftRadius, hole->borderTopLeftRadius, 0, 0, 1, p2.x, p2.y)
            .lineTo(p3.x, p3.y)
            .arcTo(hole->borderTopRightRadius, hole->borderTopRightRadius, 0, 0, 1, p4.x, p4.y)
            .lineTo(p5.x, p5.y)
            .arcTo(hole->borderBottomRightRadius, hole->borderBottomRightRadius, 0, 0, 1, p6.x, p6.y)
            .lineTo(p7.x, p7.y)
            .arcTo(hole->borderBottomLeftRadius, hole->borderBottomLeftRadius, 0, 0, 1, p8.x, p8.y)
            .lineTo(p8.x, p8.y);
        return *this;
    }

    bool toPx = true;

private:
    std::string to_px_string(float val) { return to_string(toPx ? vpToPx(val) : val); }
    std::string _path = "";
};
} // namespace rnoh


#endif // HARMONY_PATHSTRINGBUILDER_H
