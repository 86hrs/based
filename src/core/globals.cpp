#include "globals.h"

// expose interfaces to this file
#include "interfaces.h"

void g::UpdateLocalPlayer() noexcept {
  // get local player index from engine
  const std::int32_t localPlayerIndex =
      i::engine->GetLocalPlayerIndex();

  // get local player entity from client entity list
  localPlayer = i::entityList->GetEntityFromIndex(localPlayerIndex);
}
