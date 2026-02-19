#pragma once

namespace PEE::CACS
{
	struct CritWeaponHandler
	{
		inline static RE::InventoryEntryData* currentWeapon = nullptr;
		inline static std::thread::id activeThread{};


		static void SetItem(RE::InventoryEntryData* item)
		{
			currentWeapon = item;
			activeThread = std::this_thread::get_id();
		}

		static void ClearItem()
		{
			currentWeapon = nullptr;
			activeThread = {};
		}
	};

}