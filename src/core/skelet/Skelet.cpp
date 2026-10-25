#include "Skelet.hpp"

Skelet::Skelet(int lvl, int totalExp, int hp, int armor, int atk,
               int expForKill, int strength, int agility)
    : Unit(lvl, totalExp, hp, armor, atk, expForKill, strength, agility),
      max_hp_{hp},
      gold_{} {
  recalcStats();
  transitions_ = {{SkeletState::Idle,
                   {SkeletState::Run, SkeletState::Attack, SkeletState::Hurt,
                    SkeletState::Dead}},
                  {SkeletState::Run,
                   {SkeletState::Idle, SkeletState::Attack, SkeletState::Hurt,
                    SkeletState::Dead}},
                  {SkeletState::Attack,
                   {SkeletState::Idle, SkeletState::Run, SkeletState::Hurt,
                    SkeletState::Dead}},
                  {SkeletState::Hurt,
                   {SkeletState::Idle, SkeletState::Run, SkeletState::Attack,
                    SkeletState::Dead}},

                  {SkeletState::Dead, {}}};
};

void Skelet::recalcStats() {
  max_hp_ = base_hp_ + std::ceil(std::exp(lvl_ / 3) - 1) +
            std::ceil(atrb_.strength_ * 0.25);
  //   onMaxHpChanged.Emit(m_maxHp);
  armor_ = std::ceil(atrb_.agility_ / 2.0);
  atk_ = base_atk + std::ceil(atrb_.strength_ * 0.15);
  hp_ = max_hp_;
}

constexpr int pointsPerLvl{10};
Skelet Skelet::createSkelet(int lvl) {
  int all{lvl * pointsPerLvl};
  int strg{static_cast<int>(all * 8.0 / 10.0)};
  int aglt{all - strg};

  int baseHp{5};
  int baseArmor{0};
  int baseAtk{2};
  int baseExpForKill{100 * (lvl != 0 ? lvl : 1)};
  int baseTotalExp{lvl == 0 ? lvlInfo::lvlSteps[0]
                            : lvlInfo::lvlSteps[lvl - 1]};
  int baseStrength{1 + strg};
  int baseAgility{1 + aglt};
  return Skelet(lvl, baseTotalExp, baseHp, baseArmor, baseAtk, baseExpForKill,
                baseStrength, baseAgility);
}

void Skelet::Update(float dt, Vector2 dir,
                    const std::vector<Rectangle>& obstacles, Vector2 heroPos) {
  if (hp_ <= 0) {
    state_ = SkeletState::Dead;
    speed_ = 0;
    return;
  }

  bool busy = (state_ == SkeletState::Attack || state_ == SkeletState::Hurt);

  if (attack_cooldown_ > 0) attack_cooldown_ -= dt;
  if (attack_cooldown_ <= 0) busy = false;

  timer_ -= dt;
  if (hurt_timer_ > 0) hurt_timer_ -= dt;

  float dx = (heroPos.x + 192.0f) - (pos_.x + 192.0f);
  float dy = (heroPos.y + 192.0f) - (pos_.y + 192.0f);
  float distance = std::hypot(dx, dy);
  Vector2 to_hero = (distance > 0.001f) ? Vector2{dx / distance, dy / distance}
                                        : Vector2{0.0f, 0.0f};

  auto updateFacing = [&](Vector2 look_dir) {
    if (std::abs(look_dir.x) > std::abs(look_dir.y)) {
      facing_ = (look_dir.x < 0) ? RIGHT : LEFT;
    } else {
      facing_ = (look_dir.y < 0) ? UP : DOWN;
    };
  };

  bool is_chasing = (distance > 95.0f && distance < 1000.0f);

  if (distance <= 95 && attack_cooldown_ <= 0.0) {
    state_ = SkeletState::Attack;
    hit_applied_ = false;
    attack_cooldown_ = 1.0f + Random::getF(0.1f, 3.0f);
    updateFacing(to_hero);
  } else if (is_chasing && !busy) {
    int rnd = Random::get(1, 100);
    if (rnd > 90)
      state_ = SkeletState::Run;
    else if (rnd >= 100)
      state_ = SkeletState::RunCooldown;

    tryMoveX(to_hero.x * speed_ * dt, obstacles);
    tryMoveY(to_hero.y * speed_ * dt, obstacles);
    updateFacing(to_hero);

  } else if (!busy && rand_dir_.x == 0 && rand_dir_.y == 0) {
    state_ = SkeletState::Idle;
  } else if (!busy) {
    state_ = SkeletState::Run;

    tryMoveX(rand_dir_.x * speed_ * dt, obstacles);
    tryMoveY(rand_dir_.y * speed_ * dt, obstacles);

    if (std::abs(rand_dir_.x) > std::abs(rand_dir_.y)) {
      facing_ = (rand_dir_.x < 0) ? RIGHT : LEFT;
    } else {
      facing_ = (rand_dir_.y < 0) ? UP : DOWN;
    }
  }

  if (timer_ <= 0.0f) {
    int choice = Random::get(0, 100);
    if (choice <= 80) {
      timer_ = 2.0f;
      float rx = Random::get(-100, 100);
      float ry = Random::get(-100, 100);
      float len_dir{std::hypot(rx, ry)};
      rand_dir_ =
          (len_dir > 0) ? Vector2{rx / len_dir, ry / len_dir} : Vector2{0, 0};
    } else {
      timer_ = 3.0f;
      rand_dir_ = {0, 0};
    }
  }
}

int Skelet::takeDamage(int dmg) {
  if (state_ == SkeletState::Dead) return 0;
  int dmgRes = Unit::takeDamage(dmg);
  if (hp_ <= 0) {
    state_ = SkeletState::Dead;
    onDeath.Emit();
    return expForKill_;
  } else {
    hurt_timer_ = 0.25;
    onHpChanged.Emit(dmgRes);
  }
  return 0;
}

bool Skelet::tryMoveX(float step, const std::vector<Rectangle>& obstacles) {
  pos_.x += step;
  for (const Rectangle& obs : obstacles) {
    if (CheckCollisionRecs(getHitbox(), obs)) {
      pos_.x -= step;
      return false;
    }
  }
  return true;
}

bool Skelet::tryMoveY(float step, const std::vector<Rectangle>& obstacles) {
  pos_.y += step;
  for (const Rectangle& obs : obstacles) {
    if (CheckCollisionRecs(getHitbox(), obs)) {
      pos_.y = step;
      return false;
    }
  }
  return true;
}

void Skelet::setPos(float x, float y) {
  pos_.x = x;
  pos_.y = y;
}

Rectangle Skelet::getHitbox() const {
  return Rectangle{pos_.x + 192.0f - 24.0f, pos_.y + 192.0f - 44.0f, 48.0f,
                   90.0f};
}

Rectangle Skelet::getRangebox() const {
  switch (facing_) {
    case Facing::DOWN:
      return Rectangle{pos_.x + 192.0f - 36.0f, pos_.y + 192.0f + 45.0f, 72.0f,
                       36.0f};
    case Facing::UP:
      return Rectangle{pos_.x + 192.0f - 36.0f, pos_.y + 192.0f - 78.0f, 72.0f,
                       36.0f};
    case Facing::LEFT:  // RIGHT
      return Rectangle{pos_.x + 192.0f + 24.0f, pos_.y + 192.0f - 36.0f, 48.0f,
                       72.0f};
    case Facing::RIGHT:  // LEFT
      return Rectangle{pos_.x + 192.0f - 70.0f, pos_.y + 192.0f - 36.0f, 48.0f,
                       72.0f};
    default:
      return Rectangle{pos_.x + 192.0f - 36.0f, pos_.y + 192.0f + 45.0f, 72.0f,
                       36.0f};
  }
}

void Skelet::OnAnimationFinished() {
  if (state_ == SkeletState::Attack || state_ == SkeletState::Hurt)
    state_ = SkeletState::Idle;
}
