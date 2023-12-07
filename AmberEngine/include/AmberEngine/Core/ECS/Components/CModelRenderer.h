#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/ECS/Components/AComponent.h"

#include "AmberEngine/Resources/Model.h"

namespace AmberEngine
{
	namespace Core
	{
		namespace ECS
		{
			class Actor;
		}
	}
}

namespace AmberEngine::Core::ECS::Components
{
	class API_AMBERENGINE CModelRenderer : public AComponent
	{
	public:
		CModelRenderer(Actor& p_owner, std::string_view p_name, const std::string& p_filePath);
		virtual ~CModelRenderer() override;

		Resources::Model* GetModel() const;
		std::vector<Resources::Mesh*>& GetMeshes() const;
		std::string GetName() override;
		void OnInspector(UI::WidgetContainer& p_root) override;

	private:
		Resources::Model* m_model;
		std::string_view m_name;
	};
}
