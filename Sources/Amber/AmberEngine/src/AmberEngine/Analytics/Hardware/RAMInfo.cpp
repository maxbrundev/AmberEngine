/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Amberpch.h"

#include "AmberEngine/Analytics/Hardware/RAMInfo.h"

void AmberEngine::Analytics::Hardware::RAMInfo::Update()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	m_maxRAM = statex.ullTotalPhys / 1048576.0f;
	m_freeRAM = statex.ullAvailPhys / 1048576.0f;
	m_usedRAM = m_maxRAM - statex.ullAvailPhys / 1048576.0f;
}

float AmberEngine::Analytics::Hardware::RAMInfo::GetUsedRAM()
{
	return m_usedRAM;
}

float AmberEngine::Analytics::Hardware::RAMInfo::GetFreeRAM()
{
	return m_freeRAM;
}

float AmberEngine::Analytics::Hardware::RAMInfo::GetMaxRAM()
{
	return m_maxRAM;
}