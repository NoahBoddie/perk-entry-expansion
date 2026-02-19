#pragma once

#include "RE/BarterSetting.h"

#include "VendorOverride/OverrideHandler.h"

namespace PEE::VOVR
{
	struct OverrideKeywordHook
	{
		struct AE
		{
			inline static RE::InventoryEntryData* item = nullptr;

			struct Hook1
			{

				DECLARE_ALLOC_IF(REL::Module::IsAE())

				static void Install()
				{
					//SE: NA, 1.6.640: 892B00
					auto address = REL::Relocation<uintptr_t>{ REL::ID(50960) }.address();

					auto& trampoline = SKSE::GetTrampoline();

					func = trampoline.write_call<5>(address + 0x1DF, thunk);
				}


				static bool thunk(RE::InventoryEntryData* a_this)
				{
					auto result = func(a_this);

					if (result) {
						item = a_this;
					}

					return result;
				}

				inline static REL::Relocation<decltype(thunk)> func;
			};

			struct Hook2
			{
				DECLARE_ALLOC_IF(REL::Module::IsAE())

				static void Install()
				{
					//SE: NA, 1.6.640: 892B00
					auto address = REL::Relocation<uintptr_t>{ REL::ID(50960) }.address();

					auto& trampoline = SKSE::GetTrampoline();

					func = trampoline.write_call<5>(address + 0x20B, thunk);
				}


				static bool thunk(RE::Character* a_this, RE::TESBoundObject* obj, char a3, char a4, char a5, bool ignoreKeyword)
				{
					RE::InventoryEntryData dummy{ obj, 0 };

					OverrideHandler::UpdateOverride(a_this, item ? item : &dummy, &ignoreKeyword, nullptr);
					item = nullptr;
					return func(a_this, obj, a3, a4, a5, ignoreKeyword);
				}


				inline static REL::Relocation<decltype(thunk)> func;
			};
			

		};


		DECLARE_ALLOC()

		static void Install()
		{

			//SE: 855B60, 1.6.640: 894B70
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(50061, 51001) }.address();

			//static ProloguePatch patch{ address, 5 };

			auto& trampoline = SKSE::GetTrampoline();

			ProloguePatch patch(address);

			auto call_or_jmp = IsCallOrJump(address);

			auto place_query = trampoline.write_branch<5>(address, (uintptr_t)thunk);

			if (call_or_jmp != OperEnum::Jump)
				func = patch.MoveInstructions(trampoline);
			else
				func = place_query;

			if (REL::Module::IsAE()) {
				AE::Hook1::Install();
				AE::Hook2::Install();
			}
		}

		


		
		static bool thunk(RE::BarterSetting* args, RE::InventoryEntryData* item)
		{
			//OverrideHandler::UpdateOverride(args->merchant, item->object, &args->ignoreKeyword, args->target == RE::BarterSetting::kPlayer ? &args->ignoreStolen : nullptr);
			OverrideHandler::UpdateOverride(args->merchant, item, &args->ignoreKeyword, nullptr);
			return func(args, item);
		}


		inline static REL::Relocation<decltype(thunk)> func;
	};
}