#pragma once
#include <string>

#include "itemTypes.h"

class Hero;

// предметы разделенные на грейды редкости и типы

// enum class ItemHeroAction { use, equip, drop, maxHeroAction };

// enum class ItemEffect {
//   restoreHp,
//   addMaxHp,
//   addMaxStrg,
//   addMaxAgility,
//   addArmor,
//   maxItemAction
// };

// std::vector<ItemHeroAction> getItemActions(ItemTypes type) {
//   switch (type) {
//     case ItemTypes::armor:
//     case ItemTypes::ring:
//     case ItemTypes::amulet:
//       return {ItemHeroAction::equip, ItemHeroAction::drop};
//     case ItemTypes::potion:
//       return {ItemHeroAction::use, ItemHeroAction::drop};
//       ;
//     default:
//       return {ItemHeroAction::drop};
//       ;
//   }
// }

class Item {
 protected:
  int m_id;
  std::string m_name;
  std::string m_description;
  ItemType m_type;
  bool m_stackable;
  int m_maxStack;
  ItemRarity m_rarity;
  int m_cost;

 public:
  Item(int id, std::string name, std::string description, ItemType type,
       bool stackable, int maxStack = 1, ItemRarity rarity = ItemRarity::Common,
       int cost = 1)
      : m_id{id},
        m_name{std::move(name)},
        m_description{std::move(description)},
        m_type{type},
        m_stackable{stackable},
        m_maxStack{stackable ? maxStack : 1},
        m_rarity{rarity},
        m_cost{cost} {};

  virtual ~Item() = default;

  int getId() const { return m_id; }
  const std::string& getName() const { return m_name; }
  const std::string& getDescription() const { return m_description; }
  ItemType getType() const { return m_type; }
  bool isStackable() const { return m_stackable; }
  int getMaxStack() const { return m_maxStack; }
  ItemRarity getRarity() const { return m_rarity; }
  int getCost() const { return m_cost; }

  virtual bool isConsumable() const { return false; }
  virtual bool isEquipable() const { return false; }

  virtual bool use(Hero& player) = 0;
};

class ItemConsumable : public Item {
 private:
 public:
  using Item::Item;
  virtual ~ItemConsumable() = default;

  bool isConsumable() const override { return true; }
  virtual void consume(Hero& player, int itemId) = 0;

  bool use(Hero& player) override {
    consume(player, m_id);
    return true;
  }
};

class ItemEquipable : public Item {
 private:
 public:
  ItemEquipable(int id, std::string name, std::string description,
                ItemType type, bool stackable = false,
                ItemRarity rarity = ItemRarity::Common, int cost = 1)
      : Item(id, name, description, type, stackable, 1, rarity, cost) {};

  virtual ~ItemEquipable() = default;

  bool isEquipable() const override { return true; }
  virtual bool equip(Hero& player) = 0;

  bool use(Hero& player) override { return equip(player); }
};