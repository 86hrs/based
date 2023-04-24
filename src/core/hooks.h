#pragma once
#include "interfaces.h"

namespace hooks {
// call once to emplace all hooks
void Setup() noexcept;

// call once to restore all hooks
void Destroy() noexcept;

// bypass return address checks (thx osiris)
using AllocKeyValuesMemoryFn = void*(__thiscall*)(void*,
                                                  const std::int32_t) noexcept;
inline AllocKeyValuesMemoryFn AllocKeyValuesMemoryOriginal;
void* __stdcall AllocKeyValuesMemory(const std::int32_t size) noexcept;

// CreateMove hook
using CreateMoveFn = bool(__thiscall*)(IClientModeShared*, float,
                                       CUserCmd*) noexcept;
inline CreateMoveFn CreateMoveOriginal = nullptr;
bool __stdcall CreateMove(float frameTime, CUserCmd* cmd) noexcept;

// glow hook
using DoPostScreenSpaceEffectsFn = void(__thiscall*)(void*,
                                                     const void*) noexcept;
inline DoPostScreenSpaceEffectsFn DoPostScreenSpaceEffectsOriginal = nullptr;
void __stdcall DoPostScreenSpaceEffects(const void* viewSetup) noexcept;

// DrawModel hook
using DrawModelFn = void(__thiscall*)(void*, void*, const CDrawModelInfo&,
                                      CMatrix3x4*, float*, float*,
                                      const CVector&,
                                      const std::int32_t) noexcept;

inline DrawModelFn DrawModelOriginal = nullptr;
void __stdcall DrawModel(void* results, const CDrawModelInfo& info,
                         CMatrix3x4* bones, float* flexWeights,
                         float* flexDelayedWeights, const CVector& modelOrigin,
                         const std::int32_t flags) noexcept;

// PaintTravese Hook
using PaintTraverseFn = void(__thiscall*)(IVPanel*, std::uintptr_t, bool,
                                          bool) noexcept;
inline PaintTraverseFn PaintTraverseOriginal = nullptr;
void __stdcall PaintTraverse(std::uintptr_t vguiPanel, bool forceRepaint,
                             bool allowForce) noexcept;

}  // namespace hooks
