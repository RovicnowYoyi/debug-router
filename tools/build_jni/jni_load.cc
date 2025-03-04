#include "base/android/convert.h"
#include "base/android/params_transform.h"
#include "base/threading/message_pump_android.h"
#include "content/content_main.h"
#include "debugger/android/debug_host_impl.h"
#include "net/android/url_request_android.h"
#include "plugin/base/android/plugin_impl.h"
#include "render/android/jni_coordinator_bridge.h"
#include "render/android/render_object_impl_android.h"
#include "render/android/render_tree_host_impl_android.h"
#include "render/label_measurer.h"
#include "runtime/android/element_register_util.h"
#include "runtime/android/jni_runtime_bridge.h"
#include "runtime/android/lynx_object_android.h"
#include "runtime/android/result_callback.h"
#if GTEST_ENABLE
#include "test/gtest_driver.h"
#endif

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
  base::android::InitVM(vm);
  JNIEnv *env = base::android::AttachCurrentThread();
  base::MessagePumpAndroid::RegisterJNIUtils(env);
  lynx::ElementRegisterUtil::RegisterJNIUtils(env);
  base::Convert::BindingJavaClass(env);
  lynx::LabelMeasurer::RegisterJNIUtils(env);
  net::URLRequestAndroid::RegisterJNIUtils(env);
  lynx::RenderObjectImplAndroid::RegisterJNIUtils(env);
  base::android::ParamsTransform::RegisterJNIUtils(env);
  jscore::JNIRuntimeBridge::RegisterJNIUtils(env);
  lynx::RenderTreeHostImplAndroid::RegisterJNIUtils(env);
  jscore::LynxObjectAndroid::RegisterJNIUtils(env);
  lynx::JNICoordinatorBridge::RegisterJNIUtils(env);
  jscore::ResultCallbackAndroid::RegisterJNIUtils(env);
  debug::DebugHostImpl::RegisterJNIUtils(env);
  content::ContentMain::RegisterContentMain(env);
  plugin::PluginImpl::RegisterJNIUtils(env);
#if GTEST_ENABLE
  test::GTestBridge::RegisterJNIUtils(env);
#endif
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {}
