// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include <hilog/log.h>

#include <memory>
#include <string>

#include "debug_router/harmony/debug_router/src/main/cpp/debug_router_log_harmony.h"

namespace debugrouter {
namespace harmony {

DebugRouterLogHarmony::DebugRouterLogHarmony(napi_env env, napi_value js_this)
  : env_(env), js_this_ref_(nullptr) {
napi_create_reference(env, js_this, 1, &js_this_ref_);
}

napi_value DebugRouterLogHarmony::SetNativeMinLogLvel(napi_env env, napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  int32_t level;
  napi_get_value_int32(env, argv[0], &level);
  debugrouter::logging::SetMinLogLevel(level);
  return nullptr;
}

napi_value DebugRouterLogHarmony::SetHasLoggingDelegate(napi_env env, napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  bool has_logging_delegate;
  napi_get_value_bool(env, argv[0], &has_logging_delegate);
  if (has_logging_delegate) {
    DebugRouterLogHarmony* current_instance = nullptr;
        napi_unwrap(env, js_this, reinterpret_cast<void**>(&current_instance));
    if (current_instance == nullptr) {
      return nullptr;
    }
    debugrouter::logging::SetLoggingDelegate(
      std::unique_ptr<debugrouter::harmony::DebugRouterLogHarmony>(current_instance)
    );
  } else {
    debugrouter::logging::SetLoggingDelegate(nullptr);
  }
  return nullptr;
}

void DebugRouterLogHarmony::Log(debugrouter::logging::LogMessage *msg) {
  LogLevel priority = LogLevel::LOG_DEBUG;
  switch (msg->severity()) {
    case logging::LOG_VERBOSE:
    case logging::LOG_DEBUG:
      priority = LogLevel::LOG_DEBUG;
      break;
    case logging::LOG_INFO:
      priority = LogLevel::LOG_INFO;
      break;
    case logging::LOG_WARNING:
      priority = LogLevel::LOG_WARN;
      break;
    case logging::LOG_ERROR:
      priority = LogLevel::LOG_ERROR;
      break;
    case logging::LOG_FATAL:
      priority = LogLevel::LOG_FATAL;
      break;
  }
  std::string tag = "DebugRouter";
  OH_LOG_Print(LOG_APP, priority, LOG_PRINT_DOMAIN, tag.c_str(), "%{public}s",
                msg->stream().c_str());
}

}  // namespace harmony
}  // namespace debugrouter
