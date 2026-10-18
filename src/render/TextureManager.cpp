#include "TextureManager.hpp"

Atlas LoadAtlas(const char* path, int cell) {
  Atlas a;
  a.tex = LoadTexture(path);
  SetTextureFilter(a.tex, TEXTURE_FILTER_POINT);
  a.cell = cell;
  a.cols = a.tex.width / cell;  // calc columns frames animation in row
  return a;
}

// return coordinates and size of rectangle on texture
Rectangle CellRect(const Atlas& a, int index) {
  int col = index % a.cols;
  int row = index / a.cols;
  return {(float)(col * a.cell), (float)(row * a.cell), (float)a.cell,
          (float)a.cell};
}

// if animation frame is end set new animation frame
bool Animation::Update(float dt, int last_frame) {
  timer += dt;
  if (timer >= frameTime) {
    timer -= frameTime;
    if (frame < last_frame) {
      frame++;
    } else {
      if (loop) {
        frame = 0;
      } else {
        return true;
      }
    }
  }
  return false;
}

// draw texture simply move frame into new pos
void Animation::Draw(Vector2 pos, float scale, Color tint) const {
  Rectangle src = CellRect(atlas, first + frame);
  Rectangle dst = {pos.x, pos.y, atlas.cell * scale, atlas.cell * scale};
  DrawTexturePro(atlas.tex, src, dst, {0, 0}, 0.0f, tint);
}
