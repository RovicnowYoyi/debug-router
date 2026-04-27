// Copyright 2023 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#include "debug_router/native/socket/socket_server_type.h"
#ifdef _WIN32
#include "debug_router/native/socket/win/socket_server_win.h"
#else
#include "debug_router/native/socket/posix/socket_server_posix.h"
#endif
#include <chrono>
#include <system_error>

#include "debug_router/native/core/util.h"
#include "debug_router/native/thread/debug_router_executor.h"

namespace debugrouter {
namespace socket_server {

std::shared_ptr<SocketServer> SocketServer::CreateSocketServer(
    const std::shared_ptr<SocketServerConnectionListener> &listener) {
#ifdef _WIN32
  return std::make_shared<SocketServerWin>(listener);
#else
  return std::make_shared<SocketServerPosix>(listener);
#endif
}

SocketServer::SocketServer(
    const std::shared_ptr<SocketServerConnectionListener> &listener)
    : listener_(listener), usb_client_(nullptr) {}

bool SocketServer::Send(const std::string &message) {
  if (!usb_client_) {
    LOGI("SocketServerApi Send: client is null.");
    return false;
  }
  return usb_client_->Send(message);
}

void SocketServer::HandleOnOpenStatus(std::shared_ptr<UsbClient> client,
                                      int32_t code, const std::string &reason) {
  thread::DebugRouterExecutor::GetInstance().Post([=]() {
    std::shared_ptr<UsbClient> old_client_ = usb_client_;
    LOGI("SocketServerApi OnOpen: replace old client.");
    if (old_client_) {
      LOGI("SocketServerApi HandleOnOpenStatus: stop old client.");
      old_client_->Stop();
    }
    usb_client_ = client;
    if (auto listener = listener_.lock()) {
      listener->OnStatusChanged(kConnected, code, reason);
    }
  });
}

void SocketServer::HandleOnMessageStatus(std::shared_ptr<UsbClient> client,
                                         const std::string &message) {
  thread::DebugRouterExecutor::GetInstance().Post([=]() {
    if (!usb_client_ || usb_client_ != client) {
      LOGI("SocketServerApi OnMessage: client is null or not match.");
      return;
    }
    if (auto listener = listener_.lock()) {
      listener->OnMessage(message);
    }
  });
}

void SocketServer::HandleOnCloseStatus(std::shared_ptr<UsbClient> client,
                                       ConnectionStatus status, int32_t code,
                                       const std::string &reason) {
  thread::DebugRouterExecutor::GetInstance().Post([=]() {
    if (!usb_client_ || usb_client_ != client) {
      LOGI(
          "SocketServerApi OnClose: curr client is null or not match, stop "
          "error client.");
      client->Stop();
      if (auto listener = listener_.lock()) {
        listener->OnStatusChanged(status, code, reason);
      }
      return;
    }
    LOGI("SocketServerApi HandleOnCloseStatus: close curr client for OnClose.");
    usb_client_->Stop();
    usb_client_ = nullptr;
    if (auto listener = listener_.lock()) {
      listener->OnStatusChanged(status, code, reason);
    }
  });
}

void SocketServer::HandleOnErrorStatus(std::shared_ptr<UsbClient> client,
                                       ConnectionStatus status, int32_t code,
                                       const std::string &reason) {
  thread::DebugRouterExecutor::GetInstance().Post([=]() {
    if (!usb_client_ || usb_client_ != client) {
      LOGI(
          "SocketServerApi OnError: client is null or not match, stop error "
          "client.");
      client->Stop();
      return;
    }
    LOGI("SocketServerApi HandleOnErrorStatus: close curr client for OnError.");
    usb_client_->Stop();
    usb_client_ = nullptr;
    if (auto listener = listener_.lock()) {
      listener->OnStatusChanged(status, code, reason);
    }
  });
}

void SocketServer::NotifyInit(int32_t code, const std::string &info) {
  thread::DebugRouterExecutor::GetInstance().Post([=]() {
    if (auto listener = listener_.lock()) {
      listener->OnInit(code, info);
    }
  });
}

void SocketServer::setEnableServer(bool enable) {
  LOGI("SocketServer::setEnableServer:" << enable);
  // notify only when transition from false to true
  if (!is_running_.exchange(enable, std::memory_order_relaxed) && enable) {
    running_condition_.notify_one();
  }
}

void SocketServer::StartServer() { setEnableServer(true); }

void SocketServer::StopServer() {
  setEnableServer(false);
  // Close socket if it's valid
  if (socket_fd_ != kInvalidSocket) {
#ifdef _WIN32
    shutdown(socket_fd_, SD_BOTH);
#else
    shutdown(socket_fd_, SHUT_RDWR);
#endif
  }

  Close();
  if (usb_client_) {
    usb_client_->Stop();
  }
  if (temp_usb_client_) {
    temp_usb_client_->Stop();
  }
}

void SocketServer::ThreadFunc(std::shared_ptr<SocketServer> socket_server) {
  int count = 0;
  while (true) {
    {
      std::unique_lock lock(socket_server->running_mutex_);
      socket_server->running_condition_.wait(lock, [=]() {
        return socket_server->is_running_.load(std::memory_order_relaxed) ==
               true;
      });
    }
    LOGI("Init start:" << count);
#if __cpp_exceptions >= 199711L
    try {
#endif
      socket_server->Start();
#if __cpp_exceptions >= 199711L
    } catch (const std::exception &e) {
      socket_server->NotifyInit(
          -1, std::string("SocketServer::Start exception: ") + e.what());
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    } catch (...) {
      socket_server->NotifyInit(-1, "SocketServer::Start unknown exception");
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
#endif
    count++;
  }
}

void SocketServer::Init() {
#if __cpp_exceptions >= 199711L
  try {
#endif
    std::thread listen_thread(ThreadFunc, shared_from_this());
    listen_thread.detach();
#if __cpp_exceptions >= 199711L
  } catch (const std::system_error &e) {
    NotifyInit(static_cast<int32_t>(e.code().value()),
               std::string("SocketServer::Init failed: ") + e.what());
  } catch (const std::exception &e) {
    NotifyInit(-1, std::string("SocketServer::Init failed: ") + e.what());
  }
#endif
}

// close server socket
void SocketServer::Close() {
  LOGI("SocketServer::Close server socket_fd_:" << socket_fd_);
  CloseSocket(socket_fd_);
  socket_fd_ = kInvalidSocket;
}

void SocketServer::Disconnect() {
  thread::DebugRouterExecutor::GetInstance().Post([=]() {
    if (usb_client_) {
      LOGI("SocketServerApi Disconnect: stop curr client.");
      usb_client_->Stop();
      usb_client_ = nullptr;
    }
  });
}

SocketServer::~SocketServer() {
  LOGI("SocketServer::~SocketServer");
  if (usb_client_) {
    usb_client_->Stop();
  }
  if (temp_usb_client_) {
    temp_usb_client_->Stop();
  }
  Close();
}

}  // namespace socket_server
}  // namespace debugrouter
