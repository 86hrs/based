#pragma once
#include "cvector.h"

class IEffects
{
public:
	//
	// Particle effects
	//
	virtual void Beam(const CVector& Start, const CVector& End, int modelIndex,
		int nHaloIndex, unsigned char frameStart, unsigned char frameRate,
		float flLife, unsigned char width, unsigned char endWidth, unsigned char fadeLength,
		unsigned char noise, unsigned char red, unsigned char green,
		unsigned char blue, unsigned char brightness, unsigned char speed) = 0;

	//-----------------------------------------------------------------------------
	// Purpose: Emits smoke sprites.
	// Input  : origin - Where to emit the sprites.
	//			scale - Sprite scale * 10.
	//			framerate - Framerate at which to animate the smoke sprites.
	//-----------------------------------------------------------------------------
	virtual void Smoke(const CVector& origin, int modelIndex, float scale, float framerate) = 0;

	virtual void Sparks(const CVector& position, int nMagnitude = 1, int nTrailLength = 1, const CVector* dir = NULL) = 0;

	virtual void Dust(const CVector& pos, const CVector& dir, float size, float speed) = 0;

	virtual void MuzzleFlash(const CVector& origin, const CVector& angles, float scale, int iType) = 0;

	// like ricochet, but no sound
	virtual void MetalSparks(const CVector& position, const CVector& direction) = 0;

	virtual void EnergySplash(const CVector& position, const CVector& direction, bool explosive = false) = 0;

	virtual void Ricochet(const CVector& position, const CVector& direction) = 0;

	virtual float Time() = 0;
	virtual bool IsServer() = 0;

	virtual void SuppressEffectsSounds(bool bSuppress) = 0;
};
