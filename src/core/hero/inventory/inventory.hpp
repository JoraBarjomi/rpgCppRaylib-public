#pragma once
#include <algorithm>
#include <memory>
#include <vector>

#include "../../item/item.h"

class Hero;

struct InventorySlot {
  std::shared_ptr<Item> item;
  int quantity;
};

enum class EquipSlots {
  Weapon,
  Helmet,
  ChestPlate,
  Leggins,
  Gloves,
  Boots,
  Ring1,
  Ring2,
  Amulet
};

class Inventory {
 private:
  struct EquipmentSlots {
    std::shared_ptr<ItemEquipable> eq_slots[9];
  };
  EquipmentSlots m_equipment;

  std::vector<InventorySlot> m_slots;
  int m_capacity;

 public:
  explicit Inventory(int capacity = 5) : m_capacity{capacity} {};

  bool addItem(const std::shared_ptr<Item>& item, int quantity = 1);
  bool removeItem(int itemId, int quantity = 1);
  bool useItem(int itemId, Hero& player);

  bool useSlot(int index, Hero& player);
  void removeFromSlot(int index, int q = 1);

  std::shared_ptr<Item> findItem(int itemId) const;

  bool equipItem(int itemId, EquipSlots targetSlot);

  bool unequipItem(EquipSlots targetSlot);

  int getItemCount(int itemId) const;

  bool hasSpace() const;
  bool isEmpty() const;

  const std::vector<InventorySlot>& getSlots() const { return m_slots; }
};