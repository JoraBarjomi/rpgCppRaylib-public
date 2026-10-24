#pragma once
#include <string>
#include <unordered_map>

#include "../random.h"
#include "raylib.h"

class SoundManager {
 private:
  std::unordered_map<std::string, Sound> sounds_;

 public:
  SoundManager() { InitAudioDevice(); };
  ~SoundManager() {
    for (auto& [id, sound] : sounds_) {
      UnloadSound(sound);
    }
    CloseAudioDevice();
  };

  void Load(const std::string& id, const std::string& path,
            float volume = 0.3f) {
    Sound s = LoadSound(path.c_str());
    SetSoundVolume(s, volume);
    sounds_[id] = s;
  }

  void Play(const std::string& id, bool randomPitch = false) {
    auto it = sounds_.find(id);
    if (it == sounds_.end()) return;

    float pitch = randomPitch ? Random::get(90, 110) / 100.0f : 1.0f;
    SetSoundPitch(it->second, pitch);
    PlaySound(it->second);
  }
};
