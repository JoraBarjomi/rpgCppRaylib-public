#include "inventory.hpp"

bool Inventory::addItem(const std::shared_ptr<Item>& item, int quantity) {
  if (item->isStackable()) {
    for (auto& slot : m_slots) {
      if (slot.item && slot.item->getId() == item->getId() &&
          slot.quantity < slot.item->getMaxStack()) {
        int canAdd = slot.item->getMaxStack() - slot.quantity;
        int toAdd = std::min(canAdd, quantity);
        slot.quantity += toAdd;
        quantity -= toAdd;
        if (quantity == 0) return true;
      }
    }
  }

  while (quantity > 0) {
    if (static_cast<int>(m_slots.size()) >= m_capacity) {
      return false;
    }
    int stackSize =
        item->isStackable() ? std::min(quantity, item->getMaxStack()) : 1;
    m_slots.push_back(InventorySlot{item, stackSize});
    quantity -= stackSize;
  }
  return true;
}

bool Inventory::removeItem(int itemId, int quantity) {
  for (auto it = m_slots.begin(); it != m_slots.end();) {
    if (it->item->getId() == itemId) {
      if (it->quantity > quantity) {
        it->quantity -= quantity;
        return true;
      }
      quantity -= it->quantity;
      it = m_slots.erase(it);
      if (quantity == 0) return true;
    } else {
      ++it;
    }
  }
  return quantity == 0;
}

bool Inventory::useItem(int itemId, Hero& player) {
  for (size_t i = 0; i < m_slots.size(); ++i) {
    if (!m_slots[i].item || m_slots[i].item->getId() != itemId) continue;

    if (m_slots[i].item->isConsumable()) {
      auto* c{static_cast<ItemConsumable*>(m_slots[i].item.get())};
      c->use(player);
      removeItem(itemId, 1);
      return true;
    }
    if (m_slots[i].item->isEquipable()) {
      auto* e{static_cast<ItemEquipable*>(m_slots[i].item.get())};
      bool isEquiped = e->use(player);
      if (isEquiped) {
        removeItem(itemId, 1);
        return true;
      }
      return false;
    }
  }
  return false;
}

std::shared_ptr<Item> Inventory::findItem(int itemId) const {
  for (const auto& slot : m_slots) {
    if (slot.item && slot.item->getId() == itemId) return slot.item;
  }
  return nullptr;
}

bool Inventory::equipItem(int itemId, EquipSlots targetSlot) {
  auto item = std::dynamic_pointer_cast<ItemEquipable>(findItem(itemId));
  if (!item) return false;
  if (m_equipment.eq_slots[static_cast<int>(targetSlot)]) return false;

  m_equipment.eq_slots[static_cast<int>(targetSlot)] = item;
  removeItem(itemId, 1);
  return true;
}

bool Inventory::unequipItem(EquipSlots targetSlot) {}

int Inventory::getItemCount(int itemId) const {
  int total = 0;
  for (const auto& slot : m_slots) {
    if (slot.item->getId() == itemId) total += slot.quantity;
  }
  return total;
}

bool Inventory::hasSpace() const {
  return static_cast<int>(m_slots.size()) < m_capacity;
}

bool Inventory::isEmpty() const { return m_slots.size() == 0; }
