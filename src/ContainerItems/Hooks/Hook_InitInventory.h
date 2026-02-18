#pragma once

#include "ContainerItems/ContainerHandler.h"

namespace PEE::CONT
{

	struct InitInventoryHook
	{
		static void Install()
		{
			//SE: 2850E0, 1.6.640: 298AB0
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(19105, 19507) }.address();

			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(address + RELOCATION_OFFSET(0x69A, 0x78C), thunk);
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