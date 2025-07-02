// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include "debug_router/harmony/debug_router/src/main/cpp/debug_router_harmony.h"
#include "debug_router/harmony/debug_router/src/main/base/napi_util.h"

namespace debugrouter {
namespace harmony {

std::map<napi_value, int, NapiValueCompare> DebugRouterHarmony::global_handlers_map_;
std::map<napi_value, int, NapiValueCompare> DebugRouterHarmony::session_handlers_map_;
std::map<napi_value, std::string, NapiValueCompare> DebugRouterHarmony::message_handlers_map_;

napi_value DebugRouterHarmony::Init(napi_env env, napi_value exports) {
  napi_property_descriptor properties[] = {
      {"addGlobalHandler", 0, DebugRouterHarmony::AddGlobalHandler, 0, 0, 0,
       napi_static, 0},
      {"removeGlobalHandler", 0, DebugRouterHarmony::RemoveGlobalHandler, 0, 0,
       0, napi_static, 0},
      {"sendDataAsync", 0, DebugRouterHarmony::SendDataAsync, 0, 0, 0, napi_static, 0}};
  constexpr size_t size = std::size(properties);

  napi_value cons;
  napi_status status =
      napi_define_class(env, "DebugRouterHarmony", NAPI_AUTO_LENGTH,
                        Constructor, nullptr, size, properties, &cons);

  status = napi_set_named_property(env, exports, "DebugRouterHarmony", cons);
  return exports;
}

napi_value DebugRouterHarmony::Constructor(napi_env env,
                                           napi_callback_info info) {
  size_t argc = 0;
  napi_value args[1];
  napi_value js_this;
  napi_get_cb_info(env, info, &argc, args, &js_this, nullptr);
  return js_this;
}

napi_value DebugRouterHarmony::AddGlobalHandler(napi_env env,
                                                napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  if (global_handlers_map_.find(argv[0]) == global_handlers_map_.end()) {
    auto globalHandler = std::make_shared<DebugRouterGlobalHandlerHarmony>(env, argv[0]);
    int handler_id = core::DebugRouterCore::GetInstance().AddGlobalHandler(globalHandler.get());
    global_handlers_map_[argv[0]] = handler_id;
  }
  return nullptr;
}

napi_value DebugRouterHarmony::RemoveGlobalHandler(napi_env env,
                                                   napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_value result;

  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  if (global_handlers_map_.find(argv[0]) != global_handlers_map_.end()) {
    int handler_id = global_handlers_map_[argv[0]];
    bool res = core::DebugRouterCore::GetInstance().RemoveGlobalHandler(handler_id);
    global_handlers_map_.erase(argv[0]);
    napi_get_boolean(env, res, &result);
    return result;
  }
  napi_get_boolean(env, false, &result);
  return result;
}
/*
void DebugRouter::SendDataAsync(const std::string &data,
                                const std::string &type, int32_t session) {
  core::DebugRouterCore::GetInstance().SendDataAsync(data, type, session, -1,
                                                     false);
}

void DebugRouter::SendDataAsync(const std::string &data,
                                const std::string &type, int32_t session,
                                bool is_object) {
  core::DebugRouterCore::GetInstance().SendDataAsync(data, type, session, -1,
                                                     is_object);
}

// only one
void DebugRouterCore::SendDataAsync(const std::string &data, const std::string &type,
                    int32_t session, int32_t mark, bool is_object);
*/
napi_value DebugRouterHarmony::SendDataAsync(napi_env env, napi_callback_info info) {
  napi_value js_this;
  size_t argc = 4;
  napi_value argv[4];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  std::string type = base::NapiUtil::ConvertToString(env, argv[0]);
  int32_t session;
  napi_get_value_int32(env, argv[1], &session);
  std::string data = base::NapiUtil::ConvertToString(env, argv[2]);
  bool is_object = false;
  if (argc >= 4) {
    napi_get_value_bool(env, argv[3], &is_object);
  }
  core::DebugRouterCore::GetInstance().SendDataAsync(data, type, session, -1, is_object);
  return nullptr;
}

/*

  static napi_value AddSessionHandler(napi_env env, napi_callback_info info); // DebugRouterSessionHandlerHarmony *handler
  static napi_value RemoveSessionHandler(napi_env env, napi_callback_info info);
  static napi_value AddMessageHandler(napi_env env, napi_callback_info info); // DebugRouterMessageHandlerHarmony *handler
  static napi_value RemoveMessageHandler(napi_env env, napi_callback_info info);


  static napi_value ConnectAsync(napi_env env, napi_callback_info info); // url, room
  static napi_value DisconnectAsync(napi_env env, napi_callback_info info);
  static napi_value IsConnected(napi_env env, napi_callback_info info);

  static napi_value SendAsync(napi_env env, napi_callback_info info); // message
  static napi_value SendDataAsync(napi_env env, napi_callback_info info); // data, type, session_id, is_object

  static napi_value Plug(napi_env env, napi_callback_info info); // std::shared_ptr<DebugRouterSlot> &slot
  static napi_value Pull(napi_env env, napi_callback_info info); // session_id

  static napi_value IsValidSchema(napi_env env, napi_callback_info info); // schema
  static napi_value HandleSchema(napi_env env, napi_callback_info info); // schema

  static napi_value SetAppInfo(napi_env env, napi_callback_info info); // app_info(unordered_map) | key, value
  static napi_value GetAppInfoByKey(napi_env env, napi_callback_info info); // key
  static napi_value AddStateListener(napi_env env, napi_callback_info info); // DebugRouterStateListenerHarmony *listener

*/

}  // namespace harmony
}  // namespace debugrouter
