#pragma once

#include "RE/BarterSetting.h"

#include "VendorOverride/OverrideHandler.h"

namespace PEE::VOVR
{
	struct OverrideKeywordHook
	{
		struct AE
		{
			DECLARE_ALLOC_IF(REL::Module::IsAE())

			static void Install()
			{
				//SE: NA, 1.6.640: 892B00
				auto address = REL::Relocation<uintptr_t>{ REL::ID(50960) }.address();

				auto& trampoline = SKSE::GetTrampoline();

				func = trampoline.write_call<5>(address + 0x20B, thunk);
			}


			static bool thunk(RE::Character* a_this, RE::TESBoundObject* item, char a3, char a4, char a5, bool ignoreKeyword)
			{
				OverrideHandler::UpdateOverride(a_this, item, &ignoreKeyword, nullptr);
				return func(a_this, item, a3, a4, a5, ignoreKeyword);
			}


			inline static REL::Relocation<decltype(thunk)> func;

		};


		DECLARE_ALLOC()

		static void Install()
		{

			//SE: 855B60, 1.6.640: 894B70
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(50061, 51001) }.address();

			static ProloguePatch patch{ address, 5 };

			auto& trampoline = SKSE::GetTrampoline();


			auto call_or_jmp = IsCallOrJump(address);

			auto place_query = trampoline.write_branch<5>(address, (uintptr_t)thunk);

			if (call_or_jmp != OperEnum::Jump)
				func = (uintptr_t)patch.getCode();
			else
				func = place_query;

			if (REL::Module::IsAE()) {
				AE::Install();
			}
		}

		


		
		static bool thunk(RE::BarterSetting* args, RE::InventoryEntryData* item)
		{
			//OverrideHandler::UpdateOverride(args->merchant, item->object, &args->ignoreKeyword, args->target == RE::BarterSetting::kPlayer ? &args->ignoreStolen : nullptr);
			OverrideHandler::UpdateOverride(args->merchant, item->object, &args->ignoreKeyword, nullptr);
			return func(args, item);
		}


		inline static REL::Relocation<decltype(thunk)> func;
	};
}