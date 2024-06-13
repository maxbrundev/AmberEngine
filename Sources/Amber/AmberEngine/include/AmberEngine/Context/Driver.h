#pragma once

#include <GL/glew.h>


#include "AmberEngine/Settings/DriverSettings.h"
#include "AmberEngine/Rendering/Settings/EComparisonAlgorithm.h"
#include "AmberEngine/Rendering/Settings/ECullFace.h"
#include "AmberEngine/Rendering/Settings/EOperation.h"
#include "AmberEngine/Rendering/Settings/EPixelDataFormat.h"
#include "AmberEngine/Rendering/Settings/EPixelDataType.h"
#include "AmberEngine/Rendering/Settings/ERasterizationMode.h"
#include "AmberEngine/Rendering/Settings/ERenderingCapability.h"

namespace  AmberEngine::Context
{
	class Driver
	{
	public:
		Driver(const Settings::DriverSettings& p_driverSettings);
		~Driver() = default;

		void SetRasterizationLinesWidth(float p_width);
		void SetRasterizationMode(Rendering::Settings::ERasterizationMode p_rasterizationMode);
		void SetCapability(Rendering::Settings::ERenderingCapability p_capability, bool p_value);
		bool GetCapability(Rendering::Settings::ERenderingCapability p_capability);
		void SetStencilAlgorithm(Rendering::Settings::EComparisonAlgorithm p_algorithm, int32_t p_reference, uint32_t p_mask);
		void SetDepthAlgorithm(Rendering::Settings::EComparisonAlgorithm p_algorithm);
		void SetStencilMask(uint32_t p_mask);
		void SetStencilOperations(Rendering::Settings::EOperation p_stencilFail = Rendering::Settings::EOperation::KEEP, Rendering::Settings::EOperation p_depthFail = Rendering::Settings::EOperation::KEEP, Rendering::Settings::EOperation p_bothPass = Rendering::Settings::EOperation::KEEP);
		void SetCullFace(Rendering::Settings::ECullFace p_cullFace);
		void SetDepthWriting(bool p_enable);
		void SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha);
		void SetColorWriting(bool p_enable);
		void ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Rendering::Settings::EPixelDataFormat format, Rendering::Settings::EPixelDataType type, void* data);

		bool GetBool(GLenum p_parameter);
		bool GetBool(GLenum p_parameter, uint32_t p_index);
		int GetInt(GLenum p_parameter);
		int GetInt(GLenum p_parameter, uint32_t p_index);
		float GetFloat(GLenum p_parameter);
		float GetFloat(GLenum p_parameter, uint32_t p_index);
		double GetDouble(GLenum p_parameter);
		double GetDouble(GLenum p_parameter, uint32_t p_index);
		int64_t GetInt64(GLenum p_parameter);
		int64_t GetInt64(GLenum p_parameter, uint32_t p_index);
		std::string GetString(GLenum p_parameter);
		std::string GetString(GLenum p_parameter, uint32_t p_index);
		
	private:
		void InitGlew();
		static void __stdcall GLDebugMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam);

	private:
		bool isActive;
	};
}
