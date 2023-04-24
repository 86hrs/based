#pragma once
class CUserCmd;

namespace hacks {
void RunBunnyHop(CUserCmd* cmd) noexcept;
void RunRadar() noexcept;
void RunFov(int fov) noexcept;
void IgnoreFlash(float alpha) noexcept;
void RecoilControl(CUserCmd* cmd) noexcept;
}  // namespace hacks
