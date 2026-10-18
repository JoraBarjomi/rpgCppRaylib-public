#include "TileView.hpp"

void TileView::Load() {
  tiles.push_back(LoadAtlas("Terrain/Tileset/Tilemap_color1.png", 64));
  tiles.push_back(LoadAtlas("Terrain/Tileset/Tilemap_color2.png", 64));
  tiles.push_back(LoadAtlas("Terrain/Tileset/Tilemap_color4.png", 64));
  //   tiles.push_back(LoadAtlas("Terrain/Tileset/Tilemap_color3.png", 64));
  //   tiles.push_back(LoadAtlas("Terrain/Tileset/Tilemap_color5.png", 64));

  texture.resize(rows * cols);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int rnd = Random::get(0, 100);
      if (rnd <= 85)
        texture[i * cols + j] = 0;
      else if (rnd <= 98)
        texture[i * cols + j] = 1;
      else
        texture[i * cols + j] = 2;
    }
  }
}

void TileView::Unload() {
  UnloadTexture(tiles[0].tex);
  UnloadTexture(tiles[1].tex);
  UnloadTexture(tiles[2].tex);
  // UnloadTexture(tiles[3].tex);
  //   UnloadTexture(tiles[4].tex);
}

void TileView::Draw(float startX, float startY) {
  Rectangle src = CellRect(tiles[0], 10);
  for (float i = 0; i < rows; i++) {
    for (float j = 0; j < cols; j++) {
      int ind = texture[i * cols + j];  // which tile in this cell
      DrawTextureRec(tiles[ind].tex, src, {j * 64 + startX, i * 64 + startY},
                     WHITE);  // draw tile with current tile texture generated
                              // before. 64x64 tile size.
    }
  }
}