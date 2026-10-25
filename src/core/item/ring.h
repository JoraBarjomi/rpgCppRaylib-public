#pragma once
#include "../hero/Hero.hpp"
#include "../hero/inventory/inventory.hpp"
#include "../unit/Unit.h"
#include "item.h"
#include "itemTypes.h"

class Ring : public ItemEquipable {
 private:
  AccessoryStats m_stats;

 public:
  Ring() = default;
  Ring(const ItemTemplate& t)
      : ItemEquipable(t.id, std::string(t.name), std::string(t.description),
                      t.type, t.stackable, t.rarity, t.cost),
        m_stats(std::get<AccessoryStats>(t.stats)) {};
  ~Ring() = default;

  bool equip(Hero& player) override;
  bool unequip(Hero& player);
};