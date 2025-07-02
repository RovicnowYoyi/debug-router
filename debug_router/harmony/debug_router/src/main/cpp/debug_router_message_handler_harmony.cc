// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include "debug_router/harmony/debug_router/src/main/cpp/debug_router_message_handler_harmony.h"

namespace debugrouter {
namespace harmony {

DebugRouterMessageHandlerHarmony::DebugRouterMessageHandlerHarmony(napi_env env, napi_value js_this)
  : env_(env), js_this_ref_(nullptr) {
napi_create_reference(env, js_this, 1, &js_this_ref_);
}

std::string DebugRouterMessageHandlerHarmony::Handle(std::string params) {
  napi_value js_this;
  napi_get_reference_value(env_, js_this_ref_, &js_this);
  napi_value handle;
  auto status = napi_get_named_property(env_, js_this, "handle", &handle);

  napi_value args[1];
  napi_create_string_utf8(env_, params.c_str(), NAPI_AUTO_LENGTH, &args[0]);

  napi_value result;
  status = napi_call_function(env_, js_this, handle, 1, args, &result);

  size_t strSize;
  status = napi_get_value_string_utf8(env_, result, nullptr, 0, &strSize);
  char* buffer = new char[strSize + 1];
  status = napi_get_value_string_utf8(env_, result, buffer, strSize + 1, &strSize);
  std::string str(buffer);
  delete[] buffer;
  return str;
}

std::string DebugRouterMessageHandlerHarmony::GetName() const {
  napi_value js_this;
  napi_get_reference_value(env_, js_this_ref_, &js_this);
  napi_value getName;
  auto status = napi_get_named_property(env_, js_this, "getName", &getName);

  napi_value result;
  status = napi_call_function(env_, js_this, getName, 0, nullptr, &result);

  size_t strSize;
  status = napi_get_value_string_utf8(env_, result, nullptr, 0, &strSize);
  char* buffer = new char[strSize + 1];
  status = napi_get_value_string_utf8(env_, result, buffer, strSize + 1, &strSize);
  std::string str(buffer);
  delete[] buffer;
  return str;
}

}  // namespace harmony
}  // namespace debugrouter
