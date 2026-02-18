#pragma once

#include "SpeechCheck/Hooks.hpp"
#include"Multishot/Hooks.hpp"
#include"ModifyPrice/Hooks.hpp"
#include"VendorOverride/Hooks.hpp"
#include"ScrollCost/Hooks.hpp"
#include"MerchantWares/Hooks.hpp"
#include"ContainerItems/Hooks.hpp"
#include "MagicApplyCombatSpell/Hooks/Hook_MagicApplyCombatSpell.h"
#include "PotionApplySpell/Hooks/PotionApplySpell.h"

namespace PEE
{
	inline static void Install()
	{
		SKSE::AllocTrampoline(Allocator::GetAlloc());

		MULT::Install();
		MPRC::Install();
		VOVR::Install();
		SCRC::Install();
		WARES::Install();
		SPCK::Install();
		CONT::Install();

		InstallHook<ApplyHitMagicHitSpells>();
		InstallHook<Potion__ApplySpell>();
	}
}