#pragma once
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

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
