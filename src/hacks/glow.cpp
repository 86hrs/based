#include "glow.h"

#include "../core/globals.h"
#include "../core/interfaces.h"

void f::glow() noexcept {
  // make local player is valid && we are in game
  if (g::localPlayer && i::engine->IsInGame()) {
    for (int i = 0; i < i::glow->glowObjects.size; ++i) {
      // get the glow object
      IGlowManager::CGlowObject& glowObject = i::glow->glowObjects[i];

      // make sure it is used
      if (glowObject.IsUnused()) continue;

      switch (glowObject.entity->GetClientClass()->classID) {
        case CClientClass::CCSPlayer:
          if (!glowObject.entity->IsAlive()) break;
          // enemies
          if (g::guiEnemyGlow)
            if (glowObject.entity->GetTeam() != g::localPlayer->GetTeam())
              glowObject.SetColor(g::enemyGlowColor[0],
                                  g::enemyGlowColor[1],
                                  g::enemyGlowColor[2]);
            // teammates
            else if (g::guiTeamGlow)
              glowObject.SetColor(g::teamGlowColor[0],
                                  g::teamGlowColor[1],
                                  g::teamGlowColor[2]);
          break;
        default:
          break;
      }
    }
  }
}