#include "Unit.h"

void Unit::setHp(int hp) { hp_ = std::max(0, hp); }
void Unit::setArmor(int armor) { armor_ = std::max(0, armor); }
void Unit::plusArmor(int armor) { armor_ += armor; }
void Unit::minusArmor(int armor) {
  if (armor_ > 0) {
    armor_ -= armor;
  }
}

void Unit::plusAttackDmg(int attackDmgPoints) { atk_ += attackDmgPoints; }
void Unit::minusAttackDmg(int attackDmgPoints) {
  if (atk_ > 0) {
    atk_ -= attackDmgPoints;
  }
}

void Unit::plusStrength(int strg) { atrb_.strength_ += strg; }
void Unit::minusStrength(int strg) {
  if (atrb_.strength_ > 0) {
    atrb_.strength_ -= strg;
  }
}

void Unit::plusAgility(int agility) { atrb_.agility_ += agility; }
void Unit::minusAgility(int agility) {
  if (atrb_.agility_ > 0) {
    atrb_.agility_ -= agility;
  }
}

const bool Unit::isDead() const { return (hp_ <= 0 ? true : false); }

AttackOutcome Unit::attackUnit(Unit& enemy) {
  AttackOutcome outcome;

  // if (enemy.isDead()) return outcome;
  // outcome.isPenetratedArmor = (atk_ >= enemy.getArmor());
  // if (!outcome.isPenetratedArmor) return outcome;
  outcome.dmgDealt = std::max(0, atk_);

  outcome.expGained = enemy.takeDamage(outcome.dmgDealt);
  if (enemy.isDead()) {
    outcome.isEnemyDead = true;
    // outcome.expGained = enemy.getExpForKill();
  }
  return outcome;
}

int Unit::takeDamage(int dmg) {
  int real = std::max(1, dmg - armor_);
  hp_ = std::max(0, hp_ - real);
  return real;
}
