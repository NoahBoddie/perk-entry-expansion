#pragma once

namespace RE
{
	struct ChargeTimer
	{
		float		duration{};		//00
		uint32_t	pad04{};		//04
		uint64_t	releaseTime{};	//08
	};
	static_assert(sizeof(ChargeTimer) == 0x10);

}