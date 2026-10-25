#include "itemDb.h"

const std::unordered_map<int, ItemTemplate> ITEM_DB = []() {
  std::unordered_map<int, ItemTemplate> db;
  std::array<ItemTemplate, 10> ITEMS = {{
      {1, "Small Healing Potion", "Heal 10 hp", ItemType::Potion, true, 5,
       ItemRarity::Common, 10, PotionStats{10}},
      {2, "Healing Potion", "Heal 20 hp", ItemType::Potion, true, 5,
       ItemRarity::Uncommon, 20, PotionStats{20}},
      {3, "Strong Healing Potion", "Heal 30 hp", ItemType::Potion, true, 5,
       ItemRarity::Rare, 30, PotionStats{30}},
      {4, "Pure Healing Potion", "Heal 40 hp", ItemType::Potion, true, 5,
       ItemRarity::Epic, 40, PotionStats{40}},
      {5, "Healing Elixir", "Heal 100 hp", ItemType::Potion, true, 5,
       ItemRarity::Legendary, 80, PotionStats{100}},
      {6, "Iron Ring", "Gain +5 armor", ItemType::Ring, false, 1,
       ItemRarity::Common, 20, AccessoryStats{.armorBonus = 5}},
      {7, "Magic Ring", "Gain +100 attack dmg", ItemType::Ring, false, 1,
       ItemRarity::Legendary, 120, AccessoryStats{.attackBonus = 100}},
      {8, "Iron Sword", "Gain +20 attack dmg", ItemType::Weapon, false, 1,
       ItemRarity::Common, 60, AccessoryStats{.attackBonus = 100}},
      {9, "Iron Armor", "Gain +40 armor", ItemType::Armor, false, 1,
       ItemRarity::Common, 100, AccessoryStats{.attackBonus = 100}},
      {10, "Amulet of Strength", "Gain +20 strength", ItemType::Amulet, false,
       1, ItemRarity::Common, 125, AccessoryStats{.attackBonus = 100}},
  }};

  for (const auto& item : ITEMS) db[item.id] = item;
  return db;
}();

std::unique_ptr<Item> createItem(int id) {
  auto it{ITEM_DB.find(id)};
  if (it == ITEM_DB.end()) return nullptr;

  const auto& t = it->second;

  return std::visit(
      [&](const auto& stats) -> std::unique_ptr<Item> {
        using StatsType = std::decay_t<decltype(stats)>;
        if constexpr (std::is_same_v<StatsType, PotionStats>) {
          return std::make_unique<Potion>(
              t.id, std::string(t.name), std::string(t.description), t.type,
              t.stackable, t.maxStack, t.rarity, t.cost, stats.healAmount);
        } else if constexpr (std::is_same_v<StatsType, AccessoryStats>) {
          return std::make_unique<Ring>(t);
        }
        return nullptr;
      },
      t.stats);
}

std::unique_ptr<Item> createRndItem() {
  auto id{Random::get(1, 10)};
  auto it{ITEM_DB.find(id)};
  if (it == ITEM_DB.end()) return nullptr;

  const auto& t = it->second;
  const auto type = t.type;  // Weapon, Armor, Ring, Amulet, Potion

  return std::visit(
      [&](const auto& stats) -> std::unique_ptr<Item> {
        using StatsType = std::decay_t<decltype(stats)>;
        if constexpr (std::is_same_v<StatsType, PotionStats>) {
          return std::make_unique<Potion>(
              t.id, std::string(t.name), std::string(t.description), t.type,
              t.stackable, t.maxStack, t.rarity, t.cost, stats.healAmount);
        } else if constexpr (std::is_same_v<StatsType, AccessoryStats>) {
          switch (type) {
            // case ItemType::Weapon:
            //   return std::make_unique<Ring>(t);
            case ItemType::Ring:
              return std::make_unique<Ring>(t);
              // case ItemType::Ring:
              //   return std::make_unique<Ring>(t);
              // case ItemType::Ring:
              //   return std::make_unique<Ring>(t);
              // case ItemType::Ring:

            default:
              return std::make_unique<Ring>(t);
          }
        }
        return nullptr;
      },
      t.stats);
}