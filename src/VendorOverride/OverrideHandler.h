#pragma once

#include "RE/BarterSetting.h"


namespace PEE::VOVR
{
	struct OverrideHandler
	{
		//Port this to PEPE and other related stuff that could make use of it.
		inline RE::TESObjectREFR* ObtainOwnerTarget(RE::TESForm* form)
		{

			switch (*form->formType)
			{
			case RE::FormType::Faction:
			{
				static RE::TESNPC* npc = RE::IFormFactory::GetConcreteFormFactoryByType<RE::TESNPC>()->Create();
				static RE::Actor* actor = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Actor>()->Create();
				if (static uint64_t once = 0; once++)
				{
					actor->SetObjectReference(npc);
				}
				RE::TESFaction* faction = form->As<RE::TESFaction>();

				RE::FACTION_RANK data{};
				data.faction = faction;
				data.rank = 0;

				npc->factions.clear();
				npc->factions.push_back(data);
				form = actor;
			}

				[[fallthrough]];
			case RE::FormType::ActorCharacter:
				return form->As<RE::Actor>();
			}

			return nullptr;
		}

		static void UpdateOverride(RE::Actor* merchant, RE::InventoryEntryData* item, bool* keywordCheck, bool* stolenCheck, RE::TESForm* owner = nullptr)
		{
			if (!keywordCheck && !stolenCheck)
				return;

			RE::PlayerCharacter* player = GetPlayer();
			RE::TESDataHandler* handler = GetDataHandler();
			auto stock = merchant->GetVendorFaction()->vendorData.merchantContainer;

			//this can't handle what happens if it's a faction that's the owner, so I'm just not gonna do it.
			owner = nullptr;
			if (!owner) {
				owner = player;
			}

			if (keywordCheck && !*keywordCheck) {
				float should = 0.f;

				auto result = RE::HandleEntryPoint(perkEntry, player, should, perkCategory[kBarterUnrelatedItem], std::pair{merchant, stock }, std::pair{ item, owner });
				assert(result == PEPE::RequestResult::Success);
				*keywordCheck = should > 0;
			}

			if (stolenCheck && !*stolenCheck) {
				float should = 0.f;

				auto result = RE::HandleEntryPoint(perkEntry, player, should, perkCategory[kBarterStolenItem], std::pair{ merchant, stock }, std::pair{ item, owner });
				assert(result == PEPE::RequestResult::Success);
				*stolenCheck = should > 0;
			}
		}
	};
}