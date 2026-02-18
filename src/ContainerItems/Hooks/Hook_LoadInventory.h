#pragma once


namespace PEE::CONT
{

	//Hooks BGSEntryPointPerkEntry__AddLeveledList to force it to initialize the inventory if successful
	struct LoadInventoryHook
	{
		DECLARE_ALLOC()

		static void Install()
		{
			//SE: 32F640, 1.6.640: 346DC0
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(23087, 23540) }.address();

			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(address + RELOCATION_OFFSET(0xF1, 0xF2), thunk);
		}


		static RE::InventoryChanges* thunk(RE::TESObjectREFR* a_this)
		{
			auto result = func(a_this);

			if (!result && PEPE::GetCurrentCategory() == perkCategory || PEPE::GetCurrentCategory() == "Reward") {
				result = a_this->GetInventoryChanges(false);
			}

			return result;
		}

		static inline REL::Relocation<decltype(thunk)> func;
	};

}