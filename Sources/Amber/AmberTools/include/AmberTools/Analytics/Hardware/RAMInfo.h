#pragma once

namespace AmberTools::Analytics::Hardware
{
	class RAMInfo final
	{
	public:
		void Update();

		float GetUsedRAM();

		float GetFreeRAM();

		float GetMaxRAM();

	private:
		float m_usedRAM = 0.0f;
		float m_freeRAM = 0.0f;
		float m_maxRAM = 0.0f;
	};
}