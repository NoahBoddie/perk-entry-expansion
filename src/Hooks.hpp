#pragma once

//#include "SpeechCheck/Hooks.hpp"
#include"Multishot/Hooks.hpp"
#include"ModifyPrice/Hooks.hpp"

namespace PEE
{
	inline static void Install()
	{
		SKSE::AllocTrampoline(Allocator::GetAlloc());

		MULT::Install();
		MPRC::Install();

	}
}