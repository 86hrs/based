#include "../core/globals.h"
#include "../core/interfaces.h"

#include "esp.h"

static bool WorldToScreen(const CVector& point, CVector& screen) noexcept {
  // get the w2s matrix
  const CMatrix4x4& matrix = i::engine->WorldToScreenMatrix();

  // calc width first to test whether on screen or not
  float w = matrix.data[3][0] * point.x + matrix.data[3][1] * point.y +
            matrix.data[3][2] * point.z + matrix.data[3][3];

  // not on screen
  if (w < 0.001f) return false;

  float inverse = 1.f / w;

  screen.x = (matrix.data[0][0] * point.x + matrix.data[0][1] * point.y +
              matrix.data[0][2] * point.z + matrix.data[0][3]) *
             inverse;
  screen.y = (matrix.data[1][0] * point.x + matrix.data[1][1] * point.y +
              matrix.data[1][2] * point.z + matrix.data[1][3]) *
             inverse;

  int x, y;
  i::engine->GetScreenSize(x, y);

  screen.x = (x * 0.5f) + (screen.x * x) * 0.5f;
  screen.y = (y * 0.5f) - (screen.y * y) * 0.5f;

  // on screen
  return true;
}

void f::esp(std::uintptr_t vguiPanel, bool forceRepaint,
                bool allowForce) noexcept {
  // make sure we have the right panel
  if (vguiPanel == i::engineVGui->GetPanel(PANEL_TOOLS)) {
    // make sure we are in-game
    if (i::engine->IsInGame() && g::localPlayer) {
      // loop through players
      for (int i = 1; i <= i::globals->maxClients; ++i) {
        // get the player
        CEntity* player = i::entityList->GetEntityFromIndex(i);

        // make sure player is valid
        if (!player) continue;

        // make sure they aren't dormant && are alive
        if (player->IsDormant() || !player->IsAlive()) continue;

        // no esp on teammates
        if (player->GetTeam() == g::localPlayer->GetTeam()) continue;

        // dont do esp on who we are spectating
        if (!g::localPlayer->IsAlive())
          if (g::localPlayer->GetObserverTarget() == player) continue;

        // player's bone matrix
        CMatrix3x4 bones[128];
        if (!player->SetupBones(bones, 128, 0x7FF00,
                                i::globals->currentTime))
          continue;

        // screen position of head
        // we add 11.f here because we want the box ABOVE their head
        CVector top;
        if (!WorldToScreen(bones[8].Origin() + CVector{0.f, 0.f, 11.f}, top))
          continue;

        // screen position of feet
        // we subtract 9.f here because we want the box BELOW their feet
        CVector bottom;
        if (!WorldToScreen(player->GetAbsOrigin() - CVector{0.f, 0.f, 9.f},
                           bottom))
          continue;

        // the height of the box is the difference between
        // the bottom (larger number) and the top
        const float h = bottom.y - top.y;

        // we can use the height to determine a width
        const float w = h * 0.3f;

        const auto left = static_cast<int>(top.x - w);
        const auto right = static_cast<int>(top.x + w);

        // set the color to white
        i::surface->DrawSetColor(255, 255, 255, 255);

        // draw the normal box
        i::surface->DrawOutlinedRect(left, top.y, right, bottom.y);

        // set the color to black for outlines
        i::surface->DrawSetColor(0, 0, 0, 255);

        // normal box outline
        i::surface->DrawOutlinedRect(left - 1, top.y - 1, right + 1,
                                              bottom.y + 1);
        i::surface->DrawOutlinedRect(left + 1, top.y + 1, right - 1,
                                              bottom.y - 1);

        // health bar outline (use the black color here)
        i::surface->DrawOutlinedRect(left - 6, top.y - 1, left - 3,
                                              bottom.y + 1);

        // health is an integer from 0 to 100
        // we can make it a percentage by multipying
        // it by 0.01
        const float healthFrac = player->GetHealth() * 0.01f;

        // set the color of the health bar to a split between red / green
        i::surface->DrawSetColor((1.f - healthFrac) * 255,
                                          255 * healthFrac, 0, 255);

        // draw it
        i::surface->DrawFilledRect(
            left - 5, bottom.y - (h * healthFrac), left - 4, bottom.y);
      }
    }
  }
}