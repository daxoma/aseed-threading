#pragma once
#include <atomic>

namespace aseed::threading {

struct spin {
  bool try_lock() noexcept {
    auto t = _flag.load(std::memory_order_relaxed);
    if (t == true) return false;
    auto e = false;
    return _flag.compare_exchange_weak(e, true, std::memory_order_acquire);
  }

  void lock() noexcept {
    for (;;) {
      if (try_lock()) break;
      ::Sleep(0);
    }
  }

  void unlock() noexcept { _flag.store(false, std::memory_order_release); }

  spin() noexcept : _flag(false) {}

  spin(const spin&) = delete;
  void operator=(const spin&) = delete;

 private:
  std::atomic<bool> _flag;
};

}  // namespace aseed::threading