#include "stateManager.h"

bool GameStateManager::canGo(GameState to) const {
  auto it = m_transitions.find(m_current);
  if (it == m_transitions.end()) return false;
  return std::find(it->second.begin(), it->second.end(), to) !=
         it->second.end();
}

bool GameStateManager::go(GameState to) {
  if (!canGo(to)) return false;
  m_previous = m_current;
  m_current = to;
  onChange.Emit(m_previous, m_current);
  return true;
}

bool GameStateManager::pause() {
  if (m_current == GameState::Paused) return false;
  m_pauseStack.push_back(m_current);
  m_previous = m_current;
  m_current = GameState::Paused;
  onChange.Emit(m_previous, m_current);
  return true;
}

bool GameStateManager::resume() {
  if (m_pauseStack.empty()) return false;
  auto target = m_pauseStack.back();
  m_pauseStack.pop_back();
  m_previous = m_current;
  m_current = target;
  onChange.Emit(m_previous, m_current);
  return true;
}