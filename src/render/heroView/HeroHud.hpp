#pragma once
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "../ItemView.hpp"
#include "../Ui.hpp"
#include "raylib.h"

inline void DrawHpBar(int x, int y, int w, int h, int hp, int maxHp,
                      float& ghost, float dt) {
  // DrawText(TextFormat("(%i)", hero.getLvl()), 10, 10, 20, GOLD);

  float ratio = std::clamp((float)hp / maxHp, 0.0f, 1.0f);
  ghost = (ghost > ratio) ? std::max(ratio, ghost - 0.5f * dt) : ratio;

  DrawRectangle(x + 2, y + 2, w, h, Fade(BLACK, 0.4f));
  DrawRectangle(x - 2, y - 2, w + 4, h + 4, {10, 8, 8, 255});
  DrawRectangle(x, y, w, h, {55, 18, 18, 255});
  DrawRectangle(x, y, (int)(w * ghost), h, {235, 160, 60, 255});
  DrawRectangleGradientV(x, y, (int)(w * ratio), h, {235, 70, 60, 255},
                         {150, 25, 25, 255});
  DrawRectangle(x, y, (int)(w * ratio), 2, Fade(WHITE, 0.3f));
  DrawRectangleLinesEx({(float)x - 2, (float)y - 2, (float)w + 4, (float)h + 4},
                       1, {120, 95, 60, 255});

  const char* t = TextFormat("%d / %d", hp, maxHp);
  Vector2 sz = MeasureTextEx(GetFontDefault(), t, h * 0.7f, 1);
  DrawTextEx(GetFontDefault(), t,
             {float(x) + (w - sz.x) / 2.0f, float(y) + (h - sz.y) / 2.0f},
             h * 0.7f, 1, BLACK);
  DrawTextEx(GetFontDefault(), t,
             {float(x) + (w - sz.x) / 2.0f, float(y) + (h - sz.y) / 2.0f},
             h * 0.7f, 1, WHITE);
}

enum class Tab { Inventory, Stats, LvlUp, Restart };

inline int DrawStats(const Hero& hero, int S,
                     const Font& font = GetFontDefault()) {
  const float fs = 9.0 * S;
  const float W = 300.0f * S, H = 130.0f * S;
  const float pad = 12.0f * S, gap = 16.0f * S;
  const float X = std::floor((GetScreenWidth() - (float)W) / 2.0f);
  const float Y = std::floor((GetScreenHeight() - (float)H) / 2.0f);

  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
  ui::Panel({X, Y, W, H}, S);

  const float colW = (W - pad * 2.0f - gap) / 2.0f;
  const float x1 = X + pad, x2 = x1 + colW + gap;
  float y1 = Y + pad, y2 = Y + pad;

  ui::Header(font, "CHARACTER", x1, y1, colW, fs, S);
  const char* lvlText = TextFormat("%i", hero.getLvl());
  ui::Row(font, "Level", lvlText, x1, y1, colW, fs, S);

  const char* expText = TextFormat("%i", hero.getExp());
  ui::Row(font, "Experience", expText, x1, y1, colW, fs, S);

  float exp = (float)hero.getExp(), left = (float)hero.getExpLeft();
  ui::Bar(x1, y1, colW, 4.0f * S, exp / std::max(1.0f, exp + left),
          {160, 90, 230, 255}, S);

  ui::Row(font, "Next level", TextFormat("%d", hero.getExpLeft()), x1, y1, colW,
          fs, S);
  ui::Row(font, "Learning pts.", TextFormat("%d", hero.getPoints()), x1, y1,
          colW, fs, S, hero.getPoints() > 0 ? ui::kTitle : ui::kText);

  ui::Header(font, "ATTRIBUTES", x2, y2, colW, fs, S);
  ui::Row(font, "Strength", TextFormat("%d", hero.getStrength()), x2, y2, colW,
          fs, S);
  ui::Row(font, "Agility", TextFormat("%d", hero.getAgility()), x2, y2, colW,
          fs, S);
  ui::Row(font, "Hitpoints", TextFormat("%d/%d", hero.getHp(), hero.getMaxHp()),
          x2, y2, colW, fs, S);

  ui::Header(font, "PROTECTION", x2, y2, colW, fs, S);
  ui::Row(font, "Armor points", TextFormat("%d", hero.getArmor()), x2, y2, colW,
          fs, S);
  ui::Row(font, "Attack damage", TextFormat("%d", hero.getAtk()), x2, y2, colW,
          fs, S);

  const char* hint = "[C] close";
  float hw = MeasureTextEx(font, hint, fs * 0.8f, 1).x;
  ui::Text(font, hint, X + W - pad - hw, Y + H - pad - fs * 0.8f, fs * 0.8f,
           ui::kDim);

  return 0;
}

inline int DrawLvlUp(Hero& hero, int S, const Font& font = GetFontDefault()) {
  const float fs = 9.0 * S;
  const float W = 300.0f * S, H = 130.0f * S;
  const float pad = 12.0f * S, gap = 4.0f * S;
  const float X = std::floor((GetScreenWidth() - (float)W) / 2.0f);
  const float Y = std::floor((GetScreenHeight() - (float)H) / 2.0f);

  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
  ui::Panel({X, Y, W, H}, S);

  const float colW = (W - pad * 2.0f - gap) / 2.0f;
  const float x1 = X + pad, x2 = x1 + colW + gap;
  float y1 = Y + pad, y2 = Y + pad;

  ui::Header(font, "LVL UP", x1, y1, colW, fs, S);
  ui::Row(font, "Points:", TextFormat("%d", hero.getPoints()), x2, y2, colW, fs,
          S);
  {
    float rowY = y1;
    ui::Row(font, "Strength", TextFormat("%d", hero.getStrength()), x1, y1,
            colW, fs, S);
    if (ui::Button(font, "+1", {x2, rowY - S, 40, 40}, S)) {
      hero.allocateStrength(1);
    }
    y1 += gap;
  }

  {
    float rowY = y1;
    ui::Row(font, "Agility", TextFormat("%d", hero.getAgility()), x1, y1, colW,
            fs, S);
    if (ui::Button(font, "+1", {x2, rowY - S, 40, 40}, S)) {
      hero.allocateAgility(1);
    }
    y1 += gap;
  }

  const char* hint = "[C] close";
  float hw = MeasureTextEx(font, hint, fs * 0.8f, 1).x;
  ui::Text(font, hint, X + W - pad - hw, Y + H - pad - fs * 0.8f, fs * 0.8f,
           ui::kDim);

  return 0;
}

inline int DrawQuitRestartMenu(Hero& hero, int S,
                               const Font& font = GetFontDefault()) {
  const float fs = 9.0 * S;
  const float W = 300.0f * S, H = 130.0f * S;
  const float pad = 12.0f * S, gap = 64.0f * S;
  const float X = std::floor((GetScreenWidth() - (float)W) / 2.0f);
  const float Y = std::floor((GetScreenHeight() - (float)H) / 2.0f);

  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(MAROON, 0.8f));
  //   ui::Panel({X, Y, W, H}, S);

  const float colW = (W - pad * 2.0f - gap) / 2.0f;
  float x1 = X, x2 = x1 + colW + gap;
  float y1 = Y, y2 = Y;

  ui::Header(font, "GAME OVER", x1, y1, colW, fs, S);

  {
    float rowX = x1;
    if (ui::Button(font, "Restart", {rowX, y1 - S, 100, 40}, S)) {
      return 1;
    }
    x1 += gap;
  }
  {
    float rowX = x1;
    if (ui::Button(font, "Quit", {x1, y1 - S, 100, 40}, S)) {
      return 2;
    }
    x1 += gap;
  }

  return 0;
}

inline std::vector<ItemView> MakeInvView(const Hero& hero) {
  std::vector<ItemView> out;
  for (const auto& it : hero.inventory.getSlots()) {
    if (!it.item) {
      out.push_back({});
      continue;
    }
    out.push_back({it.item->getName(), it.item->getId(), it.quantity});
  }
  return out;
}

inline int DrawInventory(const std::vector<ItemView>& slots, int S,
                         const Font& font = GetFontDefault()) {
  constexpr int COLS = 6, ROWS = 12;

  const int cell = 20 * S, gapCell = 2 * S, pad = 8 * S, titleH = 14 * S;
  const float gap = 64.0f * S;
  const float fs = 9.0 * S;

  const int gridW = COLS * cell + (COLS - 1) * gapCell;
  const int gridH = ROWS * cell + (ROWS - 1) * gapCell;

  const float W = gridW + pad * 2, H = gridH + pad * 2 + titleH;

  const float margin = 20.0f * S;
  const float X = GetScreenWidth() - W - margin,
              Y = GetScreenHeight() - H - margin;

  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
  ui::Panel({X, Y, W, H}, S);

  float x1 = X + pad;
  float y1 = Y + pad;

  ui::Header(font, "INVENTORY", x1, y1, gridW, fs, S);

  Vector2 mouse = GetMousePosition();
  int clicked = -1, hovered = -1;
  const int gx = x1, gy = y1;

  for (int i = 0; i < COLS * ROWS; i++) {
    Rectangle r{(float)(gx + (i % COLS) * (cell + gapCell)),
                (float)(gy + (i / COLS) * (cell + gapCell)), (float)cell,
                (float)cell};
    bool hover = CheckCollisionPointRec(mouse, r);
    DrawRectangleRec(r,
                     hover ? Color{70, 55, 45, 255} : Color{40, 30, 30, 255});
    DrawRectangleLinesEx(
        r, 1, hover ? Color{220, 180, 90, 255} : Color{80, 65, 50, 255});

    if (i < (int)slots.size() && !slots[i].empty()) {
      DrawRectangle((int)r.x + 4 * S, (int)r.y + 4 * S, cell - 8 * S,
                    cell - 8 * S, {150, 120, 80, 255});
      if (slots[i].count > 1) {
        DrawTextEx(font, TextFormat("%d", slots[i].count),
                   {r.x + r.width - 8.0f * S, r.y + r.height - 9.0f * S},
                   7.0f * S, 1, WHITE);
      }
      if (hover) hovered = i;
      if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) clicked = i;
    }
  }

  if (hovered >= 0) {
    const char* t = slots[hovered].name.c_str();
    Vector2 sz = MeasureTextEx(font, t, fs, 1);
    float tx = std::min(mouse.x + 12, GetScreenWidth() - sz.x - 8.0f * S);
    float ty = std::min(mouse.y + 12, GetScreenHeight() - sz.y - 8.0f * S);
    DrawRectangle((int)tx - 4 * S, (int)ty - 3 * S, (int)sz.x + 8 * S,
                  (int)sz.y + 6 * S, {10, 8, 8, 240});
    DrawTextEx(font, t, {tx, ty}, fs, 1, WHITE);
  }
  return clicked;
}
