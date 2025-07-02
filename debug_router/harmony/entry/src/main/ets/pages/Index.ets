import { DebugRouterHarmony } from '@lynx/debug_router';

@Entry
@Component
struct Index {
  aboutToAppear() {
    DebugRouterHarmony.
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