#pragma once

namespace RE
{
	//I'm unsure if this is the actual size of the object, these are juse the confirmed slots.
	struct BarterSetting
	{
		enum Enum : uint8_t
		{
			kPlayer,
			kMerchant,
		};

		RE::Actor*	merchant;		//00
		Enum		target;			//08
		bool		ignoreStolen;	//09
		bool		ignoreKeyword;	//0A
		uint8_t		pad0B;			//0B
		uint32_t	pad0C;			//0C
	};
	static_assert(sizeof(BarterSetting) == 0x10);
}