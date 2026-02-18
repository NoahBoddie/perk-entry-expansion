#pragma once

#include "ContainerItems/ContainerHandler.h"

namespace PEE::WARES
{
	//move this to container stuff
	
	struct ResetMerchantChestHook
	{
		DECLARE_ALLOC()

		static void Install()
		{
			//SE: 3551C0+5C AE: 36DB60+5C

			REL::RelocationID reset_call{ 23998, 24495 };

			auto& trampoline = SKSE::GetTrampoline();

			//No func because it's a vtable call. If someone else overrides the same place, I can try to detect if what was there was a call or not
			// and if so place into func. But, not needed for not.
			trampoline.write_call<6>(reset_call.address() + 0x5C, thunk);
		}


		static void thunk(RE::TESObjectREFR* a_this, bool a2)
		{
			CONT::ContainerHandler::ignoreThread = std::this_thread::get_id();

			a_this->ResetInventory(a2);

			RE::RefHandle merc_handle = RE::BarterMenu::GetTargetRefHandle();

			RE::ActorPtr merc = RE::Actor::LookupByHandle(merc_handle);

			if (merc)
			{
				RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
				RE::Actor* merchant = merc.get();


				auto result = RE::HandleEntryPoint(perkEntry, player, a_this, perkCategory, std::pair(merchant, a_this));
				assert(result == PEPE::RequestResult::Success);
			}

			CONT::ContainerHandler::ignoreThread = {};
		}




		//static inline REL::Relocation<decltype(thunk)> func;
	};

}