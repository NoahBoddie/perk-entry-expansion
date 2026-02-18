#pragma once

#include "SpeechCheckHandler.h"
#include "Hooks/Hook_LoadCondition.h"
#include "Hooks/Hook_RerouteCondition.h"

#pragma once

namespace PEE::SPCK
{
	inline static void Install()
	{
		SpeechCheckHandler::Install();

		InstallHook<LoadConditionHook>();
		InstallHook<RerouteConditionHook>();
	}
}