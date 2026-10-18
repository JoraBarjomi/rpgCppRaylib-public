#pragma once
#include <algorithm>
#include <functional>
#include <memory>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>

template <typename... Args>
class Signal {
 public:
  using Slot = std::function<void(Args...)>;

  void Connect(Slot slot) { m_slots.push_back(std::move(slot)); }

  void Emit(Args... args) {
    for (const auto& slot : m_slots) {
      slot(args...);
    }
  }

 private:
  std::vector<Slot> m_slots;
};

enum class GameState {
  MainMenu,
  Exploring,
  Battle,
  Dialogue,
  Inventory,
  Paused,
  GameOver
};

// std::string ToString(GameState state) {
//   switch (state) {
//     case GameState::MainMenu:
//       return "Main menu";
//     case GameState::Exploring:
//       return "Exploring";
//     case GameState::Battle:
//       return "Battle";
//     case GameState::Dialogue:
//       return "Dialogue";
//     case GameState::Inventory:
//       return "Inventory";
//     case GameState::Paused:
//       return "Paused";
//     case GameState::GameOver:
//       return "GameOver";
//     default:
//       return "???";
//   }
// }

class GameStateManager {
 public:
  Signal<GameState, GameState> onChange;
  explicit GameStateManager(GameState initial = GameState::MainMenu)
      : m_current{initial}, m_previous{initial} {
    m_transitions = {
        {GameState::MainMenu, {GameState::Exploring}},
        {GameState::Exploring,
         {GameState::Battle, GameState::Dialogue, GameState::Inventory,
          GameState::Paused, GameState::GameOver}},
        {GameState::Battle,
         {GameState::Exploring, GameState::Inventory, GameState::GameOver,
          GameState::Paused}},
        {GameState::Dialogue, {GameState::Exploring, GameState::Paused}},
        {GameState::Inventory,
         {GameState::Exploring, GameState::Battle, GameState::Paused}},
        {GameState::Paused, {}},
        {GameState::GameOver, {GameState::MainMenu}},
    };
  }

  bool canGo(GameState to) const;

  bool go(GameState to);

  bool pause();

  bool resume();

  bool is(GameState state) const { return m_current == state; }
  GameState current() const { return m_current; }

 private:
  GameState m_current;
  GameState m_previous;
  std::vector<GameState> m_pauseStack;
  std::unordered_map<GameState, std::vector<GameState>> m_transitions;
};
