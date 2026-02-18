#pragma once

#include "Hooks/Hook_CritApplySpell.h"

#pragma once

namespace PEE::CACS
{
	inline static void Install()
	{
		InstallHook<Crit__ApplyCombatSpell>();
	}
}