#pragma once

#include "ModifyPrice/PriceHandler.h"

namespace PEE::MPRC
{
	struct BarterGetValueHook
	{
		DECLARE_ALLOC()

		static void Install()
		{
			//SE: 890E70, 1.6.640: ???
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(51019, 0) }.address();

			auto& trampoline = SKSE::GetTrampoline();

			func[0] = trampoline.write_call<5>(address + RELOCATION_OFFSET(0x1F1, 0x0), thunk<0>);
			func[1] = trampoline.write_call<5>(address + RELOCATION_OFFSET(0xBE8, 0x0), thunk<1>);
		}

		
		static uint32_t thunkImpl(RE::InventoryEntryData* a_this, int I)
		{
			auto result = func[I](a_this);

			if (I != 0 || a_this->object->formType != RE::FormType::AlchemyItem)
				PriceHandler::ApplyBuySell(a_this, result);

			return result;
		}

		template<int I>
		static uint32_t thunk(RE::InventoryEntryData* a_this)
		{
			return thunkImpl(a_this, I);
		}

		inline static REL::Relocation<decltype(thunk<0>)> func[2];
	};
}