// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include "debug_router/native/android/debug_router_report_service_android.h"

#include "debug_router/Android/build/gen/NativeReportServiceDelegate_jni.h"
#include "debug_router/native/android/base/android/jni_helper.h"

namespace debugrouter {
namespace android {

DebugRouterReportServiceAndroid::DebugRouterReportServiceAndroid(JNIEnv *env,
                                                                 jobject slot)
    : jobj_ptr_(std::make_unique<
                debugrouter::common::android::ScopedGlobalJavaRef<jobject>>(
          env, slot)) {}

void DebugRouterReportServiceAndroid::init(
    report::DebugRouterMetaInfo metaInfo) {
  std::string version = metaInfo.version;
  std::string processName = metaInfo.appProcessName;
  JNIEnv *env = debugrouter::common::android::AttachCurrentThread();
  Java_NativeReportServiceDelegate_init(
      env, jobj_ptr_->Get(),
      debugrouter::common::android::JNIHelper::ConvertToJNIStringRef(env,
                                                                     version)
          .Get(),
      debugrouter::common::android::JNIHelper::ConvertToJNIStringRef(
          env, processName)
          .Get());
}
void DebugRouterReportServiceAndroid::report(const std::string &eventName,
                                             const std::string &category,
                                             const std::string &metric,
                                             const std::string &extra) {
  JNIEnv *env = debugrouter::common::android::AttachCurrentThread();
  Java_NativeReportServiceDelegate_report(
      env, jobj_ptr_->Get(),
      debugrouter::common::android::JNIHelper::ConvertToJNIStringRef(env,
                                                                     eventName)
          .Get(),
      debugrouter::common::android::JNIHelper::ConvertToJNIStringRef(env,
                                                                     category)
          .Get(),
      debugrouter::common::android::JNIHelper::ConvertToJNIStringRef(env,
                                                                     metric)
          .Get(),
      debugrouter::common::android::JNIHelper::ConvertToJNIStringRef(env, extra)
          .Get());
}

bool DebugRouterReportServiceAndroid::RegisterJNIUtils(JNIEnv *env) {
  return RegisterNativesImpl(env);
}

}  // namespace android
}  // namespace debugrouter
