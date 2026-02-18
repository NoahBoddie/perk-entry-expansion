#pragma once

#include "SpreadHandler.h"

#include "RE/ChargeTimer.h"

namespace PEE::MULT
{
	struct ArrowReleaseHook
	{
		DECLARE_ALLOC()

		static void Install()
		{
			//SE: 7220B0, 1.6.640: 75F320
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(41778, 42859) }.address();

			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(address + RELOCATION_OFFSET(0x133, 0x138), thunk);
		}

		static void thunk(RE::TESObjectWEAP* a_this, RE::TESObjectREFR* source, RE::TESAmmo* overwriteAmmo, RE::EnchantmentItem* ammoEnchantment, RE::AlchemyItem* poison)
		{
			{
				std::lock_guard guard{ SpreadHandler::mutex };
				//Store the source here.
				SpreadHandler::SetActive(true, source);
				func(a_this, source, overwriteAmmo, ammoEnchantment, poison);
				SpreadHandler::SetActive(false, nullptr);
			}
			static auto player = RE::PlayerCharacter::GetSingleton();
			if (source == player) {
				auto& charge_timer = reinterpret_cast<RE::BSTSmallArray<RE::ChargeTimer, 2>&>(player->GetPlayerRuntimeData().unkBA0);
				if (charge_timer.empty() == false)
					charge_timer.erase(charge_timer.begin());
			}
		}

		inline static REL::Relocation<decltype(thunk)> func;
	};
}