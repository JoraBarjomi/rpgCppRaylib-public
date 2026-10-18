#include "HeroView.hpp"

// load animations and push in states of animations
void HeroView::Load() {
  idle_.push_back(MakeAnim("heroAnims/Idle/Character_Idle.png", 4, 0.15f));
  idle_.push_back(
      MakeAnim("heroAnims/Idle/Equipment/Character_Idle_Beard.png", 4, 0.15f));
  idle_.push_back(
      MakeAnim("heroAnims/Idle/Equipment/Character_Idle_Hair.png", 4, 0.15f));
  idle_.push_back(
      MakeAnim("heroAnims/Idle/Equipment/Character_Idle_Boots.png", 4, 0.15f));
  idle_.push_back(MakeAnim(
      "heroAnims/Idle/Equipment/Character_Idle_Leggings.png", 4, 0.15f));
  idle_.push_back(MakeAnim(
      "heroAnims/Idle/Equipment/Character_Idle_Chestplate.png", 4, 0.15f));
  idle_.push_back(
      MakeAnim("heroAnims/Idle/Equipment/Character_Idle_Gloves.png", 4, 0.15f));
  idle_.push_back(MakeAnim(
      "heroAnims/Idle/Equipment/Character_Idle_Sword_2.png", 4, 0.15f));
  //   idle_.push_back(
  //       MakeAnim("heroAnims/Idle/Equipment/Character_Idle_Hat.png", 4,
  //       0.15f));

  /*    */
  run_.push_back(MakeAnim("heroAnims/Move/Character_Move.png", 6, 0.1f));
  run_.push_back(
      MakeAnim("heroAnims/Move/Equipment/Character_Move_Beard.png", 6, 0.1f));
  run_.push_back(
      MakeAnim("heroAnims/Move/Equipment/Character_Move_Hair.png", 6, 0.1f));
  run_.push_back(
      MakeAnim("heroAnims/Move/Equipment/Character_Move_Boots.png", 6, 0.1f));
  run_.push_back(MakeAnim(
      "heroAnims/Move/Equipment/Character_Move_Leggings.png", 6, 0.1f));
  run_.push_back(MakeAnim(
      "heroAnims/Move/Equipment/Character_Move_Chestplate.png", 6, 0.1f));
  run_.push_back(
      MakeAnim("heroAnims/Move/Equipment/Character_Move_Sword_2.png", 6, 0.1f));
  run_.push_back(
      MakeAnim("heroAnims/Move/Equipment/Character_Move_Gloves.png", 6, 0.1f));
  //   run_.push_back(
  //       MakeAnim("heroAnims/Move/Equipment/Character_Move_Hat.png", 6,
  //       0.1f));

  /*    */
  attack_.push_back(
      MakeAnim("heroAnims/Attack/Character_Attack.png", 6, 0.08f, false));
  attack_.push_back(
      MakeAnim("heroAnims/Attack/Equipment/Character_Attack_Beard.png", 6,
               0.08f, false));
  attack_.push_back(MakeAnim(
      "heroAnims/Attack/Equipment/Character_Attack_Hair.png", 6, 0.08f, false));
  attack_.push_back(
      MakeAnim("heroAnims/Attack/Equipment/Character_Attack_Boots.png", 6,
               0.08f, false));
  attack_.push_back(
      MakeAnim("heroAnims/Attack/Equipment/Character_Attack_Leggings.png", 6,
               0.08f, false));
  attack_.push_back(
      MakeAnim("heroAnims/Attack/Equipment/Character_Attack_Chestplate.png", 6,
               0.08f, false));
  attack_.push_back(
      MakeAnim("heroAnims/Attack/Equipment/Character_Attack_Sword_2.png", 6,
               0.08f, false));
  attack_.push_back(
      MakeAnim("heroAnims/Attack/Equipment/Character_Attack_Gloves.png", 6,
               0.08f, false));
  attack_.push_back(MakeAnim(
      "heroAnims/Attack/Equipment/Character_Attack_FX.png", 6, 0.08f, false));

  hurt_.push_back(
      MakeAnim("heroAnims/Idle/Character_Idle.png", 4, 0.08f, false));
  hurt_.push_back(MakeAnim("heroAnims/Idle/Equipment/Character_Idle_Beard.png",
                           4, 0.08f, false));
  hurt_.push_back(MakeAnim("heroAnims/Idle/Equipment/Character_Idle_Hair.png",
                           4, 0.08f, false));
  hurt_.push_back(MakeAnim("heroAnims/Idle/Equipment/Character_Idle_Boots.png",
                           4, 0.08f, false));
  hurt_.push_back(MakeAnim(
      "heroAnims/Idle/Equipment/Character_Idle_Leggings.png", 4, 0.08f, false));
  hurt_.push_back(
      MakeAnim("heroAnims/Idle/Equipment/Character_Idle_Chestplate.png", 4,
               0.08f, false));
  hurt_.push_back(MakeAnim("heroAnims/Idle/Equipment/Character_Idle_Gloves.png",
                           4, 0.08f, false));
  hurt_.push_back(MakeAnim(
      "heroAnims/Idle/Equipment/Character_Idle_Sword_2.png", 4, 0.08f, false));

  death_.push_back(
      MakeAnim("heroAnims/Death/Character_Death.png", 11, 0.15f, false));
}

// using this for vector of animations
Animation HeroView::MakeAnim(const char* path, int frames, float frameTime,
                             bool loop) {
  Animation a;
  a.atlas = LoadAtlas(path, 128);
  a.first = 0;
  a.frameCount = frames;
  a.frameTime = frameTime;
  a.loop = loop;
  return a;
}

bool HeroView::Update(float dt, const Hero& hero) {
  facing_ = hero.getFacing();

  // if hero is moving set wanted to Layers of Animation we have to draw

  Layers* wanted;

  switch (hero.getState()) {
    case HeroState::Idle:
      wanted = &idle_;
      break;
    case HeroState::Run:
      wanted = &run_;
      break;
    case HeroState::Attack:
      wanted = &attack_;
      break;
    case HeroState::Hurt:
      wanted = &hurt_;
      break;
    case HeroState::Dead:
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
    layer.first = rowOf[hero.getFacing()] * layer.atlas.cols;
    layer.frame = base.frame;
  }

  if (wanted == &attack_ || wanted == &hurt_)
    return is_anim_end;
  else
    return false;
}

void HeroView::Draw(const Hero& hero) const {
  float now = GetTime();
  Vector2 drawPos = hero.getPos();
  Color tint = WHITE;

  if (hero.getHurtTime() > 0) {
    float t = hero.getHurtTime() / 0.2f;
    drawPos.x += sinf(now * 91.0f) * 5.0f * t;
    drawPos.y += cosf(now * 113.0f) * 3.0f * t;
    unsigned char gb = static_cast<unsigned char>(255 * (1.0f - t));
    tint = Color{255, gb, gb, 255};
  }
  for (auto& layer : *current_) layer.Draw(drawPos, 3.0f, tint);
}

void HeroView::Unload() {
  for (auto& layer : idle_) UnloadTexture(layer.atlas.tex);
  for (auto& layer : run_) UnloadTexture(layer.atlas.tex);
  for (auto& layer : attack_) UnloadTexture(layer.atlas.tex);
  for (auto& layer : hurt_) UnloadTexture(layer.atlas.tex);
  for (auto& layer : death_) UnloadTexture(layer.atlas.tex);
}