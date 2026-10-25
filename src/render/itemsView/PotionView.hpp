#pragma once

#include <string>

#include "../../random.h"
#include "../TextureManager.hpp"

class PotionView {
 private:
  const int width_ = 160;
  const int height_ = 128;
  const int cols = width_ / 32;
  const int rows = height_ / 32;

 public:
  void Load();
  void Unload();

  Atlas potions;

  Rectangle potionSmallRec;
  Rectangle potionMediumRec;
  Rectangle potionStrongRec;

  //   std::unordered_map<std::string, Atlas> potions;
};