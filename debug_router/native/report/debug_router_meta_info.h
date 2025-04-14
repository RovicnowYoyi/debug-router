// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#ifndef DEBUGROUTER_NATIVE_REPORT_DEBUG_ROUTER_META_INFO_H_
#define DEBUGROUTER_NATIVE_REPORT_DEBUG_ROUTER_META_INFO_H_

namespace debugrouter {
namespace report {

struct DebugRouterMetaInfo {
  std::string version;
  std::string appProcessName;

  DebugRouterMetaInfo(const std::string &version,
                      const std::string &processName)
      : version(version), appProcessName(processName) {}
};

}  // namespace report
}  // namespace debugrouter

#endif  // DEBUGROUTER_NATIVE_REPORT_DEBUG_ROUTER_META_INFO_H_
