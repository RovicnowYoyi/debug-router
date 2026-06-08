import { AndroidDeviceManager } from "./AndroidDeviceManager";
import { defaultLogger } from "../../utils/logger";

describe("AndroidDeviceManager.queryDeviceName", () => {
  let warnSpy: jest.SpyInstance;
  let debugSpy: jest.SpyInstance;
  let manager: AndroidDeviceManager;

  beforeEach(() => {
    jest.useFakeTimers();
    warnSpy = jest.spyOn(defaultLogger, "warn").mockImplementation(() => {});
    debugSpy = jest.spyOn(defaultLogger, "debug").mockImplementation(() => {});
    delete process.env.DEBUG_ROUTER_ADB_PROPS_TIMEOUT_MS;
    manager = new AndroidDeviceManager({} as any, {});
  });

  afterEach(() => {
    jest.useRealTimers();
    jest.restoreAllMocks();
    delete process.env.DEBUG_ROUTER_ADB_PROPS_TIMEOUT_MS;
  });

  test("returns product model when properties resolve before timeout", async () => {
    const deviceClient = {
      getProperties: jest.fn().mockResolvedValue({
        "ro.product.model": "Pixel 9",
      }),
    } as any;

    const result = await (manager as any).queryDeviceName(deviceClient, "serial-1");

    expect(result).toBe("Pixel 9");
    expect(warnSpy).not.toHaveBeenCalled();
    expect(debugSpy).not.toHaveBeenCalled();
    expect(jest.getTimerCount()).toBe(0);
  });

  test("falls back after configured timeout without blocking", async () => {
    process.env.DEBUG_ROUTER_ADB_PROPS_TIMEOUT_MS = "25";
    const deviceClient = {
      getProperties: jest.fn(
        () => new Promise<Record<string, string>>(() => {}),
      ),
    } as any;

    const resultPromise = (manager as any).queryDeviceName(
      deviceClient,
      "serial-2",
    );

    await jest.advanceTimersByTimeAsync(25);

    await expect(resultPromise).resolves.toBe("serial-2");
    expect(warnSpy).toHaveBeenCalledWith(
      "createDevice: getProperties timeout, fallback to serial-2",
    );
    expect(debugSpy).not.toHaveBeenCalled();
    expect(jest.getTimerCount()).toBe(0);
  });

  test("falls back and logs failure when getProperties rejects", async () => {
    const deviceClient = {
      getProperties: jest.fn().mockRejectedValue(new Error("adb failed")),
    } as any;

    const result = await (manager as any).queryDeviceName(deviceClient, "serial-3");

    expect(result).toBe("serial-3");
    expect(warnSpy).toHaveBeenCalledWith(
      "createDevice: getProperties failed, fallback to serial-3: adb failed",
    );
    expect(debugSpy).toHaveBeenCalledWith(
      "late getProperties error swallowed: adb failed",
    );
    expect(jest.getTimerCount()).toBe(0);
  });
});
