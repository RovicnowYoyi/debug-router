// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#ifndef DEBUGROUTER_HARMONY_DEBUGROUTER_SRC_MAIN_CPP_NATIVE_SLOT_HARMONY_H_
#define DEBUGROUTER_HARMONY_DEBUGROUTER_SRC_MAIN_CPP_NATIVE_SLOT_HARMONY_H_

#include <node_api.h>

#include "debug_router/native/core/native_slot.h"

namespace debugrouter {
namespace harmony {

class NativeSlotHarmony : public debugrouter::core::NativeSlot {

  public:
  NativeSlotHarmony(napi_env env, napi_value js_object);
  virtual ~NativeSlotHarmony() {
    napi_delete_reference(env_, js_this_ref_);
  };

  void OnMessage(const std::string& message, const std::string& type) override;

 private:
  static napi_value Constructor(napi_env env, napi_callback_info info);
  static std::string GetTypeFromJS(napi_env env, napi_value js_this);
  static std::string GetUrlFromJS(napi_env env, napi_value js_this);

  napi_env env_;
  napi_ref js_this_ref_;
};

}  // namespace harmony
}  // namespace debugrouter

#endif  // DEBUGROUTER_HARMONY_DEBUGROUTER_SRC_MAIN_CPP_NATIVE_SLOT_HARMONY_H_
