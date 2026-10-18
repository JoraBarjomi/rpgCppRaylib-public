#pragma once

struct LvlUpEvent {
  bool isLvlUp{};
  int newLvl{};
  int pointsGained{};
};

struct AttackOutcome {
  int dmgDealt{};
  bool isPenetratedArmor{};
  bool isEnemyDead{};
  int expGained{};
  LvlUpEvent isLvlUp{};
};

struct AllocatePointsEvent {
  int pointsSpent{};
  int pointsLeft{};
};