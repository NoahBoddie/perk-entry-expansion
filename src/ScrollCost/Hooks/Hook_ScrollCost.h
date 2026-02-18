namespace PEE::SCRC
{
	struct ScrollCostHook
	{

		DECLARE_ALLOC()

		static void Install()
		{

			//SE: 630F40, 1.6.640: 669090
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(37798, 38747) }.address();

			static ProloguePatch patch{ address, 6 };

			auto& trampoline = SKSE::GetTrampoline();


			auto call_or_jmp = IsCallOrJump(address);

			auto place_query = trampoline.write_branch<5>(address, (uintptr_t)thunk);

			if (call_or_jmp != OperEnum::Jump)
				func = (uintptr_t)patch.getCode();
			else
				func = place_query;

		}


		static void thunk(RE::TESObjectREFR* a_this, RE::ScrollItem* scroll, RE::MagicSystem::CastingSource source)
		{
			if (auto actor = a_this->As<RE::Actor>()) {
				std::srand(std::time(NULL));

				float chance = 0;



				auto result = RE::HandleEntryPoint(perkEntry, actor, chance, perkCategory, scroll);

				assert(result == PEPE::RequestResult::Success);

				//If successfully prevented, I feel like some kind of sound should play

				if (chance > 0) {
					if (chance > 99) {
						return;
					}

					std::mt19937 engine(std::time(nullptr));
					if (std::bernoulli_distribution dist(chance / 100.f); dist(engine)) {
						return;
					}
				}

			}

			return func(a_this, scroll, source);
		}


		inline static REL::Relocation<decltype(thunk)> func;
	};

}