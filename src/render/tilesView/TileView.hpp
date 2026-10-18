#pragma once

#include <vector>

#include "../../random.h"
#include "../TextureManager.hpp"

class TileView {
 private:
  std::vector<Atlas> tiles;
  const int width_ = 13000;
  const int height_ = 8000;
  const int cols = width_ / 64;
  const int rows = height_ / 64;
  std::vector<int> texture;
  // std::unordered_map<std::pair<int, int>, Atlas> texture;

 public:
  void Load();
  void Unload();

  void Draw(float startX, float startY);
};
