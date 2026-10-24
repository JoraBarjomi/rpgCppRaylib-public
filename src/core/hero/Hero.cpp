#include "Hero.hpp"

void Hero::recalcStats() {
  max_hp_ = max_hp_ + std::ceil(std::exp(lvl_ / 3) - 1) +
            std::ceil(atrb_.strength_ * 0.25);
  onMaxHpChanged.Emit(max_hp_);
  armor_ = std::ceil(atrb_.agility_ / 2.0);
  atk_ = atk_ + atrb_.strength_;
  hp_ = max_hp_;
}

Hero::Hero(int lvl, int totalExp, int hp, int armor, int atk, int expForKill,
           int strength, int agility)
    : Unit(lvl, totalExp, hp, armor, atk, expForKill, strength, agility),
      max_hp_{hp},
      gold_{} {
  recalcStats();
  transitions_ = {
      {HeroState::Idle,
       {HeroState::Run, HeroState::Attack, HeroState::Hurt, HeroState::Dead}},
      {HeroState::Run,
       {HeroState::Idle, HeroState::Attack, HeroState::Hurt, HeroState::Dead}},
      {HeroState::Attack,
       {HeroState::Idle, HeroState::Run, HeroState::Hurt, HeroState::Dead}},
      {HeroState::Hurt,
       {HeroState::Idle, HeroState::Run, HeroState::Attack, HeroState::Dead}},

      {HeroState::Dead, {}}};
};

constexpr int pointsPerLvl{10};
Hero Hero::createHero(int lvl) {
  int all{lvl * pointsPerLvl};
  int strg{static_cast<int>(all * 8.0 / 10.0)};
  int aglt{all - strg};

  int baseHp{5};
  int baseArmor{0};
  int baseAtk{2};
  int baseExpForKill{0};
  int baseTotalExp{lvl == 0 ? lvlInfo::lvlSteps[0]
                            : lvlInfo::lvlSteps[lvl - 1]};
  int baseStrength{1 + strg};
  int baseAgility{1 + aglt};
  return Hero(lvl, baseTotalExp, baseHp, baseArmor, baseAtk, baseExpForKill,
              baseStrength, baseAgility);
}

void Hero::Update(float dt, Vector2 dir, bool is_attack,
                  const std::vector<Rectangle> obstacles) {
  if (hp_ <= 0) {
    state_ = HeroState::Dead;
    speed_ = 0;
    return;
  }

  if (hurt_timer_ > 0.0f) hurt_timer_ -= dt;

  bool busy = (state_ == HeroState::Attack);

  if (is_attack && state_ != HeroState::Attack && state_ != HeroState::Hurt) {
    state_ = HeroState::Attack;
    hit_applied_ = false;
  } else if ((dir.x != 0 || dir.y != 0) && !busy) {
    state_ = HeroState::Run;
    step_timer_ -= dt;
    if (step_timer_ <= 0.0f) {
      onStep.Emit();
      step_timer_ = 0.35;
    }

    float oldX = pos_.x;
    pos_.x += dir.x * speed_ * dt;
    for (const Rectangle& obs : obstacles) {
      if (CheckCollisionRecs(getHitbox(), obs)) {
        pos_.x = oldX;
        break;
      }
    }

    float oldY = pos_.y;
    pos_.y += dir.y * speed_ * dt;
    for (const Rectangle& obs : obstacles) {
      if (CheckCollisionRecs(getHitbox(), obs)) {
        pos_.y = oldY;
        break;
      }
    }

    if (dir.x < 0)
      facing_ = RIGHT;
    else if (dir.x > 0)
      facing_ = LEFT;
    else if (dir.y < 0)
      facing_ = UP;
    else if (dir.y > 0)
      facing_ = DOWN;
  } else if (state_ == HeroState::Dead && !busy) {
    state_ = HeroState::Dead;
    speed_ = 0;
  } else if (!busy)
    state_ = HeroState::Idle;
}

// Turn around Hero when attack
void Hero::UpdateFacing(const Camera2D& camera) {
  Vector2 mouseCoord = GetScreenToWorld2D(GetMousePosition(), camera);

  float dx = mouseCoord.x - (pos_.x + 192.0f);
  float dy = mouseCoord.y - (pos_.y + 192.0f);

  if (std::abs(dx) > std::abs(dy)) {
    facing_ = (dx > 0) ? Facing::LEFT : Facing::RIGHT;
  } else {
    facing_ = (dy > 0) ? Facing::DOWN : Facing::UP;
  }
}

int Hero::takeDamage(int dmg) {
  if (state_ == HeroState::Dead) return 0;
  int dmgRes = Unit::takeDamage(dmg);
  if (hp_ <= 0) {
    state_ = HeroState::Dead;
    onDeath.Emit();
    return expForKill_;
  } else {
    hurt_timer_ = 0.25;
    onHpChanged.Emit(dmgRes);
  }
  return 0;
}

void Hero::setPlusPos(int x, int y) {
  pos_.x += x;
  pos_.y += y;
}

Rectangle Hero::getHitbox() const {
  return Rectangle{pos_.x + 192.0f - 24.0f, pos_.y + 192.0f - 44.0f, 48.0f,
                   90.0f};
}

Rectangle Hero::getRangebox() const {
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

void Hero::OnAnimationFinished() {
  if (state_ == HeroState::Attack || state_ == HeroState::Hurt)
    state_ = HeroState::Idle;
}

LvlUpEvent Hero::isLvlUp() {
  struct LvlUpEvent event;
  constexpr size_t maxLvl{lvlInfo::lvlSteps.size() - 1};
  if (lvl_ < maxLvl && exp_total_ >= lvlInfo::lvlSteps[lvl_ + 1]) {
    lvl_++;
    onLvlUp.Emit(lvl_);
    atrb_points_ += 10;
    onAttrPointsChanged.Emit(atrb_points_);
    event.isLvlUp = true;
    event.newLvl = lvl_;
    event.pointsGained = 10;
    return event;
  };
  return event;
}
