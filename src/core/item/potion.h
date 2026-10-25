#pragma once
#include "../hero/Hero.hpp"
#include "item.h"

class Potion : public ItemConsumable {
 private:
  int m_healingPoints;

 public:
  Potion() = default;
  Potion(int id, std::string name, std::string description,
         ItemType type = ItemType::Potion, bool stackable = true,
         int maxStack = 5, ItemRarity rarity = ItemRarity::Common, int cost = 1,
         int healingHp = 10)
      : m_healingPoints{healingHp},
        ItemConsumable(id, name, description, type, stackable, maxStack, rarity,
                       cost) {};
  ~Potion() = default;

  void consume(Hero& player, int itemId) override;
};