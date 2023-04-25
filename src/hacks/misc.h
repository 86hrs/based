#pragma once
class CUserCmd;

namespace f {
void RunBunnyHop(CUserCmd* cmd) noexcept;
void RunRadar() noexcept;
void IgnoreFlash(float alpha) noexcept;
void RecoilControl(CUserCmd* cmd) noexcept;
}  // namespace hacks
