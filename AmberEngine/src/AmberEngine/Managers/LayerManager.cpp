#include "Amberpch.h"

#include "AmberEngine/Managers/LayerManager.h"

AmberEngine::Managers::LayerManager::~LayerManager()
{
	for (auto* layer : m_layers)
	{
		delete layer;
	}
	m_layers.clear();
}

bool AmberEngine::Managers::LayerManager::AddLayer(Core::ALayer* p_layer)
{
	const auto it = std::find(m_layers.begin(), m_layers.end(), p_layer);

	if(it == m_layers.end())
	{
		m_layers.push_back(p_layer);
		p_layer->OnStart();
		return true;
	}

	return false;
}

void AmberEngine::Managers::LayerManager::DestroyLayers()
{
	for (auto* layer : m_layers)
	{
		DestroyLayer(layer);
	}
}

bool AmberEngine::Managers::LayerManager::DestroyLayer(Core::ALayer* p_layer)
{
	const auto it = std::find(m_layers.begin(), m_layers.end(), p_layer);

	if(it != m_layers.end())
	{
		p_layer->OnDestroy();
		m_layers.erase(it);
		return true;
	}

	return false;
}

AmberEngine::Core::ALayer* AmberEngine::Managers::LayerManager::GetLayer(Core::ALayer* p_layer)
{
	const auto it = std::find(m_layers.begin(), m_layers.end(), p_layer);

	 return *it;
}

void AmberEngine::Managers::LayerManager::UpdateLayers(float p_deltaTime)
{
	for (auto* layer : m_layers)
	{
		layer->OnUpdate(p_deltaTime);
	}
}
