#pragma once

namespace PEE::MPRC
{
	enum BarterEnum : uint8_t
	{
		kBuying,
		kSelling,
	};

	struct PriceHandler
	{
		inline static std::atomic<std::thread::id> currentThread{};//Used to make sure we should be altering anything
		inline static BarterEnum barterType{};


		//I wish to consume price modifiers so I can have a before and after perk application.
		static void SetBuySell(BarterEnum bt)
		{
			currentThread = std::this_thread::get_id();

			barterType = bt;
		}

		static void ApplyBuySell(RE::InventoryEntryData* data, uint32_t& price)
		{
			if (std::this_thread::get_id() == currentThread)
			{
				auto handle = RE::BarterMenu::GetTargetRefHandle();

				if (RE::NiPointer<RE::TESObjectREFR> ref = nullptr; RE::TESObjectREFR::LookupByHandle(handle, ref) == true)
				{

					static RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
					static RE::TESDataHandler* handler = RE::TESDataHandler::GetSingleton();

					auto inventory = handler->GetMerchantInventory();

					auto stock = inventory->owner;

					price = 999;

				}


				
					
				currentThread = std::thread::id{};
			}

			
		}

	};
}