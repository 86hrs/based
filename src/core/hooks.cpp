#include "hooks.h"

// include minhook for epic hookage
#include <intrin.h>

#include "../../ext/minhook/minhook.h"
#include "../core/globals.h"
#include "../hacks/aimbot.h"
#include "../hacks/esp.h"
#include "../hacks/glow.h"
#include "../hacks/misc.h"
#include "../hacks/triggerbot.h"

float* HealthToRGB(int health) {
  float* rgb = new float[3];
  if (health >= 0 && health <= 100) {
    float ratio = (float)health / 100;
    rgb[0] = 1 - ratio;  // red
    rgb[1] = ratio;      // green
    rgb[2] = 0;          // blue
  } else {
    rgb[0] = 0;
    rgb[1] = 0;
    rgb[2] = 0;
  }
  return rgb;
}

void h::Setup() noexcept {
  MH_Initialize();

  const auto keyValuesSystem = m::Get(i::keyValuesSystem, 1);
  const auto createMove = m::Get(i::clientMode, 24);
  const auto doPostScreenSpaceEffects = m::Get(i::clientMode, 44);
  const auto studioRender = m::Get(i::studioRender, 29);
  const auto paintTraverse = m::Get(i::panel, 41);
  const auto overrideView = m::Get(i::clientMode, 18);
  const auto dispatchUserMSG = m::Get(i::client, 38);
  // AllocKeyValuesMemory hook
  MH_CreateHook(keyValuesSystem, &AllocKeyValuesMemory,
                reinterpret_cast<void**>(&AllocKeyValuesMemoryOriginal));

  // CreateMove hook
  MH_CreateHook(createMove, &CreateMove,
                reinterpret_cast<void**>(&CreateMoveOriginal));

  // DoPostScreenSpaceEffects hook
  MH_CreateHook(doPostScreenSpaceEffects,  // function is @ index 44
                &DoPostScreenSpaceEffects,
                reinterpret_cast<void**>(
                    &DoPostScreenSpaceEffectsOriginal)  // og function
  );

  // DrawModel hook
  MH_CreateHook(studioRender, &DrawModel,
                reinterpret_cast<void**>(&DrawModelOriginal));

  // PaintTraverse hook
  MH_CreateHook(paintTraverse, &PaintTraverse,
                reinterpret_cast<void**>(&PaintTraverseOriginal));
  // OverideView hook
  MH_CreateHook(overrideView, &OverrideView,
                reinterpret_cast<void**>(&OverrideViewOriginal));

  // DispatchUserMessage hook
  MH_CreateHook(dispatchUserMSG, &DispatchUserMessage,
                reinterpret_cast<void**>(&DispatchUserMessageOriginal));

  MH_EnableHook(MH_ALL_HOOKS);
}

void h::Destroy() noexcept {
  // restore hooks
  MH_DisableHook(MH_ALL_HOOKS);
  MH_RemoveHook(MH_ALL_HOOKS);

  // uninit minhook
  MH_Uninitialize();
}

void* __stdcall h::AllocKeyValuesMemory(const std::int32_t size) noexcept {
  // if function is returning to speficied addresses, return nullptr to "bypass"
  if (const std::uint32_t address =
          reinterpret_cast<std::uint32_t>(_ReturnAddress());
      address == reinterpret_cast<std::uint32_t>(m::allocKeyValuesEngine) ||
      address == reinterpret_cast<std::uint32_t>(m::allocKeyValuesClient))
    return nullptr;

  // return original
  return AllocKeyValuesMemoryOriginal(i::keyValuesSystem, size);
}

bool __stdcall h::DispatchUserMessage(int type, uint32_t flags, int size,
                                      const void* message) {
  // if (g::localPlayer && i::engine->IsInGame())
  //   if (GetAsyncKeyState(VK_TAB)) DispatchUserMessage(50, 0, 0, nullptr);
  return DispatchUserMessageOriginal(i::client, type, flags, size, message);
}

bool __stdcall h::CreateMove(float frameTime, CUserCmd* cmd) noexcept {
  // make sure this function is being called from CInput::CreateMove
  if (!cmd->commandNumber)
    return CreateMoveOriginal(i::clientMode, frameTime, cmd);

  // get our local player here
  g::UpdateLocalPlayer();

  // run before aimbot becuase of silent aim problems
  if (g::ignoreRecoil) f::RecoilControl(cmd);

  // this would be done anyway by returning true
  if (CreateMoveOriginal(i::clientMode, frameTime, cmd))
    i::engine->SetViewAngles(cmd->viewAngles);

  if (g::localPlayer && g::localPlayer->IsAlive()) {
    // aimbot bone
    int iAimbotBone;
    switch (g::guiAimbotBone) {
      case 0:
        // head
        iAimbotBone = 8;
        break;
      case 1:
        // chest
        iAimbotBone = 6;
        break;
      case 2:
        // chest
        iAimbotBone = 7;
        break;
      case 3:
        // dick
        iAimbotBone = 0;
        break;
      case 4:
        // legs
        iAimbotBone = 72;
        break;
      default:
        iAimbotBone = 7;
        break;
    }
    if (g::guiAimbot)
      f::RunAimbot(cmd, iAimbotBone, g::aimbotFov, g::aimbotSmoothness);

    if (g::guiTriggerbot) f::RunTriggerBot(cmd);

    if (g::radar) f::RunRadar();

    if (g::guiBhop) f::RunBunnyHop(cmd);

    if (g::ignoreFlash)
      f::IgnoreFlash(30.f);
    else
      f::IgnoreFlash(255.f);
  }
  return false;
}

void __stdcall h::OverrideView(ViewSetup* setup) {
  if (!g::localPlayer) return OverrideViewOriginal(i::clientMode, setup);
  if (g::localPlayer->IsScoped())
    return OverrideViewOriginal(i::clientMode, setup);

  if (g::fov) setup->fov = g::fovValue;

  OverrideViewOriginal(i::clientMode, setup);
}
void __stdcall h::DoPostScreenSpaceEffects(const void* viewSetup) noexcept {
  f::glow();
  // call og function
  DoPostScreenSpaceEffectsOriginal(i::clientMode, viewSetup);
}

void __stdcall h::DrawModel(void* results, const CDrawModelInfo& info,
                            CMatrix3x4* bones, float* flexWeights,
                            float* FlexDelayedWeights,
                            const CVector& modelOrigin,
                            const std::int32_t flags) noexcept {
  if (g::guiChams) {
    // TODO move chams function to own file
    // make sure local player && renderable pointer != nullptr
    // or else *crash* :(
    if (g::localPlayer && info.renderable) {
      CEntity* entity = info.renderable->GetIClientUnknown()->GetBaseEntity();

      // make sure entity is a valid enemy player
      if (entity && entity->IsPlayer() &&
          entity->GetTeam() != g::localPlayer->GetTeam()) {
        // get our material to override
        static IMaterial* material =
            i::materialSystem->FindMaterial("debug/debugambientcube");

        // alpha modulate (once in my case)
        i::studioRender->SetAlphaModulation(1.f);

        // show through walls
        material->SetMaterialVarFlag(IMaterial::IGNOREZ, true);
        i::studioRender->SetColorModulation(g::hidden);
        i::studioRender->ForcedMaterialOverride(material);
        DrawModelOriginal(i::studioRender, results, info, bones, flexWeights,
                          FlexDelayedWeights, modelOrigin, flags);

        // do not show through walls
        material->SetMaterialVarFlag(IMaterial::IGNOREZ, false);
        if (g::bHealthBasedChams)
          i::studioRender->SetColorModulation(HealthToRGB(entity->GetHealth()));
        else
          i::studioRender->SetColorModulation(g::visible);

        i::studioRender->ForcedMaterialOverride(material);
        DrawModelOriginal(i::studioRender, results, info, bones, flexWeights,
                          FlexDelayedWeights, modelOrigin, flags);

        // reset material override + return from hook
        return i::studioRender->ForcedMaterialOverride(nullptr);
      }
    }
  }
  // call original DrawModel for things that aren't getting chamed
  DrawModelOriginal(i::studioRender, results, info, bones, flexWeights,
                    FlexDelayedWeights, modelOrigin, flags);
}

void __stdcall h::PaintTraverse(std::uintptr_t vguiPanel, bool forceRepaint,
                                bool allowForce) noexcept {
  if (g::esp) f::esp(vguiPanel, forceRepaint, allowForce);
  // call original function
  PaintTraverseOriginal(i::panel, vguiPanel, forceRepaint, allowForce);
}