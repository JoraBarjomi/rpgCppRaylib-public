#include "potion.h"

void Potion::consume(Hero& player, int itemId) { player.heal(m_healingPoints); }