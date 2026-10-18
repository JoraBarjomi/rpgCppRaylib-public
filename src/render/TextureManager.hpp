#pragma once
#include "raylib.h"
#include "resource_dir.h"  // utility header for SearchAndSetResourceDir

enum Facing { DOWN, RIGHT, LEFT, UP };

struct Atlas {
  Texture2D tex;
  int cell;
  int cols;
};

Atlas LoadAtlas(const char* path, int cell);

Rectangle CellRect(const Atlas& a, int index);

struct Animation {
  Atlas atlas;
  int first;
  int frameCount;
  float frameTime;
  int frame = 0;
  float timer = 0.0f;
  bool loop = true;

  bool Update(float dt, int last_frame);

  void Draw(Vector2 pos, float scale, Color tint) const;
};
