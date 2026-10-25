#pragma once
#include <string>

struct ItemView {
  std::string name;
  int iconIndex = -1;
  int count = 1;
  bool empty() const { return iconIndex < 0; }
};
