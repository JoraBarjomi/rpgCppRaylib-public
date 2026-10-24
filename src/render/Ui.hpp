#pragma once
#include <algorithm>
#include <cmath>

#include "raylib.h"

namespace ui {

inline constexpr Color kPanel{24, 18, 18, 240};
inline constexpr Color kBorder{110, 90, 60, 255};
inline constexpr Color kTitle{230, 200, 140, 255};
inline constexpr Color kText{225, 220, 210, 255};
inline constexpr Color kDim{150, 138, 120, 255};

inline void Panel(Rectangle r, int S) {
  DrawRectangle((int)r.x + 3 * S, (int)r.y + 3 * S, (int)r.width, (int)r.height,
                Fade(BLACK, 0.5f));
  DrawRectangleRec(r, kPanel);
  DrawRectangleLinesEx(r, 2, kBorder);
  DrawRectangleLinesEx(
      {r.x + 4.0f * S, r.y + 4.0f * S, r.width - 8.0f * S, r.height - 8.0f * S},
      1, Fade(kBorder, 0.35f));
}

inline void Text(const Font& f, const char* t, float x, float y, float size,
                 Color c) {
  DrawTextEx(f, t, {std::floor(x) + 1, std::floor(y) + 1}, size, 1,
             Fade(BLACK, 0.8f));
  DrawTextEx(f, t, {std::floor(x), std::floor(y)}, size, 1, c);
}

inline void TextFading(const Font& f, const char* t, float x, float y,
                       float size, Color c, float alpha) {
  DrawTextEx(f, t, {std::floor(x) + 1, std::floor(y) + 1}, size, 1,
             Fade(BLACK, 0.4f * alpha));
  DrawTextEx(f, t, {std::floor(x), std::floor(y)}, size, 1, Fade(c, alpha));
}

inline void Header(const Font& f, const char* t, float x, float& y, float w,
                   float size, int S) {
  Text(f, t, x, y, size, kTitle);
  y += size + 3.0f * S;
  DrawRectangle((int)x, (int)y, (int)w, S, Fade(kBorder, 0.8f));
  y += 5.0f * S;
}

inline void Row(const Font& f, const char* label, const char* value, float x,
                float& y, float w, float size, int S,
                Color valueColor = kText) {
  Text(f, label, x, y, size, kDim);
  float vw = MeasureTextEx(f, value, size, 1).x;
  Text(f, value, x + w - vw, y, size, valueColor);
  y += size + 4.0f * S;
}

inline void Bar(float x, float& y, float w, float h, float ratio, Color fill,
                int S) {
  ratio = std::clamp(ratio, 0.0f, 1.0f);
  DrawRectangle((int)x - 1, (int)y - 1, (int)w + 2, (int)h + 2, BLACK);
  DrawRectangle((int)x, (int)y, (int)w, (int)h, {35, 25, 45, 255});
  DrawRectangle((int)x, (int)y, (int)(w * ratio), (int)h, fill);
  DrawRectangle((int)x, (int)y, (int)(w * ratio), 1,
                Fade(WHITE, 0.3f));  // блик
  y += h + 4.0f * S;
}

inline bool Button(const Font& f, const char* label, Rectangle r, int S) {
  Vector2 m = GetMousePosition();
  bool hover = CheckCollisionPointRec(m, r);
  bool down = hover && IsMouseButtonDown(MOUSE_BUTTON_LEFT);

  Color bg = down    ? Color{35, 25, 25, 255}
             : hover ? Color{85, 62, 50, 255}
                     : Color{52, 38, 36, 255};

  DrawRectangle((int)r.x + 2 * S, (int)r.y + 2 * S, (int)r.width, (int)r.height,
                Fade(BLACK, 0.4f));
  DrawRectangleRec(r, bg);
  DrawRectangleLinesEx(r, 2, hover ? kTitle : kBorder);

  float fs = 9.0f * S;
  Vector2 sz = MeasureTextEx(f, label, fs, 1);
  float ty = r.y + (r.height - sz.y) / 2 + (down ? S : 0);
  Text(f, label, r.x + (r.width - sz.x) / 2, ty, fs, hover ? kTitle : kText);

  return hover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}

}  // namespace ui