// Copyright 2026 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include "debug_router/native/protocol/protocol.h"

#include <limits>

#include "gtest/gtest.h"
#include "json/reader.h"

namespace debugrouter {
namespace protocol {
namespace {

Json::Value ParseJson(const std::string& json) {
  Json::Reader reader;
  Json::Value root;
  EXPECT_TRUE(reader.parse(json, root));
  return root;
}

TEST(ProtocolTest, ClientIdMinusOneRoundTripsAsSignedValue) {
  auto body = RemoteDebugProtocol::CreateProtocolBody4Init(kInvalidClientId);
  Json::Value root = ParseJson(RemoteDebugProtocol::Stringify(body));

  ASSERT_TRUE(root[kKeyData].isInt64());
  EXPECT_EQ(root[kKeyData].asInt64(), kInvalidClientId);

  auto parsed = RemoteDebugProtocol::Parse(root);
  ASSERT_TRUE(parsed);
  ASSERT_TRUE(parsed->IsProtocolBody4Init());
  EXPECT_EQ(parsed->AsInit()->client_id_, kInvalidClientId);
}

TEST(ProtocolTest, ClientIdSupportsInt64RangeWithinJsonSafeInteger) {
  constexpr RemoteDebugPrococolClientId kLargeClientId = 9007199254740991LL;
  auto body = RemoteDebugProtocol::CreateProtocolBody4Init(kLargeClientId);
  Json::Value root = ParseJson(RemoteDebugProtocol::Stringify(body));

  ASSERT_TRUE(root[kKeyData].isInt64());
  EXPECT_EQ(root[kKeyData].asInt64(), kLargeClientId);

  auto parsed = RemoteDebugProtocol::Parse(root);
  ASSERT_TRUE(parsed);
  ASSERT_TRUE(parsed->IsProtocolBody4Init());
  EXPECT_EQ(parsed->AsInit()->client_id_, kLargeClientId);
}

TEST(ProtocolTest, CustomizedClientIdFieldsRoundTripAsSignedValues) {
  auto cdp_data = std::make_shared<CustomData4CDP>();
  cdp_data->client_id_ = kInvalidClientId;
  cdp_data->session_id_ = 7;
  cdp_data->message_ = R"({"id":1})";
  auto body = RemoteDebugProtocol::CreateProtocolBody4Custom(
      kRemoteDebugProtocolBodyData4CDP, kInvalidClientId, cdp_data);
  Json::Value root = ParseJson(RemoteDebugProtocol::Stringify(body));

  ASSERT_TRUE(root[kKeyData][kKeySender].isInt64());
  ASSERT_TRUE(root[kKeyData][kKeyData][kKeyClientId].isInt64());
  EXPECT_EQ(root[kKeyData][kKeySender].asInt64(), kInvalidClientId);
  EXPECT_EQ(root[kKeyData][kKeyData][kKeyClientId].asInt64(), kInvalidClientId);

  auto parsed = RemoteDebugProtocol::Parse(root);
  ASSERT_TRUE(parsed);
  ASSERT_TRUE(parsed->IsProtocolBody4Custom());
  EXPECT_EQ(parsed->AsCustom()->client_id_, kInvalidClientId);
  EXPECT_EQ(parsed->AsCustom()->AsCDP()->client_id_, kInvalidClientId);
}

TEST(ProtocolTest, ClientIdSupportsSignedLowerBound) {
  constexpr RemoteDebugPrococolClientId kMinClientId =
      std::numeric_limits<RemoteDebugPrococolClientId>::min();
  auto body = RemoteDebugProtocol::CreateProtocolBody4Init(kMinClientId);
  Json::Value root = ParseJson(RemoteDebugProtocol::Stringify(body));

  ASSERT_TRUE(root[kKeyData].isInt64());
  EXPECT_EQ(root[kKeyData].asInt64(), kMinClientId);

  auto parsed = RemoteDebugProtocol::Parse(root);
  ASSERT_TRUE(parsed);
  ASSERT_TRUE(parsed->IsProtocolBody4Init());
  EXPECT_EQ(parsed->AsInit()->client_id_, kMinClientId);
}

}  // namespace
}  // namespace protocol
}  // namespace debugrouter
