// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include "debug_router/harmony/debug_router/src/main/cpp/native_slot_harmony.h"
#include "debug_router/harmony/debug_router/src/main/base/fml/message_loop.h"
#include "debug_router/harmony/debug_router/src/main/base/napi_util.h"

namespace debugrouter {
namespace harmony {

std::string NativeSlotHarmony::GetTypeFromJS(napi_env env, napi_value js_this) {
  napi_value type_value;
  napi_get_named_property(env, js_this, "getType", &type_value);
  size_t length;
  napi_get_value_string_utf8(env, type_value, nullptr, 0, &length);
  std::string type(length, '\0');
  napi_get_value_string_utf8(env, type_value, &type[0], length + 1, nullptr);
  return type;
}

std::string NativeSlotHarmony::GetUrlFromJS(napi_env env, napi_value js_this) {
  napi_value url_value;
  napi_get_named_property(env, js_this, "getUrl", &url_value);
  size_t length;
  napi_get_value_string_utf8(env, url_value, nullptr, 0, &length);
  std::string url(length, '\0');
  napi_get_value_string_utf8(env, url_value, &url[0], length + 1, nullptr);
  return url;
}

NativeSlotHarmony::NativeSlotHarmony(napi_env env, napi_value js_this)
  : NativeSlot(GetTypeFromJS(env, js_this), GetUrlFromJS(env, js_this)), env_(env), js_this_ref_(nullptr) {
  napi_create_reference(env, js_this, 1, &js_this_ref_);
  napi_get_uv_event_loop(env, &loop_);
}

void NativeSlotHarmony::OnMessage(const std::string &message,
                                      const std::string &type) {
  auto ui_task_runner =
    fml::MessageLoop::EnsureInitializedForCurrentThread(loop_)
        .GetTaskRunner();
  ui_task_runner->PostTask([weak_ptr = weak_from_this(), message, type]() {
    napi_value js_this;
    auto handler = weak_ptr.lock();
    if (!handler) {
      return;
    }
    napi_get_reference_value(handler->env_, handler->js_this_ref_, &js_this);
    napi_value onMessage;
    auto status = napi_get_named_property(handler->env_, js_this, "onMessage", &onMessage);

    napi_value args[2];
    napi_create_string_utf8(handler->env_, message.c_str(), NAPI_AUTO_LENGTH, &args[0]);
    napi_create_string_utf8(handler->env_, type.c_str(), NAPI_AUTO_LENGTH, &args[1]);

    napi_value result;
    status = napi_call_function(handler->env_, js_this, onMessage, 2, args, &result);
  });
}

}  // namespace harmony
}  // namespace debugrouter
