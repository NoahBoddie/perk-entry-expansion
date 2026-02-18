#pragma once

#include "SpeechCheck/SpeechCheckHandler.h"

namespace PEE::SPCK
{

	struct LoadConditionHook
	{
		DECLARE_ALLOC()

		static void Install()
		{
			//SE: 444370, 1.6.640: 45F960, VR : ???
			REL::Relocation<uintptr_t> hook{ REL::RelocationID { 29065, 29877 } , 0x1B };

			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(hook.address(), thunk);
		}

		static void thunk(RE::TESConditionItem* a_this, RE::TESForm* form)
		{
			constexpr auto replacement = RE::FunctionID::kGetNoRumors;

			func(a_this, form);

			SpeechCheckHandler::TryAlter(a_this);
		}

		inline static REL::Relocation<decltype(thunk)> func;
	};


}