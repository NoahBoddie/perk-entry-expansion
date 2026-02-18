#pragma once

#include "Multishot/SpreadHandler.h"

namespace PEE::MULT
{
	struct ArrowCostHook
	{
		DECLARE_ALLOC()

		static void Install()
		{
			//SE: 235240, 1.6.640: 247110
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(17693, 18102) }.address();

			auto& trampoline = SKSE::GetTrampoline();

			trampoline.write_call<6>(address + RELOCATION_OFFSET(0x1124, 0x1205), thunk);
		}

		static uint32_t thunk(RE::Actor* a_this, uint32_t count)
		{
			return a_this->UseAmmo(SpreadHandler::projCount);
		}
	};
}