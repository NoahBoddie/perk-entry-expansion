#pragma once

#include "RE/BarterSetting.h"

#include "VendorOverride/OverrideHandler.h"

namespace PEE::VOVR
{
	struct OverrideStolenHook
	{
		DECLARE_ALLOC()

		struct AE
		{
			DECLARE_ALLOC_IF(REL::Module::IsAE())

			static void Install()
			{
				//SE: NA, 1.6.640: 892B00
				auto address = REL::Relocation<uintptr_t>{ REL::ID(50960) }.address();

				auto& trampoline = SKSE::GetTrampoline();

				func = trampoline.write_call<5>(address + 0x266, thunk);
			}

			static bool thunk(RE::InventoryEntryData* a_this, RE::Actor* a2, RE::TESForm* owner, bool a4)
			{
				auto result = func(a_this, a2, owner, a4);

				if (result) {
					Impl(a_this, owner, result);
				}

				return result;
			}

			inline static REL::Relocation<decltype(thunk)> func;
		};



		static void Install()
		{
			//SE: 855B60, 1.6.640: 894B70
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(50061, 51001) }.address();

			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(address + 0xD4, thunk);
		}
		static void Impl(RE::InventoryEntryData* a_this, RE::TESForm* owner, bool& out)
		{
			if (RE::NiPointer<RE::TESObjectREFR> ref = nullptr; RE::TESObjectREFR::LookupByHandle(RE::BarterMenu::GetTargetRefHandle(), ref) == true) {
				OverrideHandler::UpdateOverride(ref->As<RE::Actor>(), a_this, nullptr, &out, owner);
			}
		}


		//make this an impl function
		static bool thunk(RE::InventoryEntryData* a_this, RE::Actor* a2, RE::TESForm* owner, bool a4)
		{
			auto result = func(a_this, a2, owner, a4);

			if (result) {
				Impl(a_this, owner, result);
			}

			return true;

		}

		inline static REL::Relocation<decltype(thunk)> func;
	};
}