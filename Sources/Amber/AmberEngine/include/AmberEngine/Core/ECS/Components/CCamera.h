#pragma once

#include "AmberEngine/Core/ECS/Components/AComponent.h"

#include "AmberEngine/Rendering/Entities/Camera.h"

namespace AmberEngine::Core::ECS { class Actor; }

namespace AmberEngine::Core::ECS::Components
{
	class CCamera : public AComponent
	{
	public:

		CCamera(Actor& p_owner);
		~CCamera() = default;

		std::string GetName() override;
		void SetFov(float p_value);
		void SetSize(float p_value);
		void SetNear(float p_value);
		void SetFar(float p_value);

		void SetClearColor(const glm::vec3& p_clearColor);

		void SetProjectionMode(Rendering::Settings::EProjectionMode p_projectionMode);

		float GetFov() const;
		float GetSize() const;
		float GetNear() const;
		float GetFar() const;

		const glm::vec3& GetClearColor() const;

		Rendering::Settings::EProjectionMode GetProjectionMode() const;

		Rendering::Entities::Camera& GetCamera();

		virtual void OnInspector(UI::WidgetContainer& p_root) override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

	private:
		Rendering::Entities::Camera m_camera;
	};
}
