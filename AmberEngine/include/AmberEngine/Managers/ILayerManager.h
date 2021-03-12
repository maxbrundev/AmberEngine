#pragma once

namespace AmberEngine::Core { class ALayer; }

namespace AmberEngine::Managers
{
	class ILayerManager
	{
	public:
		virtual ~ILayerManager() = default;

		virtual bool AddLayer(Core::ALayer* p_layer)          = 0;
		virtual void DestroyLayers()                          = 0;
		virtual bool DestroyLayer(Core::ALayer* p_layer)      = 0;
		virtual Core::ALayer* GetLayer(Core::ALayer* p_layer) = 0;
	};
}
