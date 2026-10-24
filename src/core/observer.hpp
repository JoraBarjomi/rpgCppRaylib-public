#pragma once
#include <functional>
#include <vector>

template <typename... Args>
class Signal {
 public:
  using Slot = std::function<void(Args...)>;

  void Connect(Slot slot) { m_slots.push_back(std::move(slot)); }

  void Emit(Args... args) {
    for (const auto& slot : m_slots) {
      slot(args...);
    }
  }

 private:
  std::vector<Slot> m_slots;
};
