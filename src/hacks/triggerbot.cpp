#include "triggerbot.h"

#include "../core/globals.h"
#include "../core/interfaces.h"

void f::RunTriggerBot(CUserCmd* cmd) noexcept {
  // get local player
  g::localPlayer =
      i::entityList->GetEntityFromIndex(i::engine->GetLocalPlayerIndex());

  // make sure local player is alive
  if (!g::localPlayer || !g::localPlayer->IsAlive()) return;

  CVector eyePosition;
  g::localPlayer->GetEyePosition(eyePosition);

  CVector aimPunch;
  g::localPlayer->GetAimPunch(aimPunch);

  // calculate the destination of the ray
  const CVector dst =
      eyePosition +
      CVector{cmd->viewAngles + aimPunch}.ToVector().Scale(10000.f);

  // trace the ray from eyes -> dest
  CTrace trace;
  i::engineTrace->TraceRay({eyePosition, dst}, 0x46004009, g::localPlayer,
                           trace);

  // make sure we hit a player
  if (!trace.entity || !trace.entity->IsPlayer()) return;

  // make sure player is alive & is an enemy
  if (!trace.entity->IsAlive() ||
      trace.entity->GetTeam() == g::localPlayer->GetTeam())
    return;

  // make our local player shoot
  cmd->buttons |= CUserCmd::IN_ATTACK;
}