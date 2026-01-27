#pragma once

#include "ModifyPrice/PriceHandler.h"

namespace PEE::MPRC
{
	struct GetStandardItemDataHook
	{
		DECLARE_ALLOC()

		static void Install()
		{
			//SE: 8527B0, 1.6.640: 8916B0
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(50005, 50949) }.address();

			//Replace 1401D66E0 as well.
			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(address + 0x66, thunk);
		}


		static RE::StandardItemData* thunk(RE::ItemList* a_this)
		{
			auto result = func(a_this);
			
			if (result && result->objDesc)
			{

				//If it has no owner it's more likely the other guys

				if (RE::NiPointer<RE::TESObjectREFR> ref = nullptr; RE::TESObjectREFR::LookupByHandle(result->owner, ref) == true)
				{
					PriceHandler::SetBuySell(kSelling);
				}
				else
				{
					PriceHandler::SetBuySell(kBuying);

				}
			}
			return result;
		}

		inline static REL::Relocation<decltype(thunk)> func;
	};
}