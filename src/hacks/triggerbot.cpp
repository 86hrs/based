#include "triggerbot.h"

#include "../core/globals.h"
#include "../core/interfaces.h"

bool hacks::RunTriggerBot(CUserCmd* cmd) {
  // get local player
  globals::localPlayer = interfaces::entityList->GetEntityFromIndex(
      interfaces::engine->GetLocalPlayerIndex());

  // make sure local player is alive
  if (!globals::localPlayer || !globals::localPlayer->IsAlive()) return false;

  CVector eyePosition;
  globals::localPlayer->GetEyePosition(eyePosition);

  CVector aimPunch;
  globals::localPlayer->GetAimPunch(aimPunch);

  // calculate the destination of the ray
  const CVector dst =
      eyePosition +
      CVector{cmd->viewAngles + aimPunch}.ToVector().Scale(10000.f);

  // trace the ray from eyes -> dest
  CTrace trace;
  interfaces::engineTrace->TraceRay({eyePosition, dst}, 0x46004009,
                                    globals::localPlayer, trace);

  // make sure we hit a player
  if (!trace.entity || !trace.entity->IsPlayer()) return false;

  // make sure player is alive & is an enemy
  if (!trace.entity->IsAlive() ||
      trace.entity->GetTeam() == globals::localPlayer->GetTeam())
    return false;

  // make our local player shoot
  cmd->buttons |= CUserCmd::IN_ATTACK;
}