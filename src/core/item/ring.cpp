#include "ring.h"

bool Ring::equip(Hero& player) {
  bool isEquiped = player.inventory.equipItem(this->m_id, EquipSlots::Ring1);
  if (!isEquiped) {
    isEquiped = player.inventory.equipItem(this->m_id, EquipSlots::Ring2);
  }
  if (!isEquiped) return false;
  player.plusArmor(m_stats.armorBonus);
  player.plusAttackDmg(m_stats.attackBonus);
  player.plusStrength(m_stats.strgBonus);
  player.plusAgility(m_stats.agltyBonus);

  return true;
}

bool Ring::unequip(Hero& player) {
  player.minusArmor(m_stats.armorBonus);
  player.minusAttackDmg(m_stats.attackBonus);
  player.minusStrength(m_stats.strgBonus);
  player.minusAgility(m_stats.agltyBonus);
  return true;
}
