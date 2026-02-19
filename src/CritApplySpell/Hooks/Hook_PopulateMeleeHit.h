#pragma once

#include "CritApplySpell/CritWeaponHandler.h"

namespace PEE::CACS
{
	struct PopulateMeleeHitHook
	{

		DECLARE_ALLOC()

		static void Install()
		{

			//SE: 742850, 1.6.640: 7810E0

			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(42832, 44001) }.address();

			auto& trampoline = SKSE::GetTrampoline();

			ProloguePatch patch(address);

			auto call_or_jmp = IsCallOrJump(address);

			auto place_query = trampoline.write_branch<5>(address, (uintptr_t)thunk);

			if (call_or_jmp != OperEnum::Jump)
				func = patch.MoveInstructions(trampoline);
			else
				func = place_query;

		}


		static void thunk(RE::HitData* a_this, RE::Actor* aggressor, RE::Actor* target, RE::InventoryEntryData* weapon, uint8_t a5)
		{
			CritWeaponHandler::SetItem(weapon);

			func(a_this, aggressor, target, weapon, a5);

			CritWeaponHandler::ClearItem();

		}


		inline static REL::Relocation<decltype(thunk)> func;
	};

}