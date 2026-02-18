#pragma once

#include "Multishot/SpreadHandler.h"

namespace PEE::MULT
{
	struct UpdateArrowUIHook
	{
		DECLARE_ALLOC()

		static void Install()
		{
			//SE: 74B170, 1.6.640: 78A7B0
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(42928, 44108) }.address();

			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(address + RELOCATION_OFFSET(0x63A, 0x60C), thunk);
		}

		static uint32_t thunk(RE::InventoryEntryData* a_this)
		{
			auto result = func(a_this);

			if (SpreadHandler::isActive) {
				if (result <= SpreadHandler::projCount) {
					result = 1;
				}
				else {
					result -= SpreadHandler::projCount - 1;
				}
			}

			return result;
		}

		inline static REL::Relocation<decltype(thunk)> func;
	};
}