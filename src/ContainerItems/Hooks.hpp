#pragma once

#include "Hooks/Hook_InitInventory.h"
#include "Hooks/Hook_LoadInventory.h"
#include "Hooks/Hook_ResetInventory.h"

namespace PEE::CONT
{
	inline void Install()
	{
		InstallHook<InitInventoryHook>();
		InstallHook<LoadInventoryHook>();
		InstallHook<ResetInventoryHook>();
	}
}