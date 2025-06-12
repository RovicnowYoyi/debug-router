import Target from "../../utils/hdc/Target";
import Client from "../../utils/hdc/client";
import HarmonyDevice from "./HarmonyDevice";
import { WatchStatus } from "../../device/WatchStatus";
import { DebugRouterConnector } from "../../connector";
import { DeviceManager } from "../DeviceManager";
import { defaultLogger } from "../../utils/logger";

export default class HarmonyDeviceManager extends DeviceManager {
  private currentWatchStatus: WatchStatus = WatchStatus.StopWatching;
  constructor(
    driver: DebugRouterConnector,
    private readonly hdcClinet: Client,
    private readonly hdcOption: any,
  ) {
    super(driver);
    this.hdcOption = hdcOption;
  }

  private async createDevice(
    hdcClinet: Client,
    target: Target,
  ): Promise<HarmonyDevice | undefined> {
    return new Promise(async (resolve, reject) => {
      try {
        const device: HarmonyDevice = new HarmonyDevice(
          this.driver,
          target.connectKey,
          "Harmony",
          this.hdcClinet,
          this.hdcOption,
        );
        await device.forwards();
        resolve(device);
      } catch (e: any) {
        const msg =
          "createDevice: harmony: error" + target.connectKey + " " + e?.message;
        defaultLogger.debug(msg);
        resolve(undefined);
      }
    });
  }

  private async registerDevice(hdcClinet: Client, target: Target) {
    const harmonyDevice = await this.createDevice(hdcClinet, target);
    if (!harmonyDevice) {
      return;
    }
    this.driver.registerDevice(harmonyDevice);
  }

  async watchDevices() {
    if (this.currentWatchStatus !== WatchStatus.StopWatching) {
      return;
    }
    this.currentWatchStatus = WatchStatus.PrepareToWatch;

    try {
      this.hdcClinet
        .trackTargets()
        .then((tracker) => {
          tracker.on("error", async (err: Error) => {
            this.currentWatchStatus = WatchStatus.StopWatching;
            const msg = "tracker error:" + err?.message;
            defaultLogger.debug(msg);
            this.unregisterAllHarmonyTarget();
            this.reWatchHarmonyDevices();
          });

          tracker.on("add", (target: Target) => {
            this.currentWatchStatus = WatchStatus.Watching;
            defaultLogger.debug("tracker add:" + JSON.stringify(target));
            if (target.connStatus === "Connected") {
              this.registerDevice(this.hdcClinet, target);
            }
          });

          tracker.on("change", (newTarget: Target, oldTarget: Target) => {
            this.currentWatchStatus = WatchStatus.Watching;
            defaultLogger.debug("tracker change:" + JSON.stringify(newTarget));
            if (newTarget.connStatus === "Connected") {
              this.registerDevice(this.hdcClinet, newTarget);
            } else {
              if (this.driver.devices.has(newTarget.connectKey)) {
                this.driver.unregisterDevice(newTarget.connectKey);
              }
            }
          });

          tracker.on("remove", (target: Target) => {
            this.currentWatchStatus = WatchStatus.Watching;
            defaultLogger.debug("tracker remove:" + JSON.stringify(target));
            if (this.driver.devices.has(target.connType)) {
              this.driver.unregisterDevice(target.connType);
            }
          });
        })
        .catch(async (err: any) => {
          this.currentWatchStatus = WatchStatus.StopWatching;
          const msg = "listTargets catch:" + err?.message;
          defaultLogger.debug(msg);
        });
    } catch (e: any) {
      // TODO ineffectively branch
      this.currentWatchStatus = WatchStatus.StopWatching;
      const msg = "watchHarmonyDevices error:" + e?.message;
      defaultLogger.debug(msg);
    }
  }

  private reWatchHarmonyDevices() {
    setTimeout(() => {
      this.watchDevices();
    }, 300);
  }

  private unregisterAllHarmonyTarget() {
    const harmonyDevices = Array.from(this.driver.devices.values()).filter(
      (d) => d instanceof HarmonyDevice,
    );
    harmonyDevices.forEach((device) => {
      this.driver.unregisterDevice(device.serial);
    });
  }
}
