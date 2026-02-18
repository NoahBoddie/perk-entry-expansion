#pragma once

#include "SpreadHandler.h"

namespace PEE::MULT
{
	struct ProjectileCountHook
	{
		DECLARE_ALLOC()

		static void Install()
		{
			//SE: 235240, 1.6.640: 78A7B0
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(17693, 44108) }.address();

			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(address + RELOCATION_OFFSET(0xC0D, 0xC41), thunk);
		}


		static uint8_t thunk(RE::TESObjectWEAP* weapon)
		{
			auto result = func(weapon);

			if (SpreadHandler::isActive)
			{
				if (SpreadHandler::source && SpreadHandler::source->IsPlayerRef()) {
					static uint8_t add = 254;
					result += add;
					result = SpreadHandler::SetCount(result);
				}
			}

			return result;
		}

		inline static REL::Relocation<decltype(thunk)> func;
	};
}