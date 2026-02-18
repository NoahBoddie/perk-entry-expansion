#pragma once


namespace PEE::CONT
{
	struct ContainerHandler
	{
		inline static std::thread::id ignoreThread{};

		static void Handle(RE::TESObjectREFR* a_this)
		{
			if (std::this_thread::get_id() != ignoreThread) {
				if (auto obj = a_this->GetBaseObject(); obj && obj->formType == RE::FormType::Container) {
					
					if (auto player = GetPlayer()) RE::HandleEntryPoint(perkEntry, player, a_this, perkCategory, a_this);

				}
			}
		}

	};
}