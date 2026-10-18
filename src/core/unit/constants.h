#pragma once

struct StatRange {
  int min;
  int max;
};

constexpr int pointsPerLvl{10};

struct UnitGrowth {
  double strengthPercent;
  double hpPerStrength;
  double armorPerAgility;
  double atkPerStrength;
};

struct UnitBaseStats {
  StatRange hp;
  StatRange armor;
  StatRange atk;
  StatRange strg;
  StatRange aglt;
  int expForKill;
  UnitGrowth growth;
};

namespace UnitStats {
inline constexpr UnitBaseStats wolf{.hp{1, 10},
                                    .armor{0, 2},
                                    .atk{1, 5},
                                    .strg{1, 4},
                                    .aglt{2, 5},
                                    .expForKill{50},
                                    .growth{.strengthPercent{6.0 / 10.0},
                                            .hpPerStrength{0.25},
                                            .armorPerAgility{0.25},
                                            .atkPerStrength{0.15}}};

inline constexpr UnitBaseStats goblin{.hp{1, 8},
                                      .armor{0, 5},
                                      .atk{1, 6},
                                      .strg{1, 3},
                                      .aglt{1, 6},
                                      .expForKill{75},

                                      .growth{.strengthPercent{3.0 / 10.0},
                                              .hpPerStrength{0.5},
                                              .atkPerStrength{0.4}}};

inline constexpr UnitBaseStats orc{.hp{10, 20},
                                   .armor{1, 8},
                                   .atk{5, 15},
                                   .strg{5, 15},
                                   .aglt{3, 9},
                                   .expForKill{300},

                                   .growth{.strengthPercent{8.0 / 10.0},
                                           .hpPerStrength{1.25},
                                           .armorPerAgility{0.5},
                                           .atkPerStrength{0.85}}};

}  // namespace UnitStats
