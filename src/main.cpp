#include <math.h>

#include <algorithm>
#include <iostream>

#include "core/hero/Hero.hpp"
#include "core/skelet/Skelet.hpp"
#include "random.h"
#include "raylib.h"
#include "raymath.h"
#include "render/TextureManager.hpp"
#include "render/heroView/HeroHud.hpp"
#include "render/heroView/HeroView.hpp"
#include "render/logger/Logger.hpp"
#include "render/skeletView/skeletView.hpp"
#include "render/tilesView/TileView.hpp"
#include "resource_dir.h"  // utility header for SearchAndSetResourceDir

int main() {
  // Tell the window to use vsync and work on high DPI displays
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE |
                 FLAG_FULLSCREEN_MODE);
  // SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI |
  // FLAG_WINDOW_RESIZABLE);

  // Set window settings and fps max
  int currentFps = 120;
  const int screenWidth = 800;
  const int screenHeight = 450;
  // Create the window and OpenGL context

  // Create hero
  Hero hero = Hero::createHero(0);
  HeroView heroView;

  std::vector<Skelet> skeletons;
  std::vector<SkeletView> skeletonsView;
  SkeletView baseSkeletView;

  TileView tiles;

  // heroPosition on screen and speed
  bool is_attack = false;
  bool is_attack2 = false;

  // Init logger and all signals
  Logger logger;
  hero.onHpChanged.Connect([&logger](int dmg) {
    logger.AddMessage("Hero received dmg: - " + std::to_string(dmg) + " HP",
                      RED);
  });

  hero.onDeath.Connect(
      [&logger]() { logger.AddMessage("Hero died by Skelet", RED); });

  InitWindow(screenWidth, screenHeight, "RPG");

  // Load a texture from the resources directory
  SearchAndSetResourceDir("resources");

  // Load all textures for heroView
  heroView.Load();
  baseSkeletView.Load();
  tiles.Load();

  Camera2D camera;
  camera.target = (Vector2){hero.getPos().x + 192, hero.getPos().y + 192};
  camera.offset = (Vector2){GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  float spawn_timer = 0.0f;
  constexpr float spawn_interval = 3.0f;
  constexpr size_t max_skeletons = 15;

  bool menuOpen = false;
  Tab tab = Tab::Inventory;

  SetTargetFPS(currentFps);
  while (!WindowShouldClose())  // run the loop until the user presses ESCAPE or
                                // presses the Close button on the window
  {
    is_attack = false;
    is_attack2 = false;
    // time spend to drawn last frame
    float dt = GetFrameTime();
    // direction x y
    Vector2 dir = {0, 0};
    Vector2 dir2 = {0, 0};

    camera.zoom = expf(logf(camera.zoom) + GetMouseWheelMove() * 0.1f);
    if (camera.zoom > 4.0f)
      camera.zoom = 4.0f;
    else if (camera.zoom < 0.1f)
      camera.zoom = 0.1f;
    // camera.zoom = expf(logf(camera.zoom) + GetMouseWheelMove() * 0.1f);
    // camera.zoom = std::clamp(camera.zoom, 0.5f, 4.0f);

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dir.x += 1;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) dir.x -= 1;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) dir.y -= 1;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) dir.y += 1;

    if (IsKeyPressed(KEY_L)) {
      hero.attackUnit(hero);
      // is_dead = true; goState че то такое
    }
    if (IsKeyPressed(KEY_H)) {
      // is_hurt = true;
      is_attack2 = true;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      is_attack = true;
      hero.UpdateFacing(camera);
    }

    SetExitKey(KEY_P);
    if (IsKeyPressed(KEY_I)) {
      menuOpen = !(menuOpen && tab == Tab::Inventory);
      tab = Tab::Inventory;
    }
    if (IsKeyPressed(KEY_C)) {
      menuOpen = !(menuOpen && tab == Tab::Stats);
      tab = Tab::Stats;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
      menuOpen = false;
    }

    if (!menuOpen) {
      std::vector<Rectangle> obstacles;
      for (const auto& s : skeletons)
        if (!s.isDead()) obstacles.push_back(s.getHitbox());

      std::vector<Rectangle> obstaclesForOthers;
      if (!hero.isDead()) obstaclesForOthers.push_back(hero.getHitbox());

      spawn_timer -= dt;
      if (spawn_timer <= 0.0f && skeletons.size() < max_skeletons) {
        spawn_timer = spawn_interval;

        Skelet newShkilet = Skelet::createSkelet(0);
        float rx =
            hero.getPos().x + static_cast<float>(Random::get(-3000, 3000));
        float ry =
            hero.getPos().y + static_cast<float>(Random::get(-3000, 3000));
        newShkilet.setPos(rx, ry);
        skeletons.push_back(newShkilet);
        skeletonsView.push_back(baseSkeletView);
      }

      constexpr int hit_frame = 2;
      if (hero.getState() == HeroState::Attack && !hero.hitApplied() &&
          heroView.CurrentFrame() >= hit_frame) {
        for (auto& s : skeletons) {
          if (!s.isDead() &&
              CheckCollisionRecs(hero.getRangebox(), s.getHitbox())) {
            auto res = hero.attackUnit(s);
            if (res.expGained > 0) {
              hero.plusExp(res.expGained);
            }
          }
        }
        hero.markHitApplied();
      }

      for (size_t i = 0; i < skeletons.size(); i++) {
        if (!skeletons[i].hitApplied()) {
          if (CheckCollisionRecs(skeletons[i].getRangebox(),
                                 hero.getHitbox())) {
            skeletons[i].attackUnit(hero);
          }
          skeletons[i].markHitApplied();
        }

        skeletons[i].Update(dt, dir2, obstaclesForOthers, hero.getPos());
        if (skeletonsView[i].Update(dt, skeletons[i]) &&
            !skeletons[i].isDead()) {
          skeletons[i].OnAnimationFinished();
        }
      }

      float len_dir{std::hypot(dir.x, dir.y)};
      if (len_dir > 0) dir = {dir.x / len_dir, dir.y / len_dir};

      hero.Update(dt, dir, is_attack, obstacles);
      if (heroView.Update(dt, hero)) {
        hero.OnAnimationFinished();
      }
      logger.Update(dt);

      camera.target = (Vector2){hero.getPos().x + 192, hero.getPos().y + 192};
    }

    // drawing
    BeginDrawing();
    ClearBackground(GRAY);
    BeginMode2D(camera);

    tiles.Draw(-1000, 0);

    // draw our the hero to the screen
    heroView.Draw(hero);
    for (size_t i = 0; i < skeletons.size(); i++)
      skeletonsView[i].Draw(skeletons[i]);

    // DrawRectangleLinesEx(hero.getHitbox(), 2.0f, GREEN);
    // DrawRectangleLinesEx(hero.getRangebox(), 2.0f, GREEN);

    // for (size_t i = 0; i < skeletons.size(); i++) {
    //   DrawRectangleLinesEx(skeletons[i].getHitbox(), 2.0f, RED);
    //   DrawRectangleLinesEx(skeletons[i].getRangebox(), 2.0f, RED);
    // }

    EndMode2D();

    // draw the fps on the screen
    int S = std::max(1, GetScreenHeight() / 360);
    const char* fpsText = 0;
    fpsText = TextFormat("FPS: %i", GetFPS());
    DrawText(fpsText, 1800, 10, 20, WHITE);
    logger.Draw(0, 0, S);
    // hp bar and digit of lvl
    int w = 400, h = 25, margin = 20;
    DrawHpBar(GetScreenWidth() / 2 - w / 2, GetScreenHeight() - h - margin, w,
              h, hero.getHp(), hero.getMaxHp(), hero.getHpGhost(), dt);
    // DrawText(TextFormat("(%i)", hero.getLvl()), 10, 10, 20, GOLD);
    if (menuOpen && tab == Tab::Stats) {
      DrawStats(hero, S);
    }

    // const char* skeletText = 0;
    // skeletText = TextFormat("HERO ATK: %i SKELET HP: %i (is dead: %s)",
    //                         hero.getAtk(), skeletons[0].getHp(),
    //                         skeletons[0].isDead() ? "true" : "false");
    // DrawText(skeletText, 30, 30, 20, WHITE);

    // end the frame and get ready for the next one  (display frame, poll
    // input, etc...)
    EndDrawing();
  }

  heroView.Unload();
  baseSkeletView.Unload();
  tiles.Unload();
  CloseWindow();
  return 0;
}
