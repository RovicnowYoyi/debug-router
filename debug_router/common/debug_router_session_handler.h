// Copyright 2023 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#ifndef DEBUGROUTER_COMMON_DEBUG_ROUTER_SESSION_HANDLER_H_
#define DEBUGROUTER_COMMON_DEBUG_ROUTER_SESSION_HANDLER_H_

#include <string>

namespace debugrouter {
namespace common {

class DebugRouterSessionHandler {
 public:
  virtual void OnSessionCreate(int session_id, const std::string &url) = 0;
  virtual void OnSessionDestroy(int session_id) = 0;
  virtual void OnMessage(const std::string &message, const std::string &type,
                         int session_id) = 0;
};

}  // namespace common
}  // namespace debugrouter

#endif  // DEBUGROUTER_COMMON_DEBUG_ROUTER_SESSION_HANDLER_H_
