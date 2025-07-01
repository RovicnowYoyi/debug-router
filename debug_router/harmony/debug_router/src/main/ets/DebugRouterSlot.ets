// Copyright 2025 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

import { DebugRouter } from './DebugRouter';
import { DebugRouterSlotDelegate } from './DebugRouterSlotDelegate';

const TAG: string = 'DebugRouterSlot';

export class DebugRouterSlot {
  private mSessionId: number;
  private mDelegate: DebugRouterSlotDelegate;
  private mPlugged: boolean;
  private mType: string;

  constructor(mDelegate: DebugRouterSlotDelegate) {
    this.mDelegate = mDelegate;
  }

  pull(): void {
    if (mPlugged) {
      DebugRouter.getInstance().pull(mSessionId);
      mPlugged = flase;
    }
  }
  
}