#pragma once

#include "Hooks/PotionApplySpell.h"

#pragma once

namespace PEE::PACS
{
	inline static void Install()
	{
		InstallHook<Potion__ApplySpell>();
	}
}