#pragma once

namespace PEE
{
	struct Test
	{
		inline static RE::TESLevItem* item = nullptr;
		inline static RE::TESObjectMISC* gold = nullptr;

		DECLARE_ALLOC()

		static bool TryInit()
		{
			if (!gold) {



				if (auto handler = GetDataHandler()) {
					//If I can, match the ownership of the object.
					//If it's an actor only run on non-unique npcs.
					gold = RE::TESForm::LookupByID<RE::TESObjectMISC>(0xF);
				}
			}
			return gold;
		}

		static bool thunk(RE::TESObjectREFR* a_this, bool a2)
		{
			bool result = func(a_this, a2);
			
			if (result) {
				auto changes = a_this->extraList.GetByType<RE::ExtraContainerChanges>()->changes;
				logger::info("1; name: {}, changed: {}, weight: {}", a_this->GetDisplayFullName(), changes->changed, changes->armorWeight);

			}
			if (result && TryInit()) {
				//Works but unsure how it'll respond with level lists directly. Also adding items via this way
				// doesn't set ownership
				a_this->AddObjectToContainer(gold, nullptr, 9999, nullptr);
			}

			//If we open a chest and it's changed false we can assign it then. We need to prep however 
			// for if nothing gets assigned. I wouldn't want to force a change on everyone of these.
			if (result) {
				auto changes = a_this->extraList.GetByType<RE::ExtraContainerChanges>()->changes;
				logger::info("2; name: {}, changed: {}, weight: {}", a_this->GetDisplayFullName(), changes->changed, changes->armorWeight);

			}
			return result;
		}




		static inline REL::Relocation<decltype(thunk)> func;
		static void Install()
		{
			//SE: 1D8D10, 1.6.640: 1E6270
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(15800, 16038) }.address();

			static ProloguePatch patch{ address, 5 };

			auto& trampoline = SKSE::GetTrampoline();

			auto call_or_jmp = IsCallOrJump(address);

			auto place_query = trampoline.write_branch<5>(address, (uintptr_t)thunk);

			if (call_or_jmp != OperEnum::Jump)
				func = (uintptr_t)patch.getCode();
			else
				func = place_query;

			Install2();
		}




		static void* thunk2(RE::TESObjectREFR* a_this)
		{
			//For this I can use the weights as a change value. I'll set them to another number that's still trival
			// to signify that I've visited them recently.
			// Of course this would then reset once when the game

			auto result = func2(a_this);

			if (result) {
				auto changes = a_this->extraList.GetByType<RE::ExtraContainerChanges>()->changes;
				logger::info("1; name: {}, changed: {}, weight: {}", a_this->GetDisplayFullName(), changes->changed, changes->armorWeight);

			}
			return result;
		}




		static inline REL::Relocation<decltype(thunk2)> func2;
		static void Install2()
		{
			//SE: 1D8E40, 1.6.640: x
			auto address = REL::Relocation<uintptr_t>{ REL::RelocationID(15802, 0) }.address();

			static ProloguePatch patch{ address, 5 };

			auto& trampoline = SKSE::GetTrampoline();

			auto call_or_jmp = IsCallOrJump(address);

			auto place_query = trampoline.write_branch<5>(address, (uintptr_t)thunk2);

			if (call_or_jmp != OperEnum::Jump)
				func2 = (uintptr_t)patch.getCode();
			else
				func2 = place_query;
		}

	};
}