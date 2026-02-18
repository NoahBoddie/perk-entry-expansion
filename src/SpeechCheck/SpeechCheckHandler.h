#pragma once

#include "RE/StringSetting.h"
#include "Utility.h"

namespace PEE::SPCK
{
	//Move this shit

	//I have a macro for this but fuck it. 
	namespace detail
	{
		struct Speech
		{
			enum Enum
			{
				VeryEasy,
				Easy,
				Average,
				Hard,
				VeryHard,
				Total,
			};
		};
	}
	using Speech = detail::Speech::Enum;


	struct SpeechCheckData
	{
		float level;
		int8_t rank;
	};


	struct SpeechCheckHandler
	{
		inline static RE::StringSetting absFailText{ "sSpeechCheck_ForceFail", "Convincing them might not be possible." };
		inline static RE::StringSetting absSuccessText{ "sSpeechCheck_ForceSucceed", "Something something guaranteed success." };
		inline static std::string_view k_difficultyID = "SPEECH_DIFFICULTY";

		static constexpr uint64_t SPEECH_CHECK = static_cast<uint64_t>('SPCH') << 31 | 'CHCK';
		static constexpr uint64_t NO_CHECK = static_cast<uint64_t>('NULL') << 31 | 'CHCK';


		struct Forms
		{
			Forms(const Forms&) = delete;
			Forms(Forms&&) = delete;

			Forms()
			{
				difficultyList[Speech::VeryEasy] = RE::TESForm::LookupByID<RE::TESGlobal>(0xD16A3);
				difficultyList[Speech::Easy] = RE::TESForm::LookupByID<RE::TESGlobal>(0xD16A4);
				difficultyList[Speech::Average] = RE::TESForm::LookupByID<RE::TESGlobal>(0xD16A5);
				difficultyList[Speech::Hard] = RE::TESForm::LookupByID<RE::TESGlobal>(0xD1953);
				difficultyList[Speech::VeryHard] = RE::TESForm::LookupByID<RE::TESGlobal>(0xD1954);
				//Will need the data manager
				difficultyRank = RE::TESForm::LookupByEditorID<RE::TESGlobal>("_SpeechCheck_DifficultyRank");
				difficultyLevel = RE::TESForm::LookupByEditorID<RE::TESGlobal>("_SpeechCheck_DifficultyLevel");

				//Checks for articulation
				articulationList = RE::TESForm::LookupByID<RE::BGSListForm>(0xF759C);

				auto script = RE::SCRIPT_FUNCTION::LocateScriptCommand("GetEquipped");

				getEquipped = script->conditionFunction;
			}

			RE::SCRIPT_FUNCTION::Condition_t* getEquipped = nullptr;
			RE::TESGlobal* difficultyList[Speech::Total]{};

			RE::TESGlobal* difficultyRank = nullptr;
			RE::TESGlobal* difficultyLevel = nullptr;

			RE::BGSListForm* articulationList = nullptr;
			

			bool GetArticulationEquipped()
			{
				double result = 0;
				auto success = getEquipped(GetPlayer(), articulationList, nullptr, result);
				assert(success);
				return result > 0;
			}

		};

		static void Install()
		{
			auto col = RE::GameSettingCollection::GetSingleton();

			col->InsertSetting(absSuccessText);
			col->InsertSetting(absFailText);
		}


		inline static std::recursive_mutex lock;
		inline static std::thread::id processingThread;

		static Forms* GetForms()
		{
			static Forms forms;

			return &forms;
		}
		
		static void ResetCurrentRankAndLevel(const std::pair<float, float>& ret)
		{
			auto forms = GetForms();

			if (forms->difficultyLevel)
				forms->difficultyLevel->value = ret.first;

			if (forms->difficultyRank)
				forms->difficultyRank->value = ret.second;
		}


		static std::pair<float, float> SetCurrentRankAndLevel(const SpeechCheckData& data)
		{
			std::pair<float, float> result{};

			auto forms = GetForms();

			if (forms->difficultyRank) {
				result.first = forms->difficultyRank->value = data.rank;
			}

			if (forms->difficultyLevel) {
				result.second = forms->difficultyLevel->value = data.level;
			}

			return result;
		}



		static int GetDifficulty(RE::TESGlobal* global)
		{
			if (!global)
				return false;

			auto forms = GetForms();

			for (int i = 0; i < Speech::Total; i++)
			{
				if (global == forms->difficultyList[i])
					return i + 1;
			}

			//auto end = std::end(difficultyList);

			//auto result = std::find(std::begin(difficultyList), end, global);

			std::string_view id = global->GetFormEditorID();

			if (id.size() < k_difficultyID.size())
				return 0;

			
			if (id.starts_with(k_difficultyID) == false)
				return 0;

			return -1;
		}



		static std::optional<bool> ShouldForceResult(RE::TESObjectREFR* target, RE::Actor* act_tar)
		{
			std::optional<bool> result = std::nullopt;

			float value = 0;

			RE::PlayerCharacter* player = GetPlayer();

			//values below 0 count as zero.

			//For these 2, I may want to do them both, so I can send a notification.

			//The force fail entry points. Higher priority than force success.
			RE::HandleEntryPoint(perkEntry, player, value, perkCategory[kOverrideSpeechCheck], target);

			logger::debug("Forced result(player): {}", value != 0);


			if (value != 0) {
				//result = false;
				return value > 0;
			}

			if (act_tar) {
				RE::HandleEntryPoint(perkEntry, act_tar, value, perkCategory[kOverrideSpeechCheck], player);//2 + out

				logger::debug("Forced success(target): {}", value != 0);

				if (value != 0) {
					//result = false;
					return value > 0;
				}
			}


			return std::nullopt;
		}




		static bool Handle(RE::TESObjectREFR* target, const SpeechCheckData& data)
		{

			bool result = false;

			//Articulation trumps all
			if (GetForms()->GetArticulationEquipped() == true) {
				return true;
			}
			
			std::lock_guard guard{ lock };
			
			auto origin = SetCurrentRankAndLevel(data);

			auto actor = target->As<RE::Actor>();
			
			if (auto force = ShouldForceResult(target, actor); force != std::nullopt) {
				result = force.value();

				if (result) {
					RE::DebugNotification(absSuccessText);
				}
				else {
					RE::DebugNotification(absFailText);
				}
			}
			else {
				auto player = GetPlayer();

				float skill = player->AsActorValueOwner()->GetActorValue(RE::ActorValue::kSpeech);
				
				float req = data.level;

				RE::HandleEntryPoint(perkEntry, player, req, perkCategory[kSpeechCheck], target);
				if (actor) RE::HandleEntryPoint(perkEntry, actor, req, perkCategory[kSpeechCheck], player);

				RE::HandleEntryPoint(perkEntry, player, req, perkCategory[kSpeechSkill], target);
				if (actor) RE::HandleEntryPoint(perkEntry, actor, req, perkCategory[kSpeechSkill], player);

				result = skill >= req;
			}

			ResetCurrentRankAndLevel(origin);


			return result;
		}
		
		
		static void TryAlter(RE::TESConditionItem* item)
		{
			using GlobalOrFloat = RE::CONDITION_ITEM_DATA::GlobalOrFloat;

			using ItemData = RE::CONDITION_ITEM_DATA;

			
			auto& data = item->data;

			RE::CONDITIONITEMOBJECT target_type = *data.object;

			bool player_specific = false;

			switch (target_type)
			{
				//If used on the player ref specifically
			case RE::CONDITIONITEMOBJECT::kRef:
				if (auto ref = data.runOnRef.get(); !ref || ref->IsPlayerRef() == false)
					return;
				player_specific = true;


				[[fallthrough]];
				//If used on target
			case RE::CONDITIONITEMOBJECT::kTarget:
				break;

			default:
				return;
			}

			auto& func_data = data.functionData;

			switch (*func_data.function)
			{
			case RE::FUNCTION_DATA::FunctionID::kGetActorValue:
				if constexpr (1)
				{

					
					if (VoidCaster<RE::ActorValue>(func_data.params[0]) != RE::ActorValue::kSpeech) {
						return;
					}

					if (data.flags.opCode != ItemData::OpCode::kGreaterThanOrEqualTo) {

					}
					if (data.flags.swapTarget || !data.flags.global) {
						return;
					}
					RE::TESGlobal* global = data.comparisonValue.g;

					auto difficulty = GetDifficulty(global);
					if (!difficulty) {
						return;
					}

					SpeechCheckData check;
					check.level = global->value;
					check.rank = difficulty;
					func_data.function = RE::FUNCTION_DATA::FunctionID::kGetNoRumors;
					func_data.params[0] = std::bit_cast<void*>(check);
					func_data.params[1] = reinterpret_cast<void*>(SPEECH_CHECK);
					//Need to switch since this is being handled on the player, we need it on the dialogue target
					data.object = RE::CONDITIONITEMOBJECT::kSelf;
					data.flags.global = false;
					data.flags.opCode = ItemData::OpCode::kGreaterThan;
					data.comparisonValue.f = 0;

				}
				break;

			case RE::FUNCTION_DATA::FunctionID::kGetEquipped:
				if (player_specific)
				{
					auto forms = GetForms();
					{
						auto& func_data = data.functionData;

						RE::TESForm* comp_form = VoidCaster<RE::TESForm*>{ func_data.params[0] };

						if (!comp_form || comp_form != forms->articulationList) {
							return;
						}

						if (data.flags.opCode != ItemData::OpCode::kEqualTo)
							return;

						if (data.flags.swapTarget || data.flags.global)
							return;

						func_data.function = RE::FUNCTION_DATA::FunctionID::kGetNoRumors;
						func_data.params[0] = nullptr;
						func_data.params[1] = reinterpret_cast<void*>(NO_CHECK);
						data.flags.global = false;
						data.flags.opCode = ItemData::OpCode::kNotEqualTo;
						data.comparisonValue.f = 0;
					}
				}
				break;
			}
		}

	};

}