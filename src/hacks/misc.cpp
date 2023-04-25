#include "misc.h"

// access global variables
#include "../core/globals.h"

// access interfaces
#include "../core/interfaces.h"

// custom clamp function bc who uses std
inline float fclamp(float value, float min, float max) {
  if (value < min) {
    return min;
  } else if (value > max) {
    return max;
  } else {
    return value;
  }
}

// custom remainder function bc who uses std
inline float fremainder(float value, float period) {
  float result = std::fmodf(value, period);
  if (result < 0) {
    result += period;
  }
  return result;
}

void f::RunBunnyHop(CUserCmd* cmd) noexcept {
  if (!(g::localPlayer->GetFlags() & CEntity::FL_ONGROUND))
    cmd->buttons &= ~CUserCmd::IN_JUMP;
}

void f::RunRadar() noexcept {
  for (int i = 1; i <= i::globals->maxClients; ++i) {
    CEntity* entity = i::entityList->GetEntityFromIndex(i);
    if (!entity) continue;

    if (entity->IsDormant() || !entity->IsAlive()) continue;

    if (entity->GetTeam() == g::localPlayer->GetTeam()) continue;

    entity->Spotted() = true;
  }
}

void f::IgnoreFlash(float alpha) noexcept {
  // local player check
  if (!g::localPlayer) return;
  g::localPlayer->FlashAlpha() = alpha;
}

CVector oldPunch{0, 0, 0};
void f::RecoilControl(CUserCmd* cmd) noexcept {
  // local player check
  if (!g::localPlayer) return;
  // check if we are trying to shoot
  if ((cmd->buttons & CUserCmd::IN_ATTACK)) {
    CEntity* activeWeapon = g::localPlayer->GetActiveWeapon();
    if (!activeWeapon) return;
    const int weaponType = activeWeapon->GetWeaponType();

    // aim punch
    CVector aimPunch{0, 0, 0};

    switch (weaponType) {
      case CEntity::WEAPONTYPE_RIFLE:
      case CEntity::WEAPONTYPE_SUBMACHINEGUN:
      case CEntity::WEAPONTYPE_MACHINEGUN:
        g::localPlayer->GetAimPunch(aimPunch);
        break;
      default:
        break;
    }

    // view angles
    CVector viewAngles = cmd->viewAngles;

    auto newAngles = CVector{viewAngles.x + oldPunch.x - aimPunch.x,
                             viewAngles.y + oldPunch.y - aimPunch.y, 0};

    // limit is 89 and -89, if bigger quick vac :0
    newAngles.x = fclamp(newAngles.x, -89.f, 89.f);
    newAngles.y = fremainder(newAngles.y, 360.f);

    // no need to limit newAngles.z, it's always 0
    if (newAngles.x != cmd->viewAngles.x || newAngles.y != cmd->viewAngles.y)
      cmd->viewAngles = newAngles;
    oldPunch = aimPunch;
  } else {
    oldPunch = {0, 0, 0};
  }
}