// Copyright 2024 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include "debug_router/native/socket/work_thread_executor.h"

#include <system_error>

#include "debug_router/native/log/logging.h"

namespace debugrouter {
namespace base {

WorkThreadExecutor::WorkThreadExecutor()
    : is_shut_down(false), alive_flag(std::make_shared<bool>(true)) {}

bool WorkThreadExecutor::TryInit(int32_t* error_code,
                                 std::string* error_message) {
  std::lock_guard<std::mutex> lock(task_mtx);
  if (worker) {
    return true;
  }

#if __cpp_exceptions >= 199711L
  try {
#endif
    worker = std::make_unique<std::thread>([this]() { run(); });
    return true;
#if __cpp_exceptions >= 199711L
  } catch (const std::system_error& e) {
    if (error_code) {
      *error_code = static_cast<int32_t>(e.code().value());
    }
    if (error_message) {
      *error_message = e.what();
    }
    LOGE("WorkThreadExecutor::TryInit failed with system_error: "
         << e.code().value() << ", " << e.what());
    return false;
  } catch (const std::exception& e) {
    if (error_code) {
      *error_code = -1;
    }
    if (error_message) {
      *error_message = e.what();
    }
    LOGE("WorkThreadExecutor::TryInit failed with exception: " << e.what());
    return false;
  }
#endif
}

void WorkThreadExecutor::init() {
  int32_t error_code = 0;
  std::string error_message;
  if (!TryInit(&error_code, &error_message)) {
    LOGE("WorkThreadExecutor::init failed, code=" << error_code << ", message="
                                                  << error_message);
  }
}

WorkThreadExecutor::~WorkThreadExecutor() { shutdown(); }

void WorkThreadExecutor::submit(std::function<void()> task) {
  if (is_shut_down) {
    return;
  }
  std::lock_guard<std::mutex> lock(task_mtx);
  if (is_shut_down) {
    return;
  }
  // If worker is not available, try to init again
  if (!worker) {
    try {
      worker = std::make_unique<std::thread>([this]() { run(); });
    } catch (const std::system_error& e) {
      LOGE("WorkThreadExecutor::submit failed to create thread: " << e.what());
      worker.reset();
      return;  // Drop task if we can't create thread
    }
  }
  tasks.push(task);
  cond.notify_one();
}

void WorkThreadExecutor::shutdown() {
  std::shared_ptr<std::thread> worker_ptr;
  {
    std::lock_guard<std::mutex> lock(task_mtx);
    if (is_shut_down) {
      return;
    }
    is_shut_down = true;
    std::queue<std::function<void()>> empty;
    tasks.swap(empty);
    worker_ptr = std::move(worker);  // take ownership of worker
  }
  cond.notify_all();

  if (worker_ptr && worker_ptr->joinable()) {
#if __cpp_exceptions >= 199711L
    try {
#endif
      if (worker_ptr->get_id() != std::this_thread::get_id()) {
        worker_ptr->join();
        LOGI("WorkThreadExecutor::shutdown worker->join() success.");
      } else {
        worker_ptr->detach();
        LOGI("WorkThreadExecutor::shutdown worker->detach() success.");
      }
#if __cpp_exceptions >= 199711L
    } catch (const std::exception& e) {
      LOGE("WorkThreadExecutor::shutdown worker->detach() failed, "
           << e.what());
    }
#endif
  }
  LOGI("WorkThreadExecutor::shutdown success.");
}

void WorkThreadExecutor::run() {
  std::weak_ptr<bool> weak_flag = alive_flag;
  while (true) {
    auto flag = weak_flag.lock();
    if (!flag) {
      break;
    }
    if (is_shut_down) {
      break;
    }
    std::unique_lock<std::mutex> lock(task_mtx);
    cond.wait(lock, [this] { return !tasks.empty() || is_shut_down; });
    if (is_shut_down) {
      break;
    }
    if (!tasks.empty()) {
      auto task = tasks.front();
      tasks.pop();
      lock.unlock();
      if (is_shut_down) {
        break;
      }
      task();
      LOGI("WorkThreadExecutor::run task() success.");
    }
  }
}

}  // namespace base
}  // namespace debugrouter
