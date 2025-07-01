// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

export abstract class DebugRouterGlobalHandler {
  abstract openCard(url: string): void;
  abstract onMessage(type: string, sessionId: number, message: string): void;
}