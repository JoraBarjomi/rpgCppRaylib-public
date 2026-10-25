#include "PotionView.hpp"

void PotionView::Load() {
  potions = LoadAtlas("Items/Icons_Potions.png", 32);

  potionSmallRec = CellRect(potions, 3);
  potionMediumRec = CellRect(potions, 4);
  potionStrongRec = CellRect(potions, 5);
}

void PotionView::Unload() { UnloadTexture(potions.tex); }
