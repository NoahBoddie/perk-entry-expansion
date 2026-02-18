#pragma once

#include "Hooks/Hook_ScrollCost.h"

namespace PEE::SCRC
{
	inline static void Install()
	{
		InstallHook<ScrollCostHook>();
	}
}