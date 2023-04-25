#include "gui_hooks.h"

#include <intrin.h>

#include <stdexcept>

#include "../../ext/imgui/imgui.h"
#include "../../ext/imgui/imgui_impl_dx9.h"
#include "../../ext/imgui/imgui_impl_win32.h"
#include "../../ext/minhook/minhook.h"

void h::guiSetup() {
  MH_Initialize();

  if (MH_CreateHook(VirtualFunction(gui::device, 42), &EndScene,
                    reinterpret_cast<void**>(&EndSceneOriginal)))
    throw std::runtime_error("Unable to hook EndScene()");

  if (MH_CreateHook(VirtualFunction(gui::device, 16), &Reset,
                    reinterpret_cast<void**>(&ResetOriginal)))
    throw std::runtime_error("Unable to hook Reset()");

  if (MH_EnableHook(MH_ALL_HOOKS))
    throw std::runtime_error("Unable to enable hooks");

  gui::DestroyDirectX();
}

void h::guiDestroy() noexcept {
  MH_DisableHook(MH_ALL_HOOKS);
  MH_RemoveHook(MH_ALL_HOOKS);
  MH_Uninitialize();
}

long __stdcall h::EndScene(IDirect3DDevice9* device) noexcept {
  static const auto returnAddress = _ReturnAddress();

  const auto result = EndSceneOriginal(device, device);

  // stop endscene getting called twice
  if (_ReturnAddress() == returnAddress) return result;

  if (!gui::setup) gui::SetupMenu(device);

  if (gui::open) {
    gui::Render();
  }

  return result;
}

HRESULT __stdcall h::Reset(IDirect3DDevice9* device,
                               D3DPRESENT_PARAMETERS* params) noexcept {
  ImGui_ImplDX9_InvalidateDeviceObjects();
  const auto result = ResetOriginal(device, device, params);
  ImGui_ImplDX9_CreateDeviceObjects();
  return result;
}
