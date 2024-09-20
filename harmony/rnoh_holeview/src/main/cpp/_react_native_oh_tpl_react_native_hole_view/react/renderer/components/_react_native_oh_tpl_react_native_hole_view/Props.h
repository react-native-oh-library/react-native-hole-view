
/**
 * This code was generated by [react-native-codegen](https://www.npmjs.com/package/react-native-codegen).
 *
 * Do not edit this file as changes may cause incorrect behavior and will be lost
 * once the code is regenerated.
 *
 * @generated by codegen project: GeneratePropsH.js
 */
#pragma once

#include <react/renderer/components/view/ViewProps.h>
#include <react/renderer/core/PropsParserContext.h>
#include <react/renderer/core/propsConversions.h>
#include <vector>

namespace facebook {
namespace react {

struct RNHoleViewHolesStruct {
  int height;
  int width;
  int x;
  int y;
  int borderRadius;
  bool isRTL;
  int borderTopLeftRadius;
  int borderTopRightRadius;
  int borderBottomLeftRadius;
  int borderBottomRightRadius;
  int borderTopStartRadius;
  int borderTopEndRadius;
  int borderBottomStartRadius;
  int borderBottomEndRadius;
};

static inline void fromRawValue(const PropsParserContext& context, const RawValue &value, RNHoleViewHolesStruct &result) {
//   auto map = (std::unordered_map<std::string, RawValue>)value;
  auto map = (butter::map<std::string, RawValue>)value;

  auto tmp_height = map.find("height");
  if (tmp_height != map.end()) {
    fromRawValue(context, tmp_height->second, result.height);
  }
  auto tmp_width = map.find("width");
  if (tmp_width != map.end()) {
    fromRawValue(context, tmp_width->second, result.width);
  }
  auto tmp_x = map.find("x");
  if (tmp_x != map.end()) {
    fromRawValue(context, tmp_x->second, result.x);
  }
  auto tmp_y = map.find("y");
  if (tmp_y != map.end()) {
    fromRawValue(context, tmp_y->second, result.y);
  }
  auto tmp_borderRadius = map.find("borderRadius");
  if (tmp_borderRadius != map.end()) {
    fromRawValue(context, tmp_borderRadius->second, result.borderRadius);
  }
  auto tmp_isRTL = map.find("isRTL");
  if (tmp_isRTL != map.end()) {
    fromRawValue(context, tmp_isRTL->second, result.isRTL);
  }
  auto tmp_borderTopLeftRadius = map.find("borderTopLeftRadius");
  if (tmp_borderTopLeftRadius != map.end()) {
    fromRawValue(context, tmp_borderTopLeftRadius->second, result.borderTopLeftRadius);
  }
  auto tmp_borderTopRightRadius = map.find("borderTopRightRadius");
  if (tmp_borderTopRightRadius != map.end()) {
    fromRawValue(context, tmp_borderTopRightRadius->second, result.borderTopRightRadius);
  }
  auto tmp_borderBottomLeftRadius = map.find("borderBottomLeftRadius");
  if (tmp_borderBottomLeftRadius != map.end()) {
    fromRawValue(context, tmp_borderBottomLeftRadius->second, result.borderBottomLeftRadius);
  }
  auto tmp_borderBottomRightRadius = map.find("borderBottomRightRadius");
  if (tmp_borderBottomRightRadius != map.end()) {
    fromRawValue(context, tmp_borderBottomRightRadius->second, result.borderBottomRightRadius);
  }
  auto tmp_borderTopStartRadius = map.find("borderTopStartRadius");
  if (tmp_borderTopStartRadius != map.end()) {
    fromRawValue(context, tmp_borderTopStartRadius->second, result.borderTopStartRadius);
  }
  auto tmp_borderTopEndRadius = map.find("borderTopEndRadius");
  if (tmp_borderTopEndRadius != map.end()) {
    fromRawValue(context, tmp_borderTopEndRadius->second, result.borderTopEndRadius);
  }
  auto tmp_borderBottomStartRadius = map.find("borderBottomStartRadius");
  if (tmp_borderBottomStartRadius != map.end()) {
    fromRawValue(context, tmp_borderBottomStartRadius->second, result.borderBottomStartRadius);
  }
  auto tmp_borderBottomEndRadius = map.find("borderBottomEndRadius");
  if (tmp_borderBottomEndRadius != map.end()) {
    fromRawValue(context, tmp_borderBottomEndRadius->second, result.borderBottomEndRadius);
  }
}

static inline std::string toString(const RNHoleViewHolesStruct &value) {
  return "[Object RNHoleViewHolesStruct]";
}

static inline void fromRawValue(const PropsParserContext& context, const RawValue &value, std::vector<RNHoleViewHolesStruct> &result) {
  auto items = (std::vector<RawValue>)value;
  for (const auto &item : items) {
    RNHoleViewHolesStruct newItem;
    fromRawValue(context, item, newItem);
    result.emplace_back(newItem);
  }
}


struct RNHoleViewAnimationStruct {
  int duration;
  std::string timingFunction;
};

static inline void fromRawValue(const PropsParserContext& context, const RawValue &value, RNHoleViewAnimationStruct &result) {
//   auto map = (std::unordered_map<std::string, RawValue>)value;
  auto map = (butter::map<std::string, RawValue>)value;

  auto tmp_duration = map.find("duration");
  if (tmp_duration != map.end()) {
    fromRawValue(context, tmp_duration->second, result.duration);
  }
  auto tmp_timingFunction = map.find("timingFunction");
  if (tmp_timingFunction != map.end()) {
    fromRawValue(context, tmp_timingFunction->second, result.timingFunction);
  }
}

static inline std::string toString(const RNHoleViewAnimationStruct &value) {
  return "[Object RNHoleViewAnimationStruct]";
}
class RNHoleViewProps final : public ViewProps {
 public:
  RNHoleViewProps() = default;
  RNHoleViewProps(const PropsParserContext& context, const RNHoleViewProps &sourceProps, const RawProps &rawProps);

#pragma mark - Props

  std::vector<RNHoleViewHolesStruct> holes{};
  RNHoleViewAnimationStruct animation{};
};

} // namespace react
} // namespace facebook
