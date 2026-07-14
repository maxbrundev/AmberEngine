#include "Amberpch.h"

#include "AmberTools/Analytics/Hardware/RAMInfo.h"

void AmberTools::Analytics::Hardware::RAMInfo::Update()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	m_maxRAM = statex.ullTotalPhys / 1048576.0f;
	m_freeRAM = statex.ullAvailPhys / 1048576.0f;
	m_usedRAM = m_maxRAM - statex.ullAvailPhys / 1048576.0f;
}

float AmberTools::Analytics::Hardware::RAMInfo::GetUsedRAM()
{
	return m_usedRAM;
}

float AmberTools::Analytics::Hardware::RAMInfo::GetFreeRAM()
{
	return m_freeRAM;
}

float AmberTools::Analytics::Hardware::RAMInfo::GetMaxRAM()
{
	return m_maxRAM;
}