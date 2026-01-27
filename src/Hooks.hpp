#pragma once

//#include "SpeechCheck/Hooks.hpp"
#include"Multishot/Hooks.hpp"
#include"ModifyPrice/Hooks.hpp"
#include"VendorOverride/Hooks.hpp"
#include"ScrollCost/Hooks.hpp"


namespace PEE
{
	inline static void Install()
	{
		SKSE::AllocTrampoline(Allocator::GetAlloc());

		MULT::Install();
		MPRC::Install();
		VOVR::Install();
		SCRC::Install();
	}
}