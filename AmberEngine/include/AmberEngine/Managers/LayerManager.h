#pragma once

#include "AmberEngine/Managers/ILayerManager.h"

#include "AmberEngine/Core/ALayer.h"

namespace AmberEngine::Managers
{
	class LayerManager : public ILayerManager
	{
	public:
		LayerManager() = default;
		~LayerManager() override;
		
		bool AddLayer(Core::ALayer* p_layer) override;
		void DestroyLayers() override;
		bool DestroyLayer(Core::ALayer* p_layer) override;
		Core::ALayer* GetLayer(Core::ALayer* p_layer) override;
		void UpdateLayers(float p_deltaTime);

	private:
		std::vector<Core::ALayer*> m_layers;
	};
}
