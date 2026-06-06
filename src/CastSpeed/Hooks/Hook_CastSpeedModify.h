#pragma once

namespace PEE::CSPM {
	struct CastSpeedModify {
		//DECLARE_ALLOC()

		static void Patch()
		{
			static REL::Relocation<std::uintptr_t> vtable{ RE::ActorMagicCaster::VTABLE[0] };
			_originalCall = vtable.write_vfunc(0x1D, &CasterUpdate);
		}
		static void Install()
		{
			return Patch();
		}

		static float GetEffectiveCastSpeed(RE::ActorMagicCaster* a_AMC, RE::Actor* a, float perkfactor)
		{
			float workingperkfactor = perkfactor;
			auto source = a_AMC->GetCastingSource();
			//RE::HandleEntryPoint(perkEntry, a_this, sp_vec, perkCategory, PEPE::Item{ alch, extra_list }, a_this);
			RE::HandleEntryPoint(perkEntry, a_AMC->GetCasterAsActor(), workingperkfactor, perkCategory, a_AMC->currentSpell);
			return workingperkfactor;
		}

		static void CasterUpdate(RE::ActorMagicCaster* a_caster, float a_delta)
		{
			float updatedtime = a_delta;
			auto a = a_caster->GetCasterAsActor();
			auto source = a_caster->GetCastingSource();
			float castperkfactor = 1.00f;
			float concentrationperkfactor = 1.00f;
			int breakloop = 0;

			if (source == RE::MagicSystem::CastingSource::kRightHand ||
				source == RE::MagicSystem::CastingSource::kLeftHand) {
				if (a_caster->state.get() == RE::MagicCaster::State::kUnk02) {
					castperkfactor = PEE::CSPM::CastSpeedModify::GetEffectiveCastSpeed(a_caster, a, castperkfactor);
					// perkfactor = std::min((a_AMC->currentSpell->GetChargeTime()) / a_deltatime, perkfactor);
					castperkfactor = max(0.05f, castperkfactor);
					updatedtime = a_delta * castperkfactor;
				}
			}
			_originalCall(a_caster, updatedtime);
		};
		static inline REL::Relocation<decltype(&CasterUpdate)> _originalCall;
	};
}
