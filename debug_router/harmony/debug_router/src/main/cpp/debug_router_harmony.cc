// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include "debug_router/harmony/debug_router/src/main/cpp/debug_router_harmony.h"

namespace debugrouter {
namespace harmony {

std::map<napi_value, std::shared_ptr<DebugRouterGlobalHandlerHarmony>, NapiValueCompare>
    DebugRouterHarmony::global_handlers_map_;
std::map<napi_value, std::shared_ptr<DebugRouterSessionHandlerHarmony>, NapiValueCompare>
    DebugRouterHarmony::session_handlers_map_;

napi_value DebugRouterHarmony::Init(napi_env env, napi_value exports) {
  napi_property_descriptor properties[] = {
      {"addGlobalHandler", 0, DebugRouterHarmony::AddGlobalHandler, 0, 0, 0,
       napi_static, 0},
      {"removeGlobalHandler", 0, DebugRouterHarmony::RemoveGlobalHandler, 0, 0,
       0, napi_static, 0},
      {"sendData", 0, DebugRouterHarmony::SendData, 0, 0, 0, napi_static, 0}};
  constexpr size_t size = std::size(properties);

  napi_value cons;
  napi_status status =
      napi_define_class(env, "DebugRouterHarmony", NAPI_AUTO_LENGTH,
                        Constructor, nullptr, size, properties, &cons);

  status = napi_set_named_property(env, exports, "DebugRouterHarmony", cons);
  return exports;
}

napi_value DebugRouterWrapper::AddGlobalHandler(napi_env env,
                                                napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  if (handlers_.find(argv[0]) == handlers_.end()) {
    auto globalHandler = std::make_shared<HarmonyGlobalHandler>(env, argv[0]);
    handlers_[argv[0]] = globalHandler;
    debugrouter::DebugRouter::GetInstance().AddGlobalHandler(
        globalHandler.get());
  }
  return nullptr;
}

napi_value DebugRouterWrapper::RemoveGlobalHandler(napi_env env,
                                                   napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  if (handlers_.find(argv[0]) != handlers_.end()) {
    auto globalHandler = handlers_[argv[0]];
    debugrouter::DebugRouter::GetInstance().RemoveGlobalHandler(
        globalHandler.get());
    handlers_.erase(argv[0]);
  }
  return nullptr;
}

napi_value DebugRouterWrapper::SendData(napi_env env, napi_callback_info info) {
  napi_value js_this;
  size_t argc = 3;
  napi_value argv[3];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  std::string type = base::NapiUtil::ConvertToString(env, argv[0]);
  int32_t session;
  napi_get_value_int32(env, argv[1], &session);
  std::string data = base::NapiUtil::ConvertToString(env, argv[2]);
  debugrouter::DebugRouter::GetInstance().SendData(data, type, session);
  return nullptr;
}

napi_value DebugRouterWrapper::Constructor(napi_env env,
                                           napi_callback_info info) {
  size_t argc = 0;
  napi_value args[1];
  napi_value js_this;
  napi_get_cb_info(env, info, &argc, args, &js_this, nullptr);
  return js_this;
}

napi_value DebugRouterWrapper::SendData(napi_env env, napi_callback_info info) {
napi_value js_this;
size_t argc = 3;
napi_value argv[3];
napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
std::string type = base::NapiUtil::ConvertToString(env, argv[0]);
int32_t session;
napi_get_value_int32(env, argv[1], &session);
std::string data = base::NapiUtil::ConvertToString(env, argv[2]);
debugrouter::DebugRouter::GetInstance().SendData(data, type, session);
return nullptr;
}

}  // namespace harmony
}  // namespace debugrouter
