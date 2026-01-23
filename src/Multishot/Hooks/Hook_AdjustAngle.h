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
			bool do_after = false;

			if (!do_after && SpreadHandler::isActive)
			{
				if (SpreadHandler::ShouldAdjust() == true)
				{


					if constexpr (1)
					{
						float xAngle = RadToDeg(data.angleX);
						float zAngle = RadToDeg(data.angleZ);

						data.autoAim = false;
						//data.forceConeOfFire = true;

						//SpreadHandler::AdjustAngle(data.shooter, data.weaponSource, xAngle, zAngle);
						SpreadHandler::AdjustAngle(data.shooter, data.weaponSource, data.angleX, data.angleZ);

						//data.angleX = DegToRad(xAngle);
						//data.angleZ = DegToRad(zAngle);
					}
					else
					{
						auto index = ((SpreadHandler::projIndex - SpreadHandler::IsProjCountEven()) / 2) + 1;

						//evens are right, odds are left
						float dir = SpreadHandler::IsIndexEven() ? 1.f : -1.f;

						float angle = DegToRad(SpreadHandler::NormalizeAngle(dir * index * SpreadHandler::baseSpread, 180));
						auto node = data.shooter->GetFireNode();

						data.angleZ += angle;
						RE::NiPoint3 origin{};
						data.shooter->Unk_A0(node, data.angleX, data.angleZ, data.origin);

					}
				}

				SpreadHandler::projIndex++;
			}

			auto result = func(out, data);

			if (do_after &&SpreadHandler::isActive)
			{
				if (SpreadHandler::ShouldAdjust() == true)
				{
					SpreadHandler::AdjustAngleAfter(data.shooter, data.weaponSource, result->get().get());
				}
				
				SpreadHandler::projIndex++;

			}

			return result;
		}

		inline static REL::Relocation<decltype(thunk)> func;
	};
}