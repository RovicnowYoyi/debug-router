import { join } from "path";
import Hdc from "./hdc/hdc";
import Client from "./hdc/client";

let hdcPath: string | null = null;
export function getHdcInstance(hdcOption: any): Client {
  const host = hdcOption?.host ?? "127.0.0.1";
  const port = hdcOption?.port ?? 8710;
  const hdcClient =
    hdcPath === null
      ? Hdc.createClient({
          host,
          port,
        })
      : Hdc.createClient({
          host,
          port,
          bin:
            process.platform !== "win32"
              ? join(hdcPath!, "hdc")
              : join(hdcPath!, "hdc.exe"),
        });
  return hdcClient;
}
