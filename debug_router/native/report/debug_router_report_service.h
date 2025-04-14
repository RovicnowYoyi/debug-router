// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#ifndef DEBUGROUTER_NATIVE_REPORT_DEBUG_ROUTER_REPORT_SERVICE_H_
#define DEBUGROUTER_NATIVE_REPORT_DEBUG_ROUTER_REPORT_SERVICE_H_

#include "debug_router/native/report/debug_router_meta_info.h"

namespace debugrouter {
namespace report {

class DebugRouterReportService {
 public:
  virtual void init(DebugRouterMetaInfo DebugRouterMetaInfo) = 0;
  virtual void report(const std::string &eventName, const std::string &category,
                      const std::string &metric, const std::string &extra) = 0;
};

}  // namespace report
}  // namespace debugrouter

#endif  // DEBUGROUTER_NATIVE_REPORT_DEBUG_ROUTER_REPORT_SERVICE_H_
