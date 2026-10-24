#pragma once
#include <algorithm>
#include <cmath>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../../random.h"
#include "../../render/TextureManager.hpp"
#include "../unit/Unit.h"
#include "raylib.h"

class SkeletView;

enum class SkeletState {
  Idle,
  Run,
  Attack,
  Hurt,
  Dead,
};

class Skelet : public Unit {
 public:
  Skelet(int lvl, int totalExp, int hp, int armor, int atk, int expForKill,
         int strength, int agility);
  ~Skelet() = default;

  void Update(float dt, Vector2 dir, const std::vector<Rectangle>& obstacles,
              Vector2 heroPos);

  SkeletState getState() const { return state_; }
  void setState(SkeletState state) { state_ = state; }

  Vector2 getPos() const { return pos_; }
  Facing getFacing() const { return facing_; }

  void setSpeed(float speed) { speed_ = speed; }

  void recalcStats();

  static Skelet createSkelet(int lvl = 0);

  int takeDamage(int dmg) override;

  bool hitApplied() const { return hit_applied_; }

  void markHitApplied() { hit_applied_ = true; }

  void setPos(float x, float y);

  float getHurtTime() const { return hurt_timer_; }

  int getMaxHp() const { return max_hp_; }

  Rectangle getHitbox() const;

  Rectangle getRangebox() const;

  void OnAnimationFinished();

  bool tryMoveX(float step, const std::vector<Rectangle>& obstacles);
  bool tryMoveY(float step, const std::vector<Rectangle>& obstacles);

  mutable float hp_ghost_ = 1.0f;

 protected:
  int max_hp_;
  int total_exp_;
  int atrb_points_;
  int gold_;

 private:
  SkeletState state_ = SkeletState::Idle;
  Vector2 pos_ = {6500, 4200};
  float speed_{150.0f};
  Facing facing_ = Facing::DOWN;
  std::unordered_map<SkeletState, std::vector<SkeletState>> transitions_;

  bool hit_applied_ = false;

  float timer_ = 0.0f;
  Vector2 rand_dir_;

  float attack_cooldown_ = 0.0f;

  float hurt_timer_ = 0.0f;
};

