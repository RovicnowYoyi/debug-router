// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#ifndef DEBUGROUTER_HARMONY_DEBUGROUTER_SRC_MAIN_CPP_DEBUG_ROUTER_LOG_HARMONY_H_
#define DEBUGROUTER_HARMONY_DEBUGROUTER_SRC_MAIN_CPP_DEBUG_ROUTER_LOG_HARMONY_H_

#include <node_api.h>

#include "debug_router/native/log/logging.h"

namespace debugrouter {
namespace logging {
  class LogMessage;
}

namespace harmony {

class DebugRouterLogHarmony : public debugrouter::logging::LoggingDelegate {

  public:
  DebugRouterLogHarmony(napi_env env, napi_value js_object);
  virtual ~DebugRouterLogHarmony() {
    napi_delete_reference(env_, js_this_ref_);
  };

  void Log(LogMessage *msg) override;

 private:
  static napi_value Constructor(napi_env env, napi_callback_info info);
  static napi_value SetNativeMinLogLvel(napi_env env, napi_callback_info info);
  static napi_value SetHasLoggingDelegate(napi_env env, napi_callback_info info);

  napi_env env_;
  napi_ref js_this_ref_;
};

}  // namespace harmony
}  // namespace debugrouter

#endif  // DEBUGROUTER_HARMONY_DEBUGROUTER_SRC_MAIN_CPP_DEBUG_ROUTER_LOG_HARMONY_H_
