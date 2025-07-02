// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include <hilog/log.h>

#include "debug_router/native/log/logging.h"
#define HARMONY_LOG_PRINT_DOMAIN 0xAA00

namespace debugrouter {
namespace logging {
  class LogMessage;
}

#define LogMessage logging::LogMessage

namespace harmony {
void Log(LogMessage* msg) {
  // from hilog
  LogLevel priority = LogLevel::LOG_DEBUG;
  switch (msg->severity()) {
    case LogLevel::LOG_VERBOSE:
      priority = LogLevel::LOG_DEBUG;
      break;
    case LogLevel::LOG_INFO:
      priority = LogLevel::LOG_INFO;
      break;
    case LogLevel::LOG_WARNING:
      priority = LogLevel::LOG_WARN;
      break;
    case LogLevel::LOG_ERROR:
      priority = LogLevel::LOG_ERROR;
      break;
    case LogLevel::LOG_FATAL:
      priority = LogLevel::LOG_FATAL;
      break;
  }
  const char* tag = "DebugRouter";
  OH_LOG_Print(LOG_APP, priority, HARMONY_LOG_PRINT_DOMAIN, tag, "%{public}s",
               msg->stream().str().c_str());
}

}  // namespace harmony
}  // namespace debugrouter
