// Copyright 2024 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

package com.lynx.debugrouter;

import androidx.annotation.Keep;
import com.lynx.debugrouter.base.CalledByNative;
import com.lynx.debugrouter.base.report.DebugRouterMetaInfo;
import com.lynx.debugrouter.base.report.IDebugRouterReportService;
import com.lynx.debugrouter.log.LLog;
import org.json.JSONObject;

@Keep
public class NativeReportServiceDelegate {
  private static final String TAG = "NativeReportServiceDelegate";
  private IDebugRouterReportService report;

  public NativeReportServiceDelegate(IDebugRouterReportService report) {
    this.report = report;
  }

  @CalledByNative
  public void init(String version, String processName) {
    if (report != null) {
      DebugRouterMetaInfo metaInfo = new DebugRouterMetaInfo(version, processName);
      LLog.i(TAG, "init report service with version: " + version + " processName: " + processName);
      report.init(metaInfo);
    }
  }

  private static JSONObject stringToJSONObject(String jsonString) {
    try {
      return new JSONObject(jsonString);
    } catch (org.json.JSONException e) {
      System.err.println("Failed to parse JSON: " + e.getMessage());
      return new JSONObject();
    }
  }

  @CalledByNative
  public void report(String eventName, String category, String metric, String extra) {
    if (report != null) {
      JSONObject categoryObject = stringToJSONObject(category);
      JSONObject metricObject = stringToJSONObject(metric);
      JSONObject extraObject = stringToJSONObject(extra);
      report.report(eventName, categoryObject, metricObject, extraObject);
    }
  }
}
