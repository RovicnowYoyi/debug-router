// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#ifndef DEBUGROUTER_HARMONY_DEBUGROUTER_SRC_MAIN_CPP_DEBUG_ROUTER_MESSAGE_HANDLER_HARMONY_H_
#define DEBUGROUTER_HARMONY_DEBUGROUTER_SRC_MAIN_CPP_DEBUG_ROUTER_MESSAGE_HANDLER_HARMONY_H_

#include <node_api.h>

#include "debug_router/native/core/debug_router_message_handler.h"

namespace debugrouter {
namespace harmony {

class DebugRouterMessageHandlerHarmony : public debugrouter::core::DebugRouterMessageHandler {

  public:
  DebugRouterMessageHandlerHarmony(napi_env env, napi_value js_object);
  virtual ~DebugRouterMessageHandlerHarmony() {
    napi_delete_reference(env_, js_this_ref_);
  };

  std::string Handle(std::string params) override;
  std::string GetName() const override;

 private:
  static napi_value Constructor(napi_env env, napi_callback_info info);

  napi_env env_;
  napi_ref js_this_ref_;
};

}  // namespace harmony
}  // namespace debugrouter

#endif  // DEBUGROUTER_HARMONY_DEBUGROUTER_SRC_MAIN_CPP_DEBUG_ROUTER_MESSAGE_HANDLER_HARMONY_H_
