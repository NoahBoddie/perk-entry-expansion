#pragma once

#include "SpeechCheck/SpeechCheckHandler.h"

namespace PEE::SPCK
{


	struct RerouteConditionHook
	{
		///SkyrimSE.exe+4442B0+87 is actually where we'd want to set up the formulas.


		static void Install()
		{
			auto script = RE::SCRIPT_FUNCTION::LocateScriptCommand("GetNoRumors");

			func = script->conditionFunction;

			script->conditionFunction = thunk;
		}

		//A seperate hook will have to be established to turn off the condition function check.

		static bool thunk(RE::TESObjectREFR* a_this, void* param1, void* param2, double& result)
		{

			switch (reinterpret_cast<size_t>(param2))
			{
			case SpeechCheckHandler::SPEECH_CHECK:
				result = SpeechCheckHandler::Handle(a_this, std::bit_cast<SpeechCheckData>(param1));
				return true;

			case SpeechCheckHandler::NO_CHECK:
				result = 0;
				return true;

			default:
				return func(a_this, param1, param2, result);

			}

		}


		static inline RE::SCRIPT_FUNCTION::Condition_t* func = nullptr;
	};

}