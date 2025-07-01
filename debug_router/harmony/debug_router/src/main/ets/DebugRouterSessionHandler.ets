// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

export abstract class DebugRouterSessionHandler {
  abstract onSessionCreate(sessionId: number, url: string): void;
  abstract onSessionDestroy(sessionId: number): void;
  abstract onMessage(message: string, type: string, sessionId: number): void;
}