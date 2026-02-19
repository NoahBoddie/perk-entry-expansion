#pragma once

#include "CritApplySpell/CritWeaponHandler.h"

namespace PEE::CACS
{
	struct PopulateProjectileHitHook
	{

		DECLARE_ALLOC()

		static void Install()
		{

			//SE: 742C00, 1.6.640: 781470

			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(42833, 44002) }.address();

			auto& trampoline = SKSE::GetTrampoline();
			
			ProloguePatch patch(address);

			auto call_or_jmp = IsCallOrJump(address);

			auto place_query = trampoline.write_branch<5>(address, (uintptr_t)thunk);

			if (call_or_jmp != OperEnum::Jump)
				func = patch.MoveInstructions(trampoline);
			else
				func = place_query;

		}


		static void thunk(RE::HitData* a_this, RE::Actor* aggressor, RE::Actor* target, RE::Projectile* projectile)
		{
			RE::InventoryEntryData dummy{ projectile->GetProjectileRuntimeData().weaponSource, 1 };

			RE::InventoryEntryData* weapon = aggressor->GetEquippedEntryData(false);

			CritWeaponHandler::SetItem(weapon && weapon->object == dummy.object ? weapon : &dummy);

			func(a_this, aggressor, target, projectile);

			CritWeaponHandler::ClearItem();

		}


		inline static REL::Relocation<decltype(thunk)> func;
	};

}