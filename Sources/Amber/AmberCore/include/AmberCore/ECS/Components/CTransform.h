#pragma once


#include "AmberCore/ECS/Components/AComponent.h"

#include "AmberMaths/Transform.h"
#include "AmberUI/WidgetContainer.h"

namespace AmberCore::ECS
{
	class Actor;
}

namespace AmberCore::ECS::Components
{
	class CTransform : public AComponent
	{
	public:
		CTransform(Actor& p_owner, glm::vec3 p_localPosition = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 p_localRotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 p_localScale = glm::vec3(1.0f, 1.0f, 1.0f));
		virtual ~CTransform() override = default;

		void SetParent(CTransform& p_parent);
		bool RemoveParent();
		bool HasParent() const;
		void SetLocalPosition(glm::vec3 p_newPosition);
		void SetLocalRotationEuler(glm::vec3 p_newRotation);
		void SetLocalRotation(glm::quat	p_newRotation);
		void SetLocalScale(glm::vec3 p_newScale);
		void SetWorldPosition(glm::vec3 p_newPosition);
		void SetWorldRotationEuler(glm::vec3 p_newRotation);
		void SetWorldRotation(glm::quat p_newRotation);
		void SetWorldScale(glm::vec3 p_newScale);
		void TranslateLocal(const glm::vec3& p_translation);
		void RotateLocal(const glm::vec3& p_rotation);
		void ScaleLocal(const glm::vec3& p_scale);
		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetLocalRotationEuler() const;
		const glm::quat& GetLocalRotation() const;
		const glm::vec3& GetLocalScale() const;
		const glm::vec3& GetWorldPosition() const;
		const glm::vec3& GetWorldRotationEuler() const;
		const glm::quat& GetWorldRotation() const;
		const glm::vec3& GetWorldScale() const;
		const glm::mat4& GetLocalMatrix() const;
		const glm::mat4& GetWorldMatrix() const;
		
		glm::vec3 GetWorldForward() const;
		glm::vec3 GetWorldUp() const;
		glm::vec3 GetWorldRight() const;
		glm::vec3 GetLocalForward() const;
		glm::vec3 GetLocalUp() const;
		glm::vec3 GetLocalRight() const;

		AmberMaths::Transform& GetTransform();

		std::string GetName() override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;
		

	private:
		AmberMaths::Transform m_transform;
	};
}