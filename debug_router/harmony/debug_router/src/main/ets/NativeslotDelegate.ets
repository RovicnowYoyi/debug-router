// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

import { DebugRouterSlot } from './DebugRouterSlot'
import { NativeSlotHarmony } from 'libdebugrouter.so'

export class NativeSlotDelegate implements NativeSlotHarmony {
  private slot: DebugRouterSlot;

  constructor(slot: DebugRouterSlot) {
    this.slot = slot;
  }

  public getUrl(): string {
    return this.slot.getTemplateUrl() ?? "";
  }

  public getType(): string {
    return this.slot.getType() ?? "";
  }

  public onMessage(message: string, type: string): void {
    this.slot.onMessage(message, type);
  }
}
