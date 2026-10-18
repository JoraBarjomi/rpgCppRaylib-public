#pragma once
#include <vector>

#include "../../core/skelet/Skelet.hpp"
#include "../../random.h"
#include "../TextureManager.hpp"
#include "raymath.h"

class SkeletView {
 public:
  void Load();
  bool Update(float dt, const Skelet& skelet);
  void Draw(const Skelet& skelet) const;
  void Unload();

  static Animation MakeAnim(const char* path, int frames, float frameTime,
                            bool loop = true);
  int getFacingDir() { return facing_; }

 private:
  using Layers = std::vector<Animation>;
  Layers idle_, run_, attack_, hurt_, death_;
  Layers* current_ = &idle_;
  int facing_ = DOWN;
};
