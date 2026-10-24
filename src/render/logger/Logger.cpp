#include "Logger.hpp"

void Logger::AddMessage(const std::string& msg, Color color) {
  if (logs.size() >= 8) {
    logs.erase(logs.begin());
  }
  logs.push_back({msg, color, 10.0f});
}

void Logger::Update(float dt) {
  for (auto& log : logs) {
    log.timeLeft -= dt;
  }
  std::erase_if(logs, [](const Log& l) { return l.timeLeft <= 0.0f; });
}

void Logger::Draw(int x, int y, int S) const {
  const float W = 200, H = 40;
  const float fs = 8 * S;
  const float pad = 2.0f * S, gap = 16.0f * S;
  const float colW = (W - pad * 2.0f - gap) / 2.0f;
  const float x1 = x + pad, x2 = x1 + colW + gap;
  float y1 = y + pad, y2 = y + pad;

  int offsetY = 0;
  for (const auto& log : logs) {
    float alpha = (log.timeLeft < 1.0f) ? log.timeLeft : 1.0f;
    ui::TextFading(GetFontDefault(), log.text.c_str(), x1, y1 + offsetY, fs,
                   log.color, alpha);
    offsetY += 26;
  }
}
