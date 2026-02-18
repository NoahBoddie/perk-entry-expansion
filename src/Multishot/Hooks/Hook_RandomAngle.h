#pragma once

#include "RE/ChargeTimer.h"

namespace PEE::MULT
{
	//Defunct, only npcs uses this hook. Also inconsistent hook.
	struct RandomAngleXHook
	{
		//DECLARE_ALLOC()

		static void Install()
		{
			//SE: 235240, 1.6.640: 78A7B0
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(17693, 44108) }.address();


			struct Patch : Xbyak::CodeGenerator
			{
				explicit Patch(uintptr_t func)
				{
					sub(rdx, esi);
					//I don't know how to make it do original other than this.
					mov(rax, &_func);
					jmp(rax);
				}
			};



			auto& trampoline = SKSE::GetTrampoline();

			//func = trampoline.write_call<5>(address + RELOCATION_OFFSET(0x5BB, 0x58E), thunk);
			_func = trampoline.write_call<5>(address + RELOCATION_OFFSET(0x5BB, 0x58E), thunk);
		}

		//The rare complete toss of a function.
		static uint32_t thunk(void* a_rng, uint32_t seed)
		{
			return func(a_rng, seed);

		}


		struct AE
		{

		};

		inline static uintptr_t _func = 0;

		inline static REL::Relocation<decltype(thunk)> func;
	};

	struct RandomAngleYHook
	{
		DECLARE_ALLOC()

		static void Install()
		{
			//SE: 74B170, 1.6.640: 78A7B0
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(42928, 44108) }.address();

			auto& trampoline = SKSE::GetTrampoline();

			func = trampoline.write_call<5>(address + RELOCATION_OFFSET(0x5BB, 0x58E), thunk);
		}

		inline static float GetFullChargeTime()
		{
			auto collection = RE::GameSettingCollection::GetSingleton();
			auto fBowDrawTime = collection->GetSetting("fBowDrawTime")->GetFloat();
			auto fArrowBowMinTime = collection->GetSetting("fArrowBowMinTime")->GetFloat();

			auto thing = (fBowDrawTime - fArrowBowMinTime) * 1000.0f;


			float time = ((thing)+(((thing)-(thing)) >= 0.5f));

			return time;
			float _time = (((fBowDrawTime - fArrowBowMinTime) * 1000.0f)
				+ ((((fBowDrawTime - fArrowBowMinTime) * 1000.0f)
					- ((fBowDrawTime - fArrowBowMinTime) * 1000.0f)) >= 0.5f));

			return _time;
		}

		//The rare complete toss of a function.
		static float thunk(RE::BSTSmallArray<RE::ChargeTimer, 2>& a_this)
		{
			//return func(a_this);

			if (a_this.empty() == true) {
				return 0.f;
			}

			static float kMaxChargeTime = GetFullChargeTime();


			float fullChargeTimestamp = RE::GetDurationOfApplicationRunTime() - kMaxChargeTime;

			float duration = 0.f;
			auto size = a_this.size();
			do
			{


				RE::ChargeTimer& timer = a_this.front();

				if (timer.releaseTime < fullChargeTimestamp) {
					a_this.erase(a_this.begin());
					continue;
				}

				duration = timer.duration;

				if (size > 1) {
					a_this.erase(a_this.begin());
				}



			} while (--size > 1);


			if  constexpr (0)
			{
				while (a_this.size() > 1)
				{


					RE::ChargeTimer& timer = a_this.front();

					if (timer.releaseTime < fullChargeTimestamp) {
						a_this.erase(a_this.begin());
						continue;
					}

					return timer.duration;

				}
			}
			return duration;

		}



		inline static REL::Relocation<decltype(thunk)> func;
	};
}