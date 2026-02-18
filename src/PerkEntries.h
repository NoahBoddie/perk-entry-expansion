#pragma once

namespace PEE
{

	namespace WARES
	{
		inline constexpr auto perkCategory = "AddMerchantWares";
		inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kAddLeveledListOnDeath;
	}

	namespace MACS
	{
		static inline constexpr auto perkCategory = "MagicApplySpell";
		static inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kApplyCombatHitSpell;
	}

	namespace PACS 
	{
		static inline constexpr auto perkCategory = "PotionApplySpell";
		static inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kApplyCombatHitSpell;
	}


	namespace MULT
	{
		enum EP
		{
			kMultishot,
		};

		inline constexpr std::string_view perkCategory[]{ "Multishot" };
		inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kModBowZoom;
	}


	namespace SPCK
	{
		enum EP
		{
			kSpeechCheck,
			kSpeechSkill,
			kOverrideSpeechCheck,
		};

		inline constexpr std::string_view perkCategory[]{ "SpeechCheck", "SpeechSkill", "OverrideSpeechCheck"};

		inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kModFavorPoints;
	}

	namespace VOVR
	{
		enum EP
		{
			kBarterStolenItem,
			kBarterUnrelatedItem
		};

		inline constexpr std::string_view perkCategory[]{ "BarterStolenItem", "BarterUnrelatedItem" };
		inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kShouldApplyPlacedItem;
	}
	
	namespace SCRC
	{
		enum EP
		{
			kSaveScrollChance,
		};

		inline constexpr std::string_view perkCategory = "SaveScrollChance";
		inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kCanDualCastSpell;
	}

	namespace MPRC
	{
		enum EP
		{
			kModItemBuyPrice,
			kModItemSellPrice,
		};

		inline constexpr std::string_view perkCategory[]{ "ModItemBuyPrice", "ModifyItemSellPrice" };
		inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kModSecondaryValueWeight;
	}

	namespace CONT
	{
		inline constexpr std::string_view perkCategory = "AddContainerItem";
		inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kAddLeveledListOnDeath;
	}
}