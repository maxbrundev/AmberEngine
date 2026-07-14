#pragma once

#include <GL/glew.h>


#include "AmberRendering/Settings/DriverSettings.h"
#include "AmberRendering/Settings/EComparisonAlgorithm.h"
#include "AmberRendering/Settings/ECullFace.h"
#include "AmberRendering/Settings/EOperation.h"
#include "AmberRendering/Settings/EPixelDataFormat.h"
#include "AmberRendering/Settings/EPixelDataType.h"
#include "AmberRendering/Settings/ERasterizationMode.h"
#include "AmberRendering/Settings/ERenderingCapability.h"

namespace AmberRendering::Context
{
	class Driver
	{
	public:
		Driver(const AmberRendering::Settings::DriverSettings& p_driverSettings);
		~Driver() = default;

		void SetRasterizationLinesWidth(float p_width);
		void SetRasterizationMode(AmberRendering::Settings::ERasterizationMode p_rasterizationMode);
		void SetCapability(AmberRendering::Settings::ERenderingCapability p_capability, bool p_value);
		bool GetCapability(AmberRendering::Settings::ERenderingCapability p_capability);
		void SetStencilAlgorithm(AmberRendering::Settings::EComparisonAlgorithm p_algorithm, int32_t p_reference, uint32_t p_mask);
		void SetDepthAlgorithm(AmberRendering::Settings::EComparisonAlgorithm p_algorithm);
		void SetStencilMask(uint32_t p_mask);
		void SetStencilOperations(AmberRendering::Settings::EOperation p_stencilFail = AmberRendering::Settings::EOperation::KEEP, AmberRendering::Settings::EOperation p_depthFail = AmberRendering::Settings::EOperation::KEEP, AmberRendering::Settings::EOperation p_bothPass = AmberRendering::Settings::EOperation::KEEP);
		void SetCullFace(AmberRendering::Settings::ECullFace p_cullFace);
		void SetDepthWriting(bool p_enable);
		void SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha);
		void SetColorWriting(bool p_enable);
		void ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, AmberRendering::Settings::EPixelDataFormat format, AmberRendering::Settings::EPixelDataType type, void* data);

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
