
/**
 * This code was generated by [react-native-codegen](https://www.npmjs.com/package/react-native-codegen).
 *
 * Do not edit this file as changes may cause incorrect behavior and will be lost
 * once the code is regenerated.
 *
 * @generated by codegen project: GenerateEventEmitterCpp.js
 */

#include <react/renderer/components/_react_native_oh_tpl_react_native_hole_view/EventEmitters.h>


namespace facebook {
namespace react {

void RNHoleViewEventEmitter::onAnimationFinished(OnAnimationFinished $event) const {
  dispatchEvent("animationFinished", [](jsi::Runtime &runtime) {
    auto $payload = jsi::Object(runtime);
    
    return $payload;
  });
}

} // namespace react
} // namespace facebook
