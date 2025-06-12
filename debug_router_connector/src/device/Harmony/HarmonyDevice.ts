import Client from "../../utils/hdc/client";
import TargetClient from "../../utils/hdc/TargetClient";
import Forward from "../../utils/hdc/Forward";

import { defaultLogger } from "../../utils/logger";
import { DebugRouterConnector } from "../../connector";
import { BaseDevice } from "../BaseDevice";
import detectPort from "detect-port";
import { DeviceOS } from "../../utils/type";

export default class HarmonyDevice extends BaseDevice {
  private hdc: Client;
  private static readonly localBasePort: number = 15000;

  constructor(
    driver: DebugRouterConnector,
    serial: string,
    title: string,
    hdc: Client,
    hdcOption: any,
  ) {
    super(driver, {
      serial: serial,
      os: DeviceOS.Harmony,
      title,
    });
    this.hdc = hdc;
  }

  getHost(): string {
    return this.driver.hdcOption?.host ?? "127.0.0.1";
  }

  async forwards() {
    await this.forward(this.remotePorts);
  }

  private async forward(remotePorts: number[]) {
    const device = this.hdc.getDevice(this.serial);
    if (!device) {
      return;
    }
    try {
      await this.removeForward(device);
    } catch (e) {
      defaultLogger.debug(JSON.stringify(e));
    }
    this.port = [];
    // randomBase <=19 && randomBase>=0
    const randomBase = Math.floor(Math.random() * 20);
    for (let i = 0; i < remotePorts.length; i++) {
      const remotePort = remotePorts[i];
      let tryCount = 0;
      while (tryCount < 5) {
        let hostport =
          HarmonyDevice.localBasePort +
          randomBase * 500 +
          i * 100 +
          tryCount * 10;
        do {
          hostport++;
          hostport = await detectPort(hostport);
        } while (this.port.indexOf(hostport) != -1);
        try {
          const result = await device.forward(
            `tcp:${hostport}`,
            `tcp:${remotePort}`,
          );
          if (result) {
            defaultLogger.debug(
              "forward success:" + remotePort + " hostport:" + hostport,
            );
            this.port.push(hostport);
            break;
          } else {
            tryCount++;
          }
        } catch (e: any) {
          defaultLogger.debug(
            "forward failed:" +
              remotePort +
              " e:" +
              e?.message +
              " tryCount:" +
              tryCount +
              " hostport:" +
              hostport,
          );
          tryCount++;
        }
      }

      if (tryCount >= 5) {
        defaultLogger.debug("forward failed:" + remotePort);
      }
    }
    defaultLogger.debug("adb forward result:" + JSON.stringify(this.port));
  }

  async removeForward(device: TargetClient) {
    return new Promise<void>(async (resolve, reject) => {
      const forwards: Forward[] = await device.listForwards();
      for (let i = 0; i < forwards.length; i++) {
        const result = await device.removeForward(
          forwards[i].local,
          forwards[i].remote,
        );
        defaultLogger.debug(result);
      }
      resolve();
    });
  }
}
