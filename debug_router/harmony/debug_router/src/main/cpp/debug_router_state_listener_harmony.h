// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#ifndef DEBUGROUTER_HARMONY_DEBUGROUTER_SRC_MAIN_CPP_DEBUG_ROUTER_STATE_LISTENER_HARMONY_H_
#define DEBUGROUTER_HARMONY_DEBUGROUTER_SRC_MAIN_CPP_DEBUG_ROUTER_STATE_LISTENER_HARMONY_H_

#include <node_api.h>

#include "debug_router/native/core/debug_router_state_listener.h"

namespace debugrouter {
namespace harmony {

class DebugRouterStateListenerHarmony : public debugrouter::core::DebugRouterStateListener {

  public:
  DebugRouterStateListenerHarmony(napi_env env, napi_value js_object);
  virtual ~DebugRouterStateListenerHarmony() {
    napi_delete_reference(env_, js_this_ref_);
  };

  void OnOpen(ConnectionType type) override;
  void OnClose(int32_t code, const std::string &reason) override;
  void OnMessage(const std::string &message) override;
  void OnError(const std::string &error) override;

 private:
  static napi_value Constructor(napi_env env, napi_callback_info info);

  napi_env env_;
  napi_ref js_this_ref_;
};

}  // namespace harmony
}  // namespace debugrouter

#endif  // DEBUGROUTER_HARMONY_DEBUGROUTER_SRC_MAIN_CPP_DEBUG_ROUTER_STATE_LISTENER_HARMONY_H_
