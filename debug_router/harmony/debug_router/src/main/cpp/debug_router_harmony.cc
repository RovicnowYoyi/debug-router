// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include "debug_router/harmony/debug_router/src/main/cpp/debug_router_harmony.h"
#include "debug_router/harmony/debug_router/src/main/base/napi_util.h"
#include "debug_router/harmony/debug_router/src/main/cpp/native_slot_harmony.h"
#include "debug_router/harmony/debug_router/src/main/cpp/debug_router_state_listener_harmony.h"

namespace debugrouter {
namespace harmony {

std::map<napi_value, int, NapiValueCompare> DebugRouterHarmony::global_handlers_map_;
std::map<napi_value, int, NapiValueCompare> DebugRouterHarmony::session_handlers_map_;
std::map<napi_value, std::string, NapiValueCompare> DebugRouterHarmony::message_handlers_map_;

napi_value DebugRouterHarmony::Init(napi_env env, napi_value exports) {
  napi_property_descriptor properties[] = {
      {"createInstance", 0, DebugRouterHarmony::CreateInstance, 0, 0, 0,
       napi_static, 0},
      {"addGlobalHandler", 0, DebugRouterHarmony::AddGlobalHandler, 0, 0, 0,
       napi_static, 0},
      {"removeGlobalHandler", 0, DebugRouterHarmony::RemoveGlobalHandler, 0, 0,
       0, napi_static, 0},
      {"addSessionHandler", 0, DebugRouterHarmony::AddSessionHandler, 0, 0,
       0, napi_static, 0},
      {"removeSessionHandler", 0, DebugRouterHarmony::RemoveSessionHandler, 0, 0,
       0, napi_static, 0},
      {"addMessageHandler", 0, DebugRouterHarmony::AddMessageHandler, 0, 0,
       0, napi_static, 0},
      {"removeMessageHandler", 0, DebugRouterHarmony::RemoveMessageHandler, 0, 0,
       0, napi_static, 0},
      {"addStateListener", 0, DebugRouterHarmony::AddStateListener, 0, 0,
       0, napi_static, 0},
      {"connectAsync", 0, DebugRouterHarmony::ConnectAsync, 0, 0, 0, napi_static, 0},
      {"disconnectAsync", 0, DebugRouterHarmony::DisconnectAsync, 0, 0, 0, napi_static, 0},
      {"isConnected", 0, DebugRouterHarmony::IsConnected, 0, 0, 0, napi_static, 0},
      {"sendAsync", 0, DebugRouterHarmony::SendAsync, 0, 0, 0, napi_static, 0},
      {"sendDataAsync", 0, DebugRouterHarmony::SendDataAsync, 0, 0, 0, napi_static, 0},
      {"plug", 0, DebugRouterHarmony::Plug, 0, 0, 0, napi_static, 0},
      {"pull", 0, DebugRouterHarmony::Pull, 0, 0, 0, napi_static, 0},
      {"isValidSchema", 0, DebugRouterHarmony::IsValidSchema, 0, 0, 0, napi_static, 0},
      {"handleSchema", 0, DebugRouterHarmony::HandleSchema, 0, 0, 0, napi_static, 0}};
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

napi_value DebugRouterHarmony::CreateInstance(napi_env env,
                                           napi_callback_info info) {
  core::DebugRouterCore::GetInstance();
  return nullptr;
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

napi_value DebugRouterHarmony::AddSessionHandler(napi_env env,
                                                napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  if (session_handlers_map_.find(argv[0]) == session_handlers_map_.end()) {
    auto sessionHandler = std::make_shared<DebugRouterSessionHandlerHarmony>(env, argv[0]);
    int handler_id = core::DebugRouterCore::GetInstance().AddSessionHandler(sessionHandler.get());
    session_handlers_map_[argv[0]] = handler_id;
  }
  return nullptr;
}

napi_value DebugRouterHarmony::RemoveSessionHandler(napi_env env,
                                                   napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_value result;

  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  if (session_handlers_map_.find(argv[0]) != session_handlers_map_.end()) {
    int handler_id = session_handlers_map_[argv[0]];
    bool res = core::DebugRouterCore::GetInstance().RemoveSessionHandler(handler_id);
    session_handlers_map_.erase(argv[0]);
    napi_get_boolean(env, res, &result);
    return result;
  }
  napi_get_boolean(env, false, &result);
  return result;
}

napi_value DebugRouterHarmony::AddMessageHandler(napi_env env,
                                                 napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  if (message_handlers_map_.find(argv[0]) == message_handlers_map_.end()) {
    auto messageHandler = std::make_shared<DebugRouterMessageHandlerHarmony>(env, argv[0]);
    core::DebugRouterCore::GetInstance().AddMessageHandler(messageHandler.get());
    message_handlers_map_[argv[0]] = messageHandler->GetName();
  }
  return nullptr;
}

napi_value DebugRouterHarmony::RemoveMessageHandler(napi_env env,
                                                    napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  if (message_handlers_map_.find(argv[0]) != message_handlers_map_.end()) {
    std::string name = message_handlers_map_[argv[0]];
    core::DebugRouterCore::GetInstance().RemoveMessageHandler(name);
    message_handlers_map_.erase(argv[0]);
  }
  return nullptr;
}

napi_value DebugRouterHarmony::AddStateListener(napi_env env,
                                                 napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  auto stateListener = std::make_shared<DebugRouterStateListenerHarmony>(env, argv[0]);
  core::DebugRouterCore::GetInstance().AddStateListener(stateListener);
  return nullptr;
}

napi_value DebugRouterHarmony::ConnectAsync(napi_env env, napi_callback_info info) {
  napi_value js_this;
  size_t argc = 2;
  napi_value argv[2];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  std::string url = base::NapiUtil::ConvertToString(env, argv[0]);
  std::string room = base::NapiUtil::ConvertToString(env, argv[1]);
  core::DebugRouterCore::GetInstance().ConnectAsync(url, room);
  return nullptr;
}

napi_value DebugRouterHarmony::DisconnectAsync(napi_env env, napi_callback_info info) {
  core::DebugRouterCore::GetInstance().DisconnectAsync();
  return nullptr;
}

napi_value DebugRouterHarmony::IsConnected(napi_env env, napi_callback_info info) {
  bool isConnected = core::DebugRouterCore::GetInstance().IsConnected();
  napi_value result;
  napi_get_boolean(env, isConnected, &result);
  return result;
}


napi_value DebugRouterHarmony::SendAsync(napi_env env, napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  std::string message = base::NapiUtil::ConvertToString(env, argv[0]);
  core::DebugRouterCore::GetInstance().SendAsync(message);
  return nullptr;
}

napi_value DebugRouterHarmony::SendDataAsync(napi_env env, napi_callback_info info) {
  napi_value js_this;
  size_t argc = 3;
  napi_value argv[3];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  std::string type = base::NapiUtil::ConvertToString(env, argv[0]);
  int32_t session;
  napi_get_value_int32(env, argv[1], &session);
  std::string data = base::NapiUtil::ConvertToString(env, argv[2]);
  // 只为harmony提供传递data为string的接口
  core::DebugRouterCore::GetInstance().SendDataAsync(data, type, session, -1, false);
  return nullptr;
}

napi_value DebugRouterHarmony::Plug(napi_env env, napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  auto slot = std::make_shared<NativeSlotHarmony>(env, argv[0]);
  core::DebugRouterCore::GetInstance().Plug(slot);
  return nullptr;
}

napi_value DebugRouterHarmony::Pull(napi_env env, napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  int session_id;
  napi_get_value_int32(env, argv[0], &session_id);
  core::DebugRouterCore::GetInstance().Pull(session_id);
  return nullptr;
}

napi_value DebugRouterHarmony::IsValidSchema(napi_env env, napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  std::string schema = base::NapiUtil::ConvertToString(env, argv[0]);
  bool res = core::DebugRouterCore::GetInstance().IsValidSchema(schema);
  napi_value result;
  napi_get_boolean(env, res, &result);
  return result;
}

napi_value DebugRouterHarmony::HandleSchema(napi_env env, napi_callback_info info) {
  napi_value js_this;
  size_t argc = 1;
  napi_value argv[1];
  napi_get_cb_info(env, info, &argc, argv, &js_this, nullptr);
  std::string schema = base::NapiUtil::ConvertToString(env, argv[0]);
  bool res = core::DebugRouterCore::GetInstance().HandleSchema(schema);
  napi_value result;
  napi_get_boolean(env, res, &result);
  return result;
}
}  // namespace harmony
}  // namespace debugrouter
