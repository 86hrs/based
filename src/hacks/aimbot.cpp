#include "aimbot.h"

#include "../core/globals.h"
#include "../core/interfaces.h"

void f::RunAimbot(CUserCmd* cmd, int aimbotBone, int fov,
                      int smoothness) noexcept {
  // check if we are trying to shoot
  if (!(cmd->buttons & CUserCmd::IN_ATTACK)) return;

  if (g::localPlayer->IsDefusing()) return;

  CEntity* activeWeapon = g::localPlayer->GetActiveWeapon();

  if (!activeWeapon) return;

  const int weaponType = activeWeapon->GetWeaponType();

  switch (weaponType) {
    case CEntity::WEAPONTYPE_MACHINEGUN:
    case CEntity::WEAPONTYPE_RIFLE:
    case CEntity::WEAPONTYPE_SHOTGUN:
    case CEntity::WEAPONTYPE_SNIPER:
    case CEntity::WEAPONTYPE_PISTOL: {
      if (!activeWeapon->GetClip()) return;

      if (weaponType == CEntity::WEAPONTYPE_SNIPER) {
        if (!g::localPlayer->IsScoped()) return;
      }

      break;
    }

    default:
      return;
  }

  CVector bestAngle{};
  float bestFov = float(fov);

  for (int i = 1; i <= i::globals->maxClients; ++i) {
    CEntity* entity = i::entityList->GetEntityFromIndex(i);

    if (!entity) continue;

    if (entity->IsDormant() || !entity->IsAlive()) continue;

    if (entity->GetTeam() == g::localPlayer->GetTeam()) continue;

    if (entity->HasGunGameImmunity()) continue;

    // player's bone matrix
    CMatrix3x4 bones[128];
    if (!entity->SetupBones(bones, 128, 256, i::globals->currentTime))
      continue;

    // our eye position
    CVector localEyePosition;
    g::localPlayer->GetEyePosition(localEyePosition);

    // our aim punch
    CVector aimPunch{};

    switch (weaponType) {
      case CEntity::WEAPONTYPE_RIFLE:
      case CEntity::WEAPONTYPE_SUBMACHINEGUN:
      case CEntity::WEAPONTYPE_MACHINEGUN:
        g::localPlayer->GetAimPunch(aimPunch);
    }

    CTrace trace;
    i::engineTrace->TraceRay(
        CRay{localEyePosition, bones[aimbotBone].Origin()}, MASK_SHOT,
        CTraceFilter{g::localPlayer}, trace);

    if (!trace.entity || trace.fraction < 0.97f) return;

    CVector enemyAngle{
        (bones[aimbotBone].Origin() - localEyePosition).ToAngle() -
        (cmd->viewAngles + aimPunch)};

    if (const float fov = std::hypot(enemyAngle.x, enemyAngle.y);
        fov < bestFov) {
      bestFov = fov;
      bestAngle = enemyAngle;
    }
  }

  cmd->viewAngles = cmd->viewAngles + bestAngle.Scale(smoothness);
}
