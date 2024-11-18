#pragma once

namespace AmberEngine::Analytics::Hardware
{
	/**
	* The GPUInfo class will gather informations about the GPU usage
	*/
	class RAMInfo final
	{
	public:
		/**
		* Update the RAMInfo
		*/
		void Update();

		/**
		* Return the total physical memory reserved (MB)
		*/
		float GetUsedRAM();

		/**
		* Return the total physical memory free (MB)
		*/
		float GetFreeRAM();

		/**
		* Return the maximum available memory on the system (MB)
		*/
		float GetMaxRAM();

	private:
		float m_usedRAM = 0.0f;
		float m_freeRAM = 0.0f;
		float m_maxRAM = 0.0f;
	};
}