#pragma once

#include "Hooks/Hook_CritApplySpell.h"
#include "Hooks/Hook_PopulateMeleeHit.h"
#include "Hooks/Hook_PopulateProjectileHit.h"

#pragma once

namespace PEE::CACS
{
	inline static void Install()
	{
		InstallHook<Crit__ApplyCombatSpell>();
		InstallHook<PopulateMeleeHitHook>();
		InstallHook<PopulateProjectileHitHook>();
	}
}