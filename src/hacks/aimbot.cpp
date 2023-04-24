#include "aimbot.h"

#include "../core/globals.h"
#include "../core/interfaces.h"

void hacks::RunAimbot(CUserCmd* cmd, int aimbotBone, int fov,
                      int smoothness) noexcept {
  // check if we are trying to shoot
  if (!(cmd->buttons & CUserCmd::IN_ATTACK)) return;

  if (globals::localPlayer->IsDefusing()) return;

  CEntity* activeWeapon = globals::localPlayer->GetActiveWeapon();

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
        if (!globals::localPlayer->IsScoped()) return;
      }

      break;
    }

    default:
      return;
  }

  CVector bestAngle{};
  float bestFov = float(fov);

  for (int i = 1; i <= interfaces::globals->maxClients; ++i) {
    CEntity* entity = interfaces::entityList->GetEntityFromIndex(i);

    if (!entity) continue;

    if (entity->IsDormant() || !entity->IsAlive()) continue;

    if (entity->GetTeam() == globals::localPlayer->GetTeam()) continue;

    if (entity->HasGunGameImmunity()) continue;

    // player's bone matrix
    CMatrix3x4 bones[128];
    if (!entity->SetupBones(bones, 128, 256, interfaces::globals->currentTime))
      continue;

    // our eye position
    CVector localEyePosition;
    globals::localPlayer->GetEyePosition(localEyePosition);

    // our aim punch
    CVector aimPunch{};

    switch (weaponType) {
      case CEntity::WEAPONTYPE_RIFLE:
      case CEntity::WEAPONTYPE_SUBMACHINEGUN:
      case CEntity::WEAPONTYPE_MACHINEGUN:
        globals::localPlayer->GetAimPunch(aimPunch);
    }

    CTrace trace;
    interfaces::engineTrace->TraceRay(
        CRay{localEyePosition, bones[aimbotBone].Origin()}, MASK_SHOT,
        CTraceFilter{globals::localPlayer}, trace);

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
