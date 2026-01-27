#pragma once

#include "RE/BarterSetting.h"


namespace PEE::VOVR
{
	struct OverrideHandler
	{
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

		static void UpdateOverride(RE::Actor* target, RE::TESBoundObject* item, bool* keywordCheck, bool* stolenCheck, RE::TESForm* owner = nullptr)
		{
			if (!keywordCheck && !stolenCheck)
				return;

			RE::PlayerCharacter* player = GetPlayer();
			RE::TESDataHandler* handler = GetDataHandler();
			auto stock = target->GetVendorFaction()->vendorData.merchantContainer;

			if (keywordCheck) {
				*keywordCheck = true;
			}

			if (stolenCheck) {
				*stolenCheck = true;
			}
		}
	};
}