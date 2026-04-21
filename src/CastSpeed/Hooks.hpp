#pragma once

#include "Hooks/Hook_CastSpeedModify.h"

namespace PEE::CSPM
{
	inline static void Install()
	{
		InstallHook<CastSpeedModify>();
	}
}
