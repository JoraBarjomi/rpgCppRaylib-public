#pragma once
#include <algorithm>
#include <cmath>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../../render/TextureManager.hpp"
#include "../unit/Unit.h"
#include "raylib.h"

class HeroView;

enum class HeroState {
  Idle,
  Run,
  Attack,
  Hurt,
  Dead,
};

class Hero : public Unit {
 public:
  Hero(int lvl, int totalExp, int hp, int armor, int atk, int expForKill,
       int strength, int agility);
  ~Hero() = default;

  void Update(float dt, Vector2 dir, bool is_attack,
              std::vector<Rectangle> obstacles);

  void UpdateFacing(const Camera2D& camera);

  HeroState getState() const { return state_; }
  void setState(HeroState state) { state_ = state; }

  Vector2 getPos() const { return pos_; }
  Facing getFacing() const { return facing_; }

  void setSpeed(float speed) { speed_ = speed; }

  void recalcStats();

  static Hero createHero(int lvl = 0);

  bool hitApplied() const { return hit_applied_; }
  void markHitApplied() { hit_applied_ = true; }

  void takeDamage(int dmg) override;

  void setPlusPos(int x = 0, int y = 0);

  float getHurtTime() const { return hurt_timer_; }

  Rectangle getHitbox() const;

  Rectangle getRangebox() const;

  void OnAnimationFinished();

 protected:
  int max_hp_;
  int total_exp_;
  int atrb_points_;
  int gold_;

 private:
  HeroState state_ = HeroState::Idle;
  Vector2 pos_ = {6500, 4000};
  float speed_{250.0f};
  Facing facing_ = Facing::DOWN;
  std::unordered_map<HeroState, std::vector<HeroState>> transitions_;

  bool hit_applied_ = false;
  float hurt_timer_ = 0.0f;
};
