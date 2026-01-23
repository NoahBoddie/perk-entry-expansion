#pragma once

#include "Hooks/Hook_BarterGetValue.h"
#include "Hooks/Hook_GetStandardItemData.h"

#pragma once

namespace PEE::MPRC
{
	inline static void Install()
	{
		InstallHook<BarterGetValueHook>();
		InstallHook<GetStandardItemDataHook>();
	}
}