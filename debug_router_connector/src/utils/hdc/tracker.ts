import { EventEmitter } from "events";
import Target from "./Target";
import Client from "./client";

export default class Tracker extends EventEmitter {
  private targets: Target[] = [];
  private targetMap: Record<string, Target> = {};
  constructor(private readonly client: Client) {
    super();
    this.read();
  }

  public read() {
    this.client
      .listTargets()
      .then((targets) => {
        this.update(targets);
        setTimeout(() => {
          this.read();
        }, 1000);
      })
      .catch((err) => {
        this.emit("error", err);
      });
  }

  private update(newTragets: Target[]) {
    const newMap: Record<string, Target> = {};

    newTragets.forEach((newTarget) => {
      const oldTarget = this.targetMap[newTarget.connectKey];

      if (oldTarget) {
        if (oldTarget.connStatus !== newTarget.connStatus) {
          this.emit("change", newTarget, oldTarget);
        }
      } else {
        this.emit("add", newTarget);
      }
      newMap[newTarget.connectKey] = newTarget;
    });

    this.targets.forEach((oldTarget) => {
      if (!newMap[oldTarget.connectKey]) {
        this.emit("remove", oldTarget);
      }
    });

    this.targets = newTragets;
    this.targetMap = newMap;
  }
}
