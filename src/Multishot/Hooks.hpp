#pragma once

#include "Hooks/Hook_AdjustAngle.h"
#include "Hooks/Hook_ArrowRelease.h"
#include "Hooks/Hook_GetPlayerBowTime.h"
#include "Hooks/Hook_ProjectileCount.h"
#include "Hooks/Hook_ArrowCost.h"
#include "Hooks/Hook_UpdateArrowUI.h"
#pragma once

namespace PEE::MULT
{
	inline static void Install()
	{
		return;
		InstallHook<UpdateArrowUIHook>();
		InstallHook<ArrowCostHook>();
		InstallHook<AdjustAngleHook>();
		InstallHook<ArrowReleaseHook>();
		InstallHook<ProjectileCountHook>();
		InstallHook<GetPlayerBowTimeHook>();
	}
}