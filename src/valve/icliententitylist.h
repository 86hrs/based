#pragma once
#include "centity.h"

class IClientEntityList
{
public:
	constexpr CEntity* GetEntityFromIndex(const std::int32_t index) noexcept
	{
		return m::Call<CEntity*>(this, 3, index);
	}
};
