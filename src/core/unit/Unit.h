#pragma once
#include <algorithm>
#include <array>
#include <cmath>

#include "events.h"
#include "raylib.h"

namespace UnitTypes {
enum unitTypes { wolf, goblin, orc, maxUnitTypes };
}

namespace lvlInfo {
constexpr std::array<int, 10> lvlSteps{0,    500,  1000, 1500, 2000,
                                       2500, 3000, 3500, 4000, 4500};
};

class Unit {
 private:
  struct Attributes {
    int strength_;
    int agility_;
  };

 protected:
  int lvl_;
  int exp_total_;
  int hp_;
  int armor_;
  int atk_;
  int expForKill_;
  Attributes atrb_;

 public:
  Unit(int lvl, int expTotal, int hp, int armor, int atk, int expForKill,
       int strength, int agility)
      : lvl_{lvl},
        exp_total_{expTotal},
        hp_{hp},
        armor_{armor},
        atk_{atk},
        expForKill_{expForKill},
        atrb_{strength, agility} {};
  virtual ~Unit() = default;

  virtual void recalcStats() = 0;

  int getLvl() const { return lvl_; }
  int getHp() const { return hp_; }
  int getArmor() const { return armor_; }
  int getAtk() const { return atk_; }
  int getExpForKill() const { return expForKill_; }

  int getExp() const { return exp_total_; }

  int getExpLeft() const {
    return std::abs(getExp() - lvlInfo::lvlSteps[lvl_ + 1]);
  }

  int getStrength() const { return atrb_.strength_; }
  int getAgility() const { return atrb_.agility_; }

  void setHp(int hp);
  void setArmor(int armor);

  void plusArmor(int armor);
  void minusArmor(int armor);

  void plusAttackDmg(int attackDmgPoints);
  void minusAttackDmg(int attackDmgPoints);

  void plusStrength(int strg);
  void minusStrength(int strg);

  void plusAgility(int agility);
  void minusAgility(int agility);

  void plusExp(int exp);

  const bool isDead() const;

  virtual AttackOutcome attackUnit(Unit& enemy);

  virtual int takeDamage(int dmg);
};
