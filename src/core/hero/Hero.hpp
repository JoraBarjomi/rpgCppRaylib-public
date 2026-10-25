#pragma once
#include <algorithm>
#include <cmath>
#include <thread>
#include <unordered_map>
#include <vector>

#include "../../render/Observer.hpp"
#include "../../render/TextureManager.hpp"
#include "../../render/logger/Logger.hpp"
#include "../unit/Unit.h"
#include "inventory/inventory.hpp"
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
  //
  Signal<int> onHpChanged;
  Signal<int> onMaxHpChanged;
  Signal<int> onExpChanged;
  Signal<int> onLvlUp;
  Signal<int> onAttrPointsChanged;
  Signal<int> onStrengthChanged;
  Signal<int> onAgilityChanged;
  Signal<> onDeath;
  Signal<> onInventoryChanged;
  Signal<int> onGoldChanged;
  Signal<> onStep;
  Signal<> onAttack;
  Signal<> onHitEnemy;
  //

  //
  Inventory inventory{72};
  //

  Hero(int lvl, int totalExp, int hp, int armor, int atk, int expForKill,
       int strength, int agility);
  ~Hero() = default;

  void Update(float dt, Vector2 dir, bool is_attack,
              std::vector<Rectangle> obstacles);

  void UpdateFacing(const Camera2D& camera);

  bool isHavePoints() const { return atrb_points_ > 0; }

  int getPoints() const { return atrb_points_; }

  HeroState getState() const { return state_; }
  void setState(HeroState state) { state_ = state; }

  Vector2 getPos() const { return pos_; }
  Facing getFacing() const { return facing_; }

  void setSpeed(float speed) { speed_ = speed; }

  void recalcStats();

  static Hero createHero(int lvl = 0);

  bool hitApplied() const { return hit_applied_; }
  void markHitApplied() { hit_applied_ = true; }

  int takeDamage(int dmg) override;

  void setPlusPos(int x = 0, int y = 0);

  float getHurtTime() const { return hurt_timer_; }

  int getMaxHp() const { return max_hp_; }

  Rectangle getHitbox() const;

  Rectangle getRangebox() const;

  void OnAnimationFinished();

  float& getHpGhost() const { return hp_ghost_; }

  void plusExp(int exp) {
    exp_total_ += exp;
    onExpChanged.Emit(exp);
  }

  LvlUpEvent isLvlUp();

  void allocateStrength(int points) {
    int spend = std::min(points, atrb_points_);
    atrb_.strength_ += spend;
    atrb_points_ -= spend;
    onStrengthChanged.Emit(atrb_.agility_);
    recalcStats();
  }

  void allocateAgility(int points) {
    int spend = std::min(points, atrb_points_);
    atrb_.agility_ += spend;
    atrb_points_ -= spend;
    onAgilityChanged.Emit(atrb_.agility_);
  }

  void heal(int points);

  void UseItemAt(int clicked);

 protected:
  int max_hp_;
  int atrb_points_ = 0;
  int gold_ = 0;
  int base_hp_ = 4;
  int base_atk_ = 2;

 private:
  HeroState state_ = HeroState::Idle;
  Vector2 pos_ = {6500, 4000};
  float speed_{250.0f};
  Facing facing_ = Facing::DOWN;
  std::unordered_map<HeroState, std::vector<HeroState>> transitions_;

  bool hit_applied_ = false;
  float hurt_timer_ = 0.0f;

  mutable float hp_ghost_ = 1.0f;
  float step_timer_ = 0.0f;
};
