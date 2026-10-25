#pragma once
#include <string_view>
#include <variant>

enum class ItemRarity { Common, Uncommon, Rare, Epic, Legendary };

enum class ItemType { Weapon, Armor, Ring, Amulet, Potion, Junk, maxItemType };

struct PotionStats {
  int healAmount;
};

struct WeaponStats {
  int attackBonus;
};

struct ArmorStats {
  int armorBonus;
};

struct AccessoryStats {
  int attackBonus;
  int armorBonus;
  int strgBonus;
  int agltyBonus;
};

struct ItemTemplate {
  int id;
  std::string_view name;
  std::string_view description;
  ItemType type;
  bool stackable;
  int maxStack;
  ItemRarity rarity;
  int cost;
  std::variant<PotionStats, WeaponStats, ArmorStats, AccessoryStats> stats;
};