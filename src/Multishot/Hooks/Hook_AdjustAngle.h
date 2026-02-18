#pragma once

#include "Multishot/SpreadHandler.h"

namespace PEE::MULT
{
	struct AdjustAngleHook
	{
		DECLARE_ALLOC()

		static void Install()
		{
			//SE: 235240, 1.6.640: 78A7B0
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(17693, 44108) }.address();

			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(address + RELOCATION_OFFSET(0xE82, 0xE60), thunk);
		}

		//as temp as temp can be
		static unsigned bounded_rand(unsigned range)
		{
			for (unsigned x, r;;)
				if (x = rand(), r = x % range, x - r <= -range)
					return r;
		}

		static RE::ProjectileHandle* thunk(RE::ProjectileHandle* out, RE::Projectile::LaunchData& data)
		{
			Vector3 angle{};

			if (SpreadHandler::isActive)
			{
				if (SpreadHandler::ShouldAdjust() == true)
				{
					data.autoAim = false;
					angle = SpreadHandler::ChangeAngle(data.angleX, data.angleZ);
				}
			}

			auto result = func(out, data);

			RE::ProjectileHandle;

			if (SpreadHandler::isActive)
			{
				if (SpreadHandler::ShouldAdjust() == true)
				{
					SpreadHandler::AdjustAngleAfter(data.shooter, data.weaponSource, result->get().get(), angle);
				}
				
				SpreadHandler::projIndex++;

			}

			return result;
		}

		inline static REL::Relocation<decltype(thunk)> func;
	};
}