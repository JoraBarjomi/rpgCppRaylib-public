#include "skeletView.hpp"

// load animations and push in states of animations
void SkeletView::Load() {
  idle_.push_back(MakeAnim("Skeleton/Skeleton_Idle.png", 4, 0.15f));

  attack_.push_back(MakeAnim("Skeleton/Skeleton_Move.png", 6, 0.1f));

  /*    */
  run_.push_back(MakeAnim("Skeleton/Skeleton_Move.png", 6, 0.1f));

  hurt_.push_back(MakeAnim("Skeleton/Skeleton_Idle.png", 4, 0.1f, false));

  death_.push_back(MakeAnim("Skeleton/Character_Death.png", 11, 0.1f, false));
}

// using this for vector of animations
Animation SkeletView::MakeAnim(const char* path, int frames, float frameTime,
                               bool loop) {
  Animation a;
  a.atlas = LoadAtlas(path, 128);
  a.first = 0;
  a.frameCount = frames;
  a.frameTime = frameTime;
  a.loop = loop;
  return a;
}

bool SkeletView::Update(float dt, const Skelet& skelet) {
  facing_ = skelet.getFacing();

  // if hero is moving set wanted to Layers of Animation we have to draw
  Layers* wanted;

  switch (skelet.getState()) {
    case SkeletState::Idle:
      wanted = &idle_;
      break;
    case SkeletState::Run:
      wanted = &run_;
      break;
    case SkeletState::Attack:
      wanted = &attack_;
      break;
    case SkeletState::Hurt:
      wanted = &hurt_;
      break;
    case SkeletState::Dead:
      wanted = &death_;
      break;
    default:
      wanted = &idle_;
      break;
  }

  if (wanted != current_) {
    current_ = wanted;
    for (auto& layer : *current_) {
      layer.frame = 0;
      layer.timer = 0.0f;
    }
  }

  // 4 directions of anims
  static const int rowOf[4] = {0, 1, 2, 3};
  // 0 layer of our cur
  Animation& base = (*current_)[0];
  // play our anim from this frame
  base.first = rowOf[facing_] * base.atlas.cols;
  bool is_anim_end = false;
  is_anim_end = base.Update(dt, base.frameCount - 1);

  // sync others layers
  for (size_t i = 1; i < current_->size(); i++) {
    Animation& layer = (*current_)[i];
    layer.first = rowOf[skelet.getFacing()] * layer.atlas.cols;
    layer.frame = base.frame;
  }

  if (wanted == &hurt_ || wanted == &death_)
    return is_anim_end;
  else
    return false;
}

void SkeletView::DrawHpBar(int x, int y, int hp, int maxHp, float& ghost,
                           float dt) const {
  const int W = 34, H = 7;
  float ratio = std::clamp((float)hp / maxHp, 0.0f, 1.0f);
  ghost = (ghost > ratio) ? std::max(ratio, ghost - 0.7f * dt) : ratio;

  DrawRectangle(x, y, W + 2, H + 2, BLACK);
  DrawRectangle(x, y, (int)(W * ghost), H, ORANGE);
  DrawRectangle(x, y, (int)(W * ratio), H, MAROON);
}

void SkeletView::Draw(const Skelet& skelet) const {
  float now = GetTime();
  Vector2 drawPos = skelet.getPos();

  if (!skelet.isDead()) {
    DrawText(TextFormat("%i", skelet.getLvl()), drawPos.x + 163,
             drawPos.y + 133, 10, GOLD);
    DrawHpBar(drawPos.x + 175, drawPos.y + 134, skelet.getHp(),
              skelet.getMaxHp(), skelet.hp_ghost_, now);
  }

  Color tint = WHITE;
  if (skelet.getHurtTime() > 0) {
    float t = skelet.getHurtTime() / 0.2f;
    drawPos.x += sinf(now * 91.0f) * 5.0f * t;
    drawPos.y += cosf(now * 113.0f) * 3.0f * t;
    unsigned char gb = static_cast<unsigned char>(255 * (1.0f - t));
    tint = Color{255, gb, gb, 255};
  }

  if (skelet.getState() == SkeletState::Attack) {
    Vector2 center = {drawPos.x + 192, drawPos.y + 192};
    float waveT = fmodf(now, 0.6f) /
                  0.6f;  // normalize from 0 to 1 amplitude in this cur time
    float wr = 30.0f + 120.0f * waveT;  // wave radius
    DrawRing(center, wr - 5, wr, 0, 360, 48, Fade(RED, 0.7f * (1.0 - waveT)));
    DrawCircle(center.x, center.y, 44.0f, Fade(RED, 0.24f));
    unsigned char k = static_cast<unsigned char>(
        100 * (0.5f + 0.5f * sinf(now * 30.0f)));  // pulse effect
    tint = Color{255, static_cast<unsigned char>(255 - k), 0, 255};
  }
  for (auto& layer : *current_) {
    layer.Draw(drawPos, 3.0f, tint);
  }
}

void SkeletView::Unload() {
  for (auto& layer : idle_) UnloadTexture(layer.atlas.tex);
  for (auto& layer : run_) UnloadTexture(layer.atlas.tex);
  for (auto& layer : hurt_) UnloadTexture(layer.atlas.tex);
  for (auto& layer : death_) UnloadTexture(layer.atlas.tex);
}
