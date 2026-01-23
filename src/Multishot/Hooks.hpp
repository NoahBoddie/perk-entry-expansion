#pragma once

#include "Hooks/Hook_AdjustAngle.h"
#include "Hooks/Hook_ArrowRelease.h"
#include "Hooks/Hook_GetPlayerBowTime.h"
#include "Hooks/Hook_ProjectileCount.h"

#pragma once

namespace PEE::MULT
{
	inline static void Install()
	{
		//This hook has been canned.
		return;
		InstallHook<AdjustAngleHook>();
		InstallHook<ArrowReleaseHook>();
		InstallHook<ProjectileCountHook>();
		InstallHook<GetPlayerBowTimeHook>();
	}
}