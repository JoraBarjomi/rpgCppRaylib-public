#pragma once
#include <vector>

#include "../../core/hero/Hero.hpp"
#include "../TextureManager.hpp"

class HeroView {
 public:
  void Load();
  bool Update(float dt, const Hero& hero);
  void Draw(const Hero& hero) const;
  void Unload();

  static Animation MakeAnim(const char* path, int frames, float frameTime,
                            bool loop = true);
  int getFacingDir() { return facing_; }

  int CurrentFrame() const { return (*current_)[0].frame; }

 private:
  using Layers = std::vector<Animation>;
  Layers idle_, run_, attack_, hurt_, death_;
  Layers* current_ = &idle_;
  int facing_ = Facing::DOWN;
};
