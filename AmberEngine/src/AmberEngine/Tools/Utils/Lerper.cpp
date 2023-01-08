#include "Amberpch.h"

#include "AmberEngine/Tools/Utils/Lerper.h"

#include "AmberEngine/Tools/Utils/Maths.h"

AmberEngine::Utils::Lerper::Lerper(float p_value, float p_lerpTime) : m_currentValue(p_value), m_value(p_value), m_lerpTime(p_lerpTime)
{
}

float AmberEngine::Utils::Lerper::Update(float p_deltaTime)
{
	if (m_currentTime < m_lerpTime)
	{
		m_currentValue = Maths::Lerp(m_value, m_target, m_currentTime / m_lerpTime);

		m_currentTime += 1.0f * p_deltaTime;
	}
	else
	{
		m_currentValue = m_target;
		m_isDone = true;
	}

	return m_currentValue;
}

void AmberEngine::Utils::Lerper::SetValue(float p_value)
{
	m_value = p_value;
}

void AmberEngine::Utils::Lerper::SetTarget(float p_value)
{
	m_target = p_value;
}

void AmberEngine::Utils::Lerper::Reset()
{
	m_currentTime = 0.0f;

	m_isDone = false;
}

void AmberEngine::Utils::Lerper::Reverse()
{
	m_currentTime = 0.0f;

	m_target = m_initialValue;
	m_value  = m_currentValue;

	m_isDone = false;
}

void AmberEngine::Utils::Lerper::ReverseInfinitely()
{
	m_currentTime = 0.0f;

	m_target = m_value;
	m_value  = m_currentValue;

	m_isDone = false;
}

bool AmberEngine::Utils::Lerper::IsDone() const
{
	return m_isDone;
}
