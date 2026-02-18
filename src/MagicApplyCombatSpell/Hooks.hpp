#pragma once

#include "Hooks/Hook_MagicApplyCombatSpell.h"

#pragma once

namespace PEE::MACS
{
	inline static void Install()
	{
		InstallHook<ApplyHitMagicHitSpells>();
	}
}