#pragma once
#include <cstdint>

class CEntity;
namespace g {
inline CEntity* localPlayer = nullptr;

// update the local player pointer
void UpdateLocalPlayer() noexcept;

inline bool guiBhop = false;

inline bool guiTeamGlow = false;
inline bool guiEnemyGlow = false;
inline float teamGlowColor[3] = {0.f, 0.f, 1.f};
inline float enemyGlowColor[3] = {1.f, 0.f, 0.f};

inline bool esp = false;

inline bool guiChams = false;
inline bool bHealthBasedChams = false;
inline float visible[3] = {1.f, 1.f, 0.f};
inline float hidden[3] = {0.f, 1.f, 1.f};

inline bool guiAimbot = false;
inline int guiAimbotBone = 0;
inline int aimbotFov = 5;
inline int aimbotSmoothness = 1;

inline bool guiTriggerbot = false;

inline bool fov = false;
inline int fovValue = 90;

inline bool ignoreFlash = false;

inline bool radar = false;

inline bool ignoreRecoil = false;
}  // namespace globals
