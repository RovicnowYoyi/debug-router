// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include "debug_router/harmony/debug_router/src/main/cpp/native_slot_harmony.h"

namespace debugrouter {
namespace harmony {

std::string NativeSlotHarmony::GetTypeFromJS(napi_env env, napi_value js_this) {
  napi_value type_value;
  napi_get_named_property(env, js_this, "type", &type_value);
  size_t length;
  napi_get_value_string_utf8(env, type_value, nullptr, 0, &length);
  std::string type(length, '\0');
  napi_get_value_string_utf8(env, type_value, &type[0], length + 1, nullptr);
  return type;
}

std::string NativeSlotHarmony::GetUrlFromJS(napi_env env, napi_value js_this) {
  napi_value url_value;
  napi_get_named_property(env, js_this, "url", &url_value);
  size_t length;
  napi_get_value_string_utf8(env, url_value, nullptr, 0, &length);
  std::string url(length, '\0');
  napi_get_value_string_utf8(env, url_value, &url[0], length + 1, nullptr);
  return url;
}

NativeSlotHarmony::NativeSlotHarmony(napi_env env, napi_value js_this)
  : NativeSlot(GetTypeFromJS(env, js_this), GetUrlFromJS(env, js_this)), env_(env), js_this_ref_(nullptr) {
  napi_create_reference(env, js_this, 1, &js_this_ref_);
}

void NativeSlotHarmony::OnMessage(const std::string &message,
                                      const std::string &type) {
  napi_value js_this;
  napi_get_reference_value(env_, js_this_ref_, &js_this);
  napi_value onMessage;
  auto status = napi_get_named_property(env_, js_this, "onMessage", &onMessage);

  napi_value args[2];
  napi_create_string_utf8(env_, message.c_str(), NAPI_AUTO_LENGTH, &args[0]);
  napi_create_string_utf8(env_, type.c_str(), NAPI_AUTO_LENGTH, &args[1]);

  napi_value result;
  status = napi_call_function(env_, js_this, onMessage, 2, args, &result);
}

}  // namespace harmony
}  // namespace debugrouter
