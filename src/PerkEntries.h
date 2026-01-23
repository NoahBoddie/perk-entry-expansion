#pragma once

namespace PEE
{

	namespace SPCK
	{
		static inline constexpr auto perkCategory = "SpeechCheck";
		static inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kActivate;//Temporary I need to figure it out.
	}

	namespace WARES
	{
		static inline constexpr auto perkCategory = "MerchantWares";
		static inline constexpr RE::PerkEntryPoint perkEntry[2]{};//Temporary I need to figure it out.
	}


	namespace MULT
	{
		enum EP
		{
			kMultishot,
			kShotSpread,
			kShotRotation,
		};

		static inline constexpr std::string_view perkCategory[]{ "Multishot", "MultiSpread", "MultiRotation" };
		static inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kModBowZoom;
	}

}