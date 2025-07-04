import { DebugRouter, DebugRouterStateListenerHarmony } from '@lynx/debug_router';
import { NativeSlotHarmony } from '@lynx/debug_router';
import { hilog } from '@kit.PerformanceAnalysisKit';

class StateListener implements DebugRouterStateListenerHarmony {
  public onOpen(connectionType: string): void {
    hilog.info(0x0000, 'StateListener', '%{public}s', 'StateListener onOpen');
  }

  public onClose(code: number, reason: string): void {
    hilog.info(0x0000, 'StateListener', '%{public}s', 'StateListener onClose');
  }
  public onMessage(message: string): void {
    hilog.info(0x0000, 'StateListener', '%{public}s', 'StateListener onMessage');
  }
  public onError(error: string): void {
    hilog.info(0x0000, 'StateListener', '%{public}s', 'StateListener onError');
  }
}

@Entry
@Component
struct Index {
  aboutToAppear() {
    DebugRouter.createInstance();
    DebugRouter.addStateListener(new StateListener());
  }
  build() {
    Column() {
      Text('Hello world!')
        .fontSize(30)
    }
    .height('100%')
    .width('100%')
    .justifyContent(FlexAlign.Center)
    .alignItems(HorizontalAlign.Center)
  }
}