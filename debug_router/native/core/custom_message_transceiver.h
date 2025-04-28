// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#ifndef DEBUGROUTER_NATIVE_CORE_CUSTOM_MESSAGE_TRANSCEIVER_H_
#define DEBUGROUTER_NATIVE_CORE_CUSTOM_MESSAGE_TRANSCEIVER_H_

#include <string>

#include "debug_router/native/core/message_transceiver.h"

namespace debugrouter {
namespace core {

// used for custome websocket transceiver
class CustomMessageTransceiver : public MessageTransceiver {
 public:
 CustomMessageTransceiver();
  ~CustomMessageTransceiver();

  virtual void WrapAndSend(const std::string &type, int session, const std::string &data, int mark, bool isObject) = 0;
};

}  // namespace core
}  // namespace debugrouter

#endif  // DEBUGROUTER_NATIVE_CORE_CUSTOM_MESSAGE_TRANSCEIVER_H_
