// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

export interface DebugRouterGlobalHandlerHarmony {
  onOpenCard: (url: string) => void;
  onMessage: (message: string, type: string) => void;
}

export interface DebugRouterSessionHandlerHarmony {
  onSessionCreate: (session_id: number, url: string) => void;
  onSessionDestroy: (session_id: number) => void;
  onMessage: (message: string, type: string, session_id: number) => void;
}

export interface DebugRouterMessageHandlerHarmony {
  handle: (params: string) => string;
  getName: () => string;
}

export interface DebugRouterStateListenerHarmony {
  onOpen: (connectionType: string) => void;
  onClose: (code: number, reason: string) => void;
  onMessage: (message: string) => void;
  onError: (error: string) => void;
}

export interface NativeSlotHarmony {
  onMessage: (message: string, type: string, session_id: number) => void;
}

export declare class DebugRouterLogHarmony {
  log: (message: string) => void;
}

export declare class DebugRouterHarmony {
  static addGlobalHandler: (handler: DebugRouterGlobalHandlerHarmony) => void;
  static removeGlobalHandler: (handler: DebugRouterGlobalHandlerHarmony) => void;
  static addSessionHandler: (handler: DebugRouterSessionHandlerHarmony) => void;
  static removeSessionHandler: (handler: DebugRouterSessionHandlerHarmony) => void;
  static addMessageHandler: (handler: DebugRouterMessageHandlerHarmony) => void;
  static removeMessageHandler: (handler: DebugRouterMessageHandlerHarmony) => void;
  static sendData: (type: string, session: number, data: string) => void;
}
 
