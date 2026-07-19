#pragma once


#include "AmberCore/ECS/Components/AComponent.h"

#include "AmberTools/Eventing/Event.h"

#include "AmberRendering/Resources/Model.h"

namespace AmberCore::ECS { class Actor; }

namespace AmberCore::ECS::Components
{
	class CModelRenderer : public AComponent
	{
	public:
		enum class EFrustumBehaviour
		{
			DISABLED = 0,
			CULL_MODEL = 1,
			CULL_MESHES = 2,
			CULL_CUSTOM = 3
		};

		CModelRenderer(Actor& p_owner);
		virtual ~CModelRenderer() override = default;

		void SetModel(AmberRendering::Resources::Model* p_model);
		AmberRendering::Resources::Model* GetModel() const;

		void SetFrustumBehaviour(EFrustumBehaviour p_boundingMode);
		EFrustumBehaviour GetFrustumBehaviour() const;

		const AmberRendering::Geometry::BoundingSphere& GetCustomBoundingSphere() const;
		void SetCustomBoundingSphere(const AmberRendering::Geometry::BoundingSphere& p_boundingSphere);

		virtual std::string GetName() override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;

	private:
		AmberRendering::Resources::Model* m_model = nullptr;
		AmberTools::Eventing::Event<> m_modelChangedEvent;
		AmberRendering::Geometry::BoundingSphere m_customBoundingSphere = { {}, 1.0f };
		EFrustumBehaviour m_frustumBehaviour = EFrustumBehaviour::CULL_MODEL;
	};
}
