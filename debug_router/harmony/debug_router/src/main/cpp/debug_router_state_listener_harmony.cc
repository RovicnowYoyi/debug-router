// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include "debug_router/harmony/debug_router/src/main/cpp/debug_router_state_listener_harmony.h"

namespace debugrouter {
namespace harmony {

DebugRouterStateListenerHarmony::DebugRouterStateListenerHarmony(napi_env env, napi_value js_this)
  : env_(env), js_this_ref_(nullptr) {
  napi_create_reference(env, js_this, 1, &js_this_ref_);
}

void DebugRouterStateListenerHarmony::OnOpen(debugrouter::core::ConnectionType type) {
  napi_value js_this;
  napi_get_reference_value(env_, js_this_ref_, &js_this);
  napi_value onOpen;
  auto status = napi_get_named_property(env_, js_this, "onOpen", &onOpen);

  napi_value args[1];
  napi_create_string_utf8(env_, debugrouter::core::ConnectionTypes[type].c_str(), NAPI_AUTO_LENGTH, &args[0]);

  napi_value result;
  status = napi_call_function(env_, js_this, onOpen, 1, args, &result);
}

void DebugRouterStateListenerHarmony::OnClose(int32_t code, const std::string &reason) {
  napi_value js_this;
  napi_get_reference_value(env_, js_this_ref_, &js_this);
  napi_value onClose;
  auto status = napi_get_named_property(env_, js_this, "onClose", &onClose);

  napi_value args[2];
  napi_create_int32(env_, code, &args[0]);
  napi_create_string_utf8(env_, reason.c_str(), NAPI_AUTO_LENGTH, &args[1]);

  napi_value result;
  status = napi_call_function(env_, js_this, onClose, 2, args, &result);
}

void DebugRouterStateListenerHarmony::OnMessage(const std::string &message) {
  napi_value js_this;
  napi_get_reference_value(env_, js_this_ref_, &js_this);
  napi_value onMessage;
  auto status = napi_get_named_property(env_, js_this, "onMessage", &onMessage);

  napi_value args[1];
  napi_create_string_utf8(env_, message.c_str(), NAPI_AUTO_LENGTH, &args[0]);

  napi_value result;
  status = napi_call_function(env_, js_this, onMessage, 1, args, &result);
}

void DebugRouterStateListenerHarmony::OnError(const std::string &error) {
  napi_value js_this;
  napi_get_reference_value(env_, js_this_ref_, &js_this);
  napi_value onError;
  auto status = napi_get_named_property(env_, js_this, "onError", &onError);

  napi_value args[1];
  napi_create_string_utf8(env_, error.c_str(), NAPI_AUTO_LENGTH, &args[0]);

  napi_value result;
  status = napi_call_function(env_, js_this, onError, 1, args, &result);
}

}  // namespace harmony
}  // namespace debugrouter
