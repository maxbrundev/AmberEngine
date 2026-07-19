#pragma once

#include "AmberTools/Eventing/Event.h"

namespace AmberEditor::Settings
{
	class EditorSettings
	{
	public:
		template<typename T>
		class Property
		{
		public:
			Property(T p_value) : m_value(p_value) {}

			AmberTools::Eventing::Event<T> ValueChangedEvent;

			inline T& operator=(T p_value)
			{
				Set(p_value);
				return m_value;
			}

			inline void Set(T p_value)
			{
				m_value = p_value;
				ValueChangedEvent.Invoke(m_value);
			}

			inline operator T()
			{
				return m_value;
			}

			inline T Get() const
			{
				return m_value;
			}

		private:
			T m_value;
		};

		EditorSettings() = delete;

		inline static Property<bool> ShowGeometryBounds = { false };
		inline static Property<bool> ShowLightBounds = { false };
		inline static Property<bool> ShowGeometryFrustumCullingInSceneView = { false };
		inline static Property<bool> ShowLightFrustumCullingInSceneView = { false };
		inline static Property<float> LightBillboardScale = { 0.5f };
		inline static Property<float> TranslationSnapUnit = { 1.0f };
		inline static Property<float> RotationSnapUnit = { 15.0f };
		inline static Property<float> ScalingSnapUnit = { 1.0f };
	};
}
