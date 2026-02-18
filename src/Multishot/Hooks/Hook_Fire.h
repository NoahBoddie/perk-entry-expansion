#pragma once

#include "RE/ChargeTimer.h"

namespace PEE::MULT
{
	struct SomethingHook
	{
		//DECLARE_ALLOC()

		static void Install()
		{
			//SE: 235240, 1.6.640: 247110
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(17693, 18102) }.address();

			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(address + RELOCATION_OFFSET(0xC0D, 0xC41), thunk);
		}

		static void thunk(RE::TESObjectWEAP* a_this, RE::TESObjectREFR* source, RE::TESAmmo* overwriteAmmo, RE::EnchantmentItem* ammoEnchantment, RE::AlchemyItem* poison)
		{
			func(a_this, source, overwriteAmmo, ammoEnchantment, poison);
			static auto player = RE::PlayerCharacter::GetSingleton();
			auto& charge_timer = reinterpret_cast<RE::BSTSmallArray<RE::ChargeTimer, 2>&>(player->GetPlayerRuntimeData().unkBA0);
			if (charge_timer.empty() == false)
				charge_timer.erase(charge_timer.begin());
		}

		inline static REL::Relocation<decltype(thunk)> func;
	};
}