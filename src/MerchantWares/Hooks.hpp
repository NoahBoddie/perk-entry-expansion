#pragma once

#include "Hooks/Hook_ResetMerchantChest.h"

#pragma once

namespace PEE::WARES
{
	inline static void Install()
	{
		InstallHook<ResetMerchantChestHook>();
	}
}