#pragma once
#include "../core/netvars.h"
#include "../util/memory.h"
#include "cclientclass.h"
#include "cmatrix.h"
#include "cvector.h"

class CEntity;
class IClientUnknown {
 public:
  constexpr CEntity* GetBaseEntity() noexcept {
    return m::Call<CEntity*>(this, 7);
  }
};

class IClientRenderable {
 public:
  constexpr IClientUnknown* GetIClientUnknown() noexcept {
    return m::Call<IClientUnknown*>(this, 0);
  }
};

class CModel;
class CEntity {
 public:
  enum EFlags : std::int32_t {
    FL_ONGROUND = (1 << 0),
    FL_DUCKING = (1 << 1),
    FL_ANIMDUCKING = (1 << 2),
    FL_WATERJUMP = (1 << 3),
    FL_ONTRAIN = (1 << 4),
    FL_INRAIN = (1 << 5),
    FL_FROZEN = (1 << 6),
    FL_ATCONTROLS = (1 << 7),
    FL_CLIENT = (1 << 8),
    FL_FAKECLIENT = (1 << 9),
    FL_INWATER = (1 << 10),
    FL_FLY = (1 << 11),
    FL_SWIM = (1 << 12),
    FL_CONVEYOR = (1 << 13),
    FL_NPC = (1 << 14),
    FL_GODMODE = (1 << 15),
    FL_NOTARGET = (1 << 16),
    FL_AIMTARGET = (1 << 17),
    FL_PARTIALGROUND = (1 << 18),
    FL_STATICPROP = (1 << 19),
    FL_GRAPHED = (1 << 20),
    FL_GRENADE = (1 << 21),
    FL_STEPMOVEMENT = (1 << 22),
    FL_DONTTOUCH = (1 << 23),
    FL_BASEVELOCITY = (1 << 24),
    FL_WORLDBRUSH = (1 << 25),
    FL_OBJECT = (1 << 26),
    FL_KILLME = (1 << 27),
    FL_ONFIRE = (1 << 28),
    FL_DISSOLVING = (1 << 29),
    FL_TRANSRAGDOLL = (1 << 30),
    FL_UNBLOCKABLE_BY_PLAYER = (1 << 31)
  };

  enum EWeaponType : int {
    WEAPONTYPE_KNIFE = 0,
    WEAPONTYPE_PISTOL = 1,
    WEAPONTYPE_SUBMACHINEGUN = 2,
    WEAPONTYPE_RIFLE = 3,
    WEAPONTYPE_SHOTGUN = 4,
    WEAPONTYPE_SNIPER = 5,
    WEAPONTYPE_MACHINEGUN = 6,
    WEAPONTYPE_C4 = 7,
    WEAPONTYPE_PLACEHOLDER = 8,
    WEAPONTYPE_GRENADE = 9,
    WEAPONTYPE_HEALTHSHOT = 11,
    WEAPONTYPE_FISTS = 12,
    WEAPONTYPE_BREACHCHARGE = 13,
    WEAPONTYPE_BUMPMINE = 14,
    WEAPONTYPE_TABLET = 15,
    WEAPONTYPE_MELEE = 16
  };

 public:  // netvars
  NETVAR(GetFlags, "CBasePlayer->m_fFlags", std::int32_t)
  NETVAR(IsScoped, "CCSPlayer->m_bIsScoped", bool)
  NETVAR(IsDefusing, "CCSPlayer->m_bIsDefusing", bool)
  NETVAR(HasGunGameImmunity, "CCSPlayer->m_bGunGameImmunity", bool)
  NETVAR(GetClip, "CBaseCombatWeapon->m_iClip1", int)
  NETVAR(Spotted, "CBaseEntity->m_bSpotted", bool)
  NETVAR(FOV, "CBasePlayer->m_iFOV", int)
  NETVAR(FlashAlpha, "CCSPlayer->m_flFlashMaxAlpha", float)
  NETVAR(DefaultFOV, "CBasePlayer->m_iDefaultFOV", int)

 public:  // renderable virtual functions (+0x4)
  constexpr CModel* GetModel() noexcept {
    return m::Call<CModel*>(this + 0x4, 8);
  }

  constexpr bool SetupBones(CMatrix3x4* out, std::int32_t max,
                            std::int32_t mask, float currentTime) noexcept {
    return m::Call<bool>(this + 0x4, 13, out, max, mask, currentTime);
  }

 public:  // networkable virtual functions (+0x8)
  constexpr CClientClass* GetClientClass() noexcept {
    return m::Call<CClientClass*>(this + 0x8, 2);
  }

  constexpr bool IsDormant() noexcept {
    return m::Call<bool>(this + 0x8, 9);
  }

  constexpr std::int32_t GetIndex() noexcept {
    return m::Call<std::int32_t>(this + 0x8, 10);
  }

 public:  // entity virtual functions
  constexpr const CVector& GetAbsOrigin() noexcept {
    return m::Call<const CVector&>(this, 10);
  }

  constexpr std::int32_t GetTeam() noexcept {
    return m::Call<std::int32_t>(this, 88);
  }

  constexpr std::int32_t GetHealth() noexcept {
    return m::Call<std::int32_t>(this, 122);
  }

  constexpr bool IsAlive() noexcept { return m::Call<bool>(this, 156); }

  constexpr bool IsPlayer() noexcept { return m::Call<bool>(this, 158); }

  constexpr bool IsWeapon() noexcept { return m::Call<bool>(this, 166); }

  constexpr CEntity* GetActiveWeapon() noexcept {
    return m::Call<CEntity*>(this, 268);
  }

  constexpr void GetEyePosition(CVector& eyePosition) noexcept {
    m::Call<void>(this, 285, std::ref(eyePosition));
  }

  constexpr CEntity* GetObserverTarget() noexcept {
    return m::Call<CEntity*>(this, 295);
  }

  constexpr void GetAimPunch(CVector& aimPunch) noexcept {
    m::Call<void>(this, 346, std::ref(aimPunch));
  }

  constexpr int GetWeaponType() noexcept {
    return m::Call<int>(this, 455);
  }
};
