#include <math.h>

#include <algorithm>
#include <iostream>

#include "core/hero/Hero.hpp"
#include "core/skelet/Skelet.hpp"
#include "random.h"
#include "raylib.h"
#include "raymath.h"
#include "render/SoundManager.hpp"
#include "render/TextureManager.hpp"
#include "render/heroView/HeroHud.hpp"
#include "render/heroView/HeroView.hpp"
#include "render/logger/Logger.hpp"
#include "render/skeletView/skeletView.hpp"
#include "render/tilesView/TileView.hpp"
#include "resource_dir.h"  // utility header for SearchAndSetResourceDir
#include "states/game.hpp"

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
  // Hero hero = Hero::createHero(0);
  HeroView heroView;

  // std::vector<Skelet> skeletons;
  // std::vector<SkeletView> skeletonsView;
  SkeletView baseSkeletView;

  TileView tiles;

  // heroPosition on screen and speed
  // bool is_attack = false;
  // bool is_attack2 = false;
  // bool showLvlUp = false;
  // float lvlUpDelay = 0.0f;

  // Init logger and all signals
  // Logger logger;

  InitWindow(screenWidth, screenHeight, "RPG");

  SoundManager audio;
  // Load a texture from the resources directory
  SearchAndSetResourceDir("resources");

  // Load all textures for heroView
  heroView.Load();
  baseSkeletView.Load();
  tiles.Load();
  audio.Load("hit", "sounds/hero/hit/pain3.wav", 0.6);
  audio.Load("death", "sounds/hero/death/die2.wav", 0.6);
  audio.Load("lvlUp", "sounds/hero/lvlUp/lvlUp.mp3", 0.6);
  audio.Load("step1", "sounds/hero/step/step1.wav");
  audio.Load("step2", "sounds/hero/step/step2.wav");
  audio.Load("slash1", "sounds/hero/swordsSounds/slash1.wav", 0.6);
  audio.Load("slash2", "sounds/hero/swordSounds/slash2.wav", 0.6);
  audio.Load("slash3", "sounds/hero/swordSounds/slash3.wav", 0.6);
  audio.Load("slash4", "sounds/hero/swordSounds/slash4.wav", 0.6);
  audio.Load("swordHit1", "sounds/hero/swordSounds/swordHit1.wav", 0.6);
  audio.Load("swordHit2", "sounds/hero/swordSounds/swordHit2.wav", 0.6);
  audio.Load("swordHit3", "sounds/hero/swordSounds/swordHit3.wav", 0.6);
  audio.Load("skeletDeath", "sounds/skelet/skeletDeath.wav", 0.6);
  audio.Load("skeletHurt", "sounds/skelet/skeletHurt.wav", 0.6);
  audio.Load("skeletStep", "sounds/skelet/skeletStep.wav", 0.6);

  GameResult result;
  do {
    result = RunSession(audio, heroView, baseSkeletView, tiles);
  } while (result == GameResult::Restart);

  audio.~SoundManager();
  heroView.Unload();
  baseSkeletView.Unload();
  tiles.Unload();
  CloseWindow();
  return 0;
}
