#pragma once 
#include "PerkEntries.h"

#include "CritApplySpell/CritWeaponHandler.h"

/*
SE ID: 42844 SE Offset: 0x3a3 (Heuristic)
AE ID: 44016 AE Offset: 0x40b
*/

namespace PEE::CACS{
	struct Crit__ApplyCombatSpell {
		DECLARE_ALLOC()

		static void Patch()
		{
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> _magicApplyHook{ REL::RelocationID(42844, 44016), REL::VariantOffset(0x3a3, 0x40b, 0x3a3) }; //VR ID and offset completely unknown
			_originalCall = trampoline.write_call<5>(_magicApplyHook.address(), &ApplyCritDMGEntry);
		}

		static void Install()
		{
			return Patch();
		}

		static void  ApplyCritDMGEntry(RE::BGSPerkEntry::EntryPoint ep, RE::Actor* attacker, RE::TESObjectWEAP* weapon, RE::Actor* target, float& damage) 
		{
			_originalCall(ep, attacker, weapon, target, damage);

			std::vector<RE::SpellItem*> sp_vec;

			if (CritWeaponHandler::currentWeapon && std::this_thread::get_id() == CritWeaponHandler::activeThread)
			{
				RE::HandleEntryPoint(perkEntry, attacker, sp_vec, perkCategory, weapon, target);

				if (sp_vec.empty())
				{
					logger::debug("Spell Vector for critical is empty");
				}
				else {
					for (auto spell : sp_vec) {
						if (spell) {
							logger::debug("applySpell is: {}", spell->GetName());
							if (spell->IsPermanent()) {
								target->AddSpell(spell);
							}
							else {
								attacker->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
							}
						}
					}
				}
			}
		};
		static inline REL::Relocation<decltype(&ApplyCritDMGEntry)> _originalCall;
			
		
	};
}