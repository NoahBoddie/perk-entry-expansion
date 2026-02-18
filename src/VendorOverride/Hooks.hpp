#pragma once

#include "Hooks/Hook_OverrideKeywordCheck.h"
#include "Hooks/Hook_OverrideStolenCheck.h"

namespace PEE::VOVR
{
	inline static void Install()
	{
		InstallHook<OverrideKeywordHook>();
		InstallHook<OverrideStolenHook>();
	}
}