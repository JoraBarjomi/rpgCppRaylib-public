#pragma once
#include <string>
#include <vector>

#include "../Ui.hpp"
#include "raylib.h"

class Logger {
 private:
  struct Log {
    std::string text;
    Color color;
    float timeLeft;
  };

  std::vector<Log> logs;

 public:
  void AddMessage(const std::string& msg, Color color = RAYWHITE);

  void Update(float dt);

  void Draw(int x = 20, int y = 20, int S = 1) const;
};
