#pragma once
#include <memory>

#include "../core/hero/Hero.hpp"
#include "../core/item/itemDb.h"
#include "../render/SoundManager.hpp"
#include "../render/TextureManager.hpp"
#include "../render/heroView/HeroHud.hpp"
#include "../render/heroView/HeroView.hpp"
#include "../render/logger/Logger.hpp"
#include "../render/skeletView/skeletView.hpp"
#include "../render/tilesView/TileView.hpp"
#include "raylib.h"

static void ConnectHeroSignals(Hero& hero, Logger& logger,
                               SoundManager& audio) {
  hero.onHpChanged.Connect([&logger, &audio](int dmg) {
    logger.AddMessage("Hero received dmg: - " + std::to_string(dmg) + " HP",
                      WHITE);
    audio.Play("hit", true);
  });

  hero.onDeath.Connect([&logger, &audio]() {
    logger.AddMessage("Hero died by Skelet", WHITE);
    audio.Play("death", true);
  });

  hero.onLvlUp.Connect([&logger, &audio](int lvl) {
    logger.AddMessage("New level " + std::to_string(lvl), GOLD);
    audio.Play("lvlUp");
  });

  hero.onStep.Connect([&audio]() {
    int rnd = Random::get(1, 100);
    if (rnd > 50)
      audio.Play("step1", true);
    else
      audio.Play("step2", true);
  });

  hero.onAttack.Connect([&audio]() {
    audio.Play("slash" + std::to_string(Random::get(1, 4)), true);
  });

  hero.onHitEnemy.Connect([&audio]() {
    audio.Play("swordHit" + std::to_string(Random::get(1, 3)), true);
  });

  hero.onExpChanged.Connect([&logger](int exp) {
    logger.AddMessage("+" + std::to_string(exp) + " exp", WHITE);
  });

  hero.onAttrPointsChanged.Connect([&logger](int points) {
    logger.AddMessage("+" + std::to_string(points) + " points", GOLD);
  });
}

enum class GameResult { Quit, Restart };

static GameResult RunSession(SoundManager& audio, HeroView& heroView,
                             SkeletView& baseSkeletView, TileView& tiles) {
  int currentFps = 120;
  int menuAction = 0;

  Hero hero = Hero::createHero(0);
  for (size_t i = 0; i < 24; i++) {
    hero.inventory.addItem(createRndItem(), 1);
  }

  std::vector<Skelet> skeletons;
  std::vector<SkeletView> skeletonsView;
  Logger logger;

  ConnectHeroSignals(hero, logger, audio);

  bool is_attack = false;
  bool is_attack2 = false;
  bool showLvlUp = false;
  float lvlUpDelay = 0.0f;
  bool showRestartMenu = false;
  float deadDelay = 0.0f;

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
  SetExitKey(KEY_Q);
  while (!WindowShouldClose())  // run the loop until the user presses ESCAPE or
                                // presses the Close button on the window
  {
    int S = std::max(1, GetScreenHeight() / 360);
    is_attack = false;
    is_attack2 = false;
    // time spend to drawn last frame
    float dt = GetFrameTime();
    // direction x y
    Vector2 dir = {0, 0};
    Vector2 dir2 = {0, 0};

    if (lvlUpDelay > 0 && showLvlUp) lvlUpDelay -= dt;
    if (lvlUpDelay <= 0 && showLvlUp) {
      showLvlUp = false;
      menuOpen = true;
      tab = Tab::LvlUp;
    }

    if (deadDelay > 0 && showRestartMenu) deadDelay -= dt;
    if (deadDelay <= 0 && showRestartMenu) {
      showRestartMenu = false;
      menuOpen = true;
      tab = Tab::Restart;
    }

    camera.zoom = expf(logf(camera.zoom) + GetMouseWheelMove() * 0.1f);
    camera.zoom = std::clamp(camera.zoom, 0.5f, 4.0f);

    if (hero.isDead() && IsKeyPressed(KEY_R)) return GameResult::Restart;

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dir.x += 1;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) dir.x -= 1;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) dir.y -= 1;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) dir.y += 1;

    if (IsKeyPressed(KEY_L)) {
      hero.attackUnit(hero);
    }
    if (IsKeyPressed(KEY_H)) {
      // is_hurt = true;
      is_attack2 = true;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !menuOpen &&
        !hero.isDead()) {
      is_attack = true;
      hero.UpdateFacing(camera);
      hero.onAttack.Emit();
    }

    if (IsKeyPressed(KEY_I)) {
      menuOpen = !(menuOpen && tab == Tab::Inventory);
      tab = Tab::Inventory;
    }
    if (IsKeyPressed(KEY_C)) {
      menuOpen = !(menuOpen && tab == Tab::Stats);
      tab = Tab::Stats;
    }
    if (IsKeyPressed(KEY_U)) {
      menuOpen = !(menuOpen && tab == Tab::LvlUp);
      tab = Tab::LvlUp;
    }
    if (menuOpen && tab == Tab::LvlUp && hero.getPoints() == 0)
      menuOpen = false;
    if (IsKeyPressed(KEY_ESCAPE)) {
      menuOpen = false;
    }

    if (!menuOpen && !hero.isDead()) {
      std::vector<Rectangle> obstacles;
      for (const auto& s : skeletons)
        if (!s.isDead()) obstacles.push_back(s.getHitbox());

      std::vector<Rectangle> obstaclesForOthers;
      if (!hero.isDead()) obstaclesForOthers.push_back(hero.getHitbox());

      spawn_timer -= dt;
      if (spawn_timer <= 0.0f && skeletons.size() < max_skeletons) {
        spawn_timer = spawn_interval;
        for (size_t i = 0; i < 3; i++) {
          Skelet newShkilet = Skelet::createSkelet(Random::get(0, 5));
          float rx =
              hero.getPos().x + static_cast<float>(Random::get(-1000, 1000));
          float ry =
              hero.getPos().y + static_cast<float>(Random::get(-1000, 1000));
          newShkilet.setPos(rx, ry);
          skeletons.push_back(newShkilet);
          skeletonsView.push_back(baseSkeletView);
        }
      }

      constexpr int hit_frame = 2;
      if (hero.getState() == HeroState::Attack && !hero.hitApplied() &&
          heroView.CurrentFrame() >= hit_frame) {
        for (auto& s : skeletons) {
          if (!s.isDead() &&
              CheckCollisionRecs(hero.getRangebox(), s.getHitbox())) {
            auto res = hero.attackUnit(s);
            hero.onHitEnemy.Emit();
            if (res.isEnemyDead)
              audio.Play("skeletDeath", true);
            else
              audio.Play("skeletHurt", true);

            if (res.expGained > 0) {
              hero.plusExp(res.expGained);
              auto r = hero.isLvlUp();
              if (r.isLvlUp) {
                showLvlUp = true;
                lvlUpDelay = 3.0f;
              }
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

    } else if (!menuOpen && hero.isDead()) {
      if (heroView.Update(dt, hero)) hero.OnAnimationFinished();
      logger.Update(dt);
      if (!showRestartMenu) {
        showRestartMenu = true;
        deadDelay = 1.5f;
      }
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
    const char* fpsText = 0;
    fpsText = TextFormat("FPS: %i", GetFPS());
    DrawText(fpsText, 1800, 10, 20, WHITE);
    logger.Draw(0, 0, S);
    // hp bar and digit of lvl
    int w = 400, h = 25, margin = 20;
    DrawHpBar(GetScreenWidth() / 2 - w / 2, GetScreenHeight() - h - margin, w,
              h, hero.getHp(), hero.getMaxHp(), hero.getHpGhost(), dt);
    // DrawText(TextFormat("(%i)", hero.getLvl()), 10, 10, 20, GOLD);
    if (menuOpen && tab == Tab::Inventory) {
      int clicked = DrawInventory(MakeInvView(hero), S);
      if (clicked >= 0 && clicked < (int)hero.inventory.getSlots().size()) {
        hero.UseItemAt(clicked);
      }
    }
    if (menuOpen && tab == Tab::Stats) {
      DrawStats(hero, S);
    }
    if (menuOpen && tab == Tab::LvlUp) {
      DrawLvlUp(hero, S);
      showLvlUp = false;
    }
    if (hero.isDead() && menuOpen && tab == Tab::Restart) {
      int r = DrawQuitRestartMenu(hero, S);
      if (r == 1)
        menuAction = 1;
      else if (r == 2)
        menuAction = 2;
    }
    // const char* skeletText = 0;
    // skeletText = TextFormat("HERO ATK: %i SKELET HP: %i (is dead: %s)",
    //                         hero.getAtk(), skeletons[0].getHp(),
    //                         skeletons[0].isDead() ? "true" : "false");
    // DrawText(skeletText, 30, 30, 20, WHITE);

    // end the frame and get ready for the next one  (display frame, poll
    // input, etc...)

    EndDrawing();

    if (menuAction == 1)
      return GameResult::Restart;
    else if (menuAction == 2)
      return GameResult::Quit;
  }
  return GameResult::Quit;
}