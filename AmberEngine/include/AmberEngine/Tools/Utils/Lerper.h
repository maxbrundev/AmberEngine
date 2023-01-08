#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Utils
{
	class API_AMBERENGINE Lerper
	{
	public:
		Lerper(float p_value, float p_lerpTime);
		~Lerper() = default;

		float Update(float p_deltaTime);

		void SetValue(float p_value);
		void SetTarget(float p_value);
		void Reset();
		void Reverse();
		void ReverseInfinitely();

		bool IsDone() const;

	private:
		bool m_isDone = false;

		float m_currentValue = 0.0f;
		float m_value        = 0.0f;
		float m_target       = 0.0f;
		float m_lerpTime     = 0.0f;
		float m_currentTime  = 0.0f;
		float m_initialValue = 0.0f;
	};
}