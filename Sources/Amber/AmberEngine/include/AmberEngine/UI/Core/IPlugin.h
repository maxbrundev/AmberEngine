#pragma once
\
#include "AmberEngine/ImGUI/imgui.h"

#include "AmberEngine/Eventing/Event.h"

class IPlugin
{
public:
	/**
	* Execute the plugin behaviour
	*/
	virtual void Execute() = 0;

	/* Feel free to store any data you want here */
	void* userData = nullptr;
};

class Pluginable
{
public:
	/**
	* Destructor (Destroys every plugins)
	*/
	virtual ~Pluginable()
	{
		RemoveAllPlugins();
	}

	/**
	* Add a plugin
	* @param p_args
	*/
	template<typename T, typename... Args>
	T& AddPlugin(Args&& ... p_args)
	{
		static_assert(std::is_base_of<IPlugin, T>::value, "T should derive from IPlugin");

		T* newPlugin = new T(std::forward<Args>(p_args)...);
		m_plugins.push_back(newPlugin);
		return *newPlugin;
	}

	/**
	* Returns the plugin of the given type, or nullptr if not found
	*/
	template<typename T>
	T* GetPlugin()
	{
		static_assert(std::is_base_of<IPlugin, T>::value, "T should derive from IPlugin");

		for (auto it = m_plugins.begin(); it != m_plugins.end(); ++it)
		{
			T* result = dynamic_cast<T*>(*it);
			if (result)
				return result;
		}

		return nullptr;
	}

	/**
	* Execute every plugins
	*/
	void ExecutePlugins()
	{
		for (auto& plugin : m_plugins)
			plugin->Execute();
	}

	/**
	* Remove every plugins
	*/
	void RemoveAllPlugins()
	{
		for (auto& plugin : m_plugins)
			delete plugin;

		m_plugins.clear();
	}

private:
	std::vector<IPlugin*> m_plugins;
};

template<typename T>
class DDTarget : public IPlugin
{
public:
	/**
	* Create the drag and drop target
	* @param p_identifier
	*/
	DDTarget(const std::string& p_identifier) : identifier(p_identifier)
	{}

	/**
	* Execute the drag and drop target behaviour
	* @param p_identifier
	*/
	virtual void Execute() override
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (!m_isHovered)
				HoverStartEvent.Invoke();

			m_isHovered = true;

			ImGuiDragDropFlags target_flags = 0;
			// target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something

			if (!showYellowRect)
				target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(identifier.c_str(), target_flags))
			{
				T data = *(T*)payload->Data;
				DataReceivedEvent.Invoke(data);
			}
			ImGui::EndDragDropTarget();
		}
		else
		{
			if (m_isHovered)
				HoverEndEvent.Invoke();

			m_isHovered = false;
		}
	}

	/**
	* Returns true if the drag and drop target is hovered by a drag and drop source
	*/
	bool IsHovered() const
	{
		return m_isHovered;
	}

public:
	std::string identifier;
	AmberEngine::Eventing::Event<T> DataReceivedEvent;
	AmberEngine::Eventing::Event<> HoverStartEvent;
	AmberEngine::Eventing::Event<> HoverEndEvent;

	bool showYellowRect = true;

private:
	bool m_isHovered;
};


template<typename T>
class DDSource : public IPlugin
{
public:
	/**
	* Create the drag and drop source
	* @param p_identifier
	* @param p_tooltip
	* @param p_data
	*/
	DDSource
	(
		const std::string& p_identifier,
		const std::string& p_tooltip,
		T p_data
	) : identifier(p_identifier), tooltip(p_tooltip), data(p_data)
	{}

	/**
	* Execute the behaviour of the drag and drop source
	*/
	virtual void Execute() override
	{
		ImGuiDragDropFlags src_flags = 0;
		src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
		src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging

		if (!hasTooltip)
			src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip

		if (ImGui::BeginDragDropSource(src_flags))
		{
			if (!m_isDragged)
				DragStartEvent.Invoke();

			m_isDragged = true;

			if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
				ImGui::Text(tooltip.c_str());
			ImGui::SetDragDropPayload(identifier.c_str(), &data, sizeof(data));
			ImGui::EndDragDropSource();
		}
		else
		{
			if (m_isDragged)
				DragStopEvent.Invoke();

			m_isDragged = false;
		}
	}

	/**
	* Returns true if the drag and drop source is dragged
	*/
	bool IsDragged() const
	{
		return m_isDragged;
	}

public:
	std::string identifier;
	std::string tooltip;
	T data;
	AmberEngine::Eventing::Event<> DragStartEvent;
	AmberEngine::Eventing::Event<> DragStopEvent;

	bool hasTooltip = true;

private:
	bool m_isDragged;
};