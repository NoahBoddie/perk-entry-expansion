#pragma once

#include "ContainerItems/ContainerHandler.h"

namespace PEE::CONT
{

	struct ResetInventoryHook
	{
		DECLARE_ALLOC()

		static void Install()
		{
			//SE: 298C80, 1.6.640: 2AD110
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(19375, 19802) }.address();

			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(address + RELOCATION_OFFSET(0x145, 0x12B), thunk);
		}


		static uint64_t thunk(RE::TESObjectREFR* a_this, bool a2)
		{
			auto result = func(a_this, a2);

			ContainerHandler::Handle(a_this);

			return result;
		}

		static inline REL::Relocation<decltype(thunk)> func;
	};


}