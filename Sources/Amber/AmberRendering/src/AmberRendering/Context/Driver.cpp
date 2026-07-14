#include "Amberpch.h"

#include "AmberRendering/Context/Driver.h"

#include "AmberDebug/Logger.h"

AmberRendering::Context::Driver::Driver(const AmberRendering::Settings::DriverSettings& p_driverSettings)
{
	InitGlew();

	isActive = true;

	if (p_driverSettings.enableDebugCallback)
	{
		GLint flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(GLDebugMessageCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
	}
	
	if (p_driverSettings.enableDepthTest)
		glEnable(GL_DEPTH_TEST);

	if (p_driverSettings.enableBackFaceCulling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	if (p_driverSettings.enableBlend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	if(p_driverSettings.enableMultisample)
		glEnable(GL_MULTISAMPLE);
}

void AmberRendering::Context::Driver::SetRasterizationLinesWidth(float p_width)
{
	glLineWidth(p_width);
}

void AmberRendering::Context::Driver::SetRasterizationMode(AmberRendering::Settings::ERasterizationMode p_rasterizationMode)
{
	glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(p_rasterizationMode));
}

void AmberRendering::Context::Driver::SetCapability(AmberRendering::Settings::ERenderingCapability p_capability, bool p_value)
{
	(p_value ? glEnable : glDisable)(static_cast<GLenum>(p_capability));
}

bool AmberRendering::Context::Driver::GetCapability(AmberRendering::Settings::ERenderingCapability p_capability)
{
	return glIsEnabled(static_cast<GLenum>(p_capability));
}

void AmberRendering::Context::Driver::SetStencilAlgorithm(AmberRendering::Settings::EComparisonAlgorithm p_algorithm, int32_t p_reference, uint32_t p_mask)
{
	glStencilFunc(static_cast<GLenum>(p_algorithm), p_reference, p_mask);
}

void AmberRendering::Context::Driver::SetDepthAlgorithm(AmberRendering::Settings::EComparisonAlgorithm p_algorithm)
{
	glDepthFunc(static_cast<GLenum>(p_algorithm));
}

void AmberRendering::Context::Driver::SetStencilMask(uint32_t p_mask)
{
	glStencilMask(p_mask);
}

void AmberRendering::Context::Driver::SetStencilOperations(AmberRendering::Settings::EOperation p_stencilFail, AmberRendering::Settings::EOperation p_depthFail, AmberRendering::Settings::EOperation p_bothPass)
{
	glStencilOp(static_cast<GLenum>(p_stencilFail), static_cast<GLenum>(p_depthFail), static_cast<GLenum>(p_bothPass));
}

void AmberRendering::Context::Driver::SetCullFace(AmberRendering::Settings::ECullFace p_cullFace)
{
	glCullFace(static_cast<GLenum>(p_cullFace));
}

void AmberRendering::Context::Driver::SetDepthWriting(bool p_enable)
{
	glDepthMask(p_enable);
}

void AmberRendering::Context::Driver::SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha)
{
	glColorMask(p_enableRed, p_enableGreen, p_enableBlue, p_enableAlpha);
}

void AmberRendering::Context::Driver::SetColorWriting(bool p_enable)
{
	glColorMask(p_enable, p_enable, p_enable, p_enable);
}

void AmberRendering::Context::Driver::ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, AmberRendering::Settings::EPixelDataFormat format, AmberRendering::Settings::EPixelDataType type, void* data)
{
	glReadPixels(x, y, width, height, static_cast<GLenum>(format), static_cast<GLenum>(type), data);
}

bool AmberRendering::Context::Driver::GetBool(GLenum p_parameter)
{
	GLboolean result;
	glGetBooleanv(p_parameter, &result);
	return static_cast<bool>(result);
}

bool AmberRendering::Context::Driver::GetBool(GLenum p_parameter, uint32_t p_index)
{
	GLboolean result;
	glGetBooleani_v(p_parameter, p_index, &result);
	return static_cast<bool>(result);
}

int AmberRendering::Context::Driver::GetInt(GLenum p_parameter)
{
	GLint result;
	glGetIntegerv(p_parameter, &result);
	return static_cast<int>(result);
}

int AmberRendering::Context::Driver::GetInt(GLenum p_parameter, uint32_t p_index)
{
	GLint result;
	glGetIntegeri_v(p_parameter, p_index, &result);
	return static_cast<int>(result);
}

float AmberRendering::Context::Driver::GetFloat(GLenum p_parameter)
{
	GLfloat result;
	glGetFloatv(p_parameter, &result);
	return static_cast<float>(result);
}

float AmberRendering::Context::Driver::GetFloat(GLenum p_parameter, uint32_t p_index)
{
	GLfloat result;
	glGetFloati_v(p_parameter, p_index, &result);
	return static_cast<float>(result);
}

double AmberRendering::Context::Driver::GetDouble(GLenum p_parameter)
{
	GLdouble result;
	glGetDoublev(p_parameter, &result);
	return static_cast<double>(result);
}

double AmberRendering::Context::Driver::GetDouble(GLenum p_parameter, uint32_t p_index)
{
	GLdouble result;
	glGetDoublei_v(p_parameter, p_index, &result);
	return static_cast<double>(result);
}

int64_t AmberRendering::Context::Driver::GetInt64(GLenum p_parameter)
{
	GLint64 result;
	glGetInteger64v(p_parameter, &result);
	return static_cast<int64_t>(result);
}

int64_t AmberRendering::Context::Driver::GetInt64(GLenum p_parameter, uint32_t p_index)
{
	GLint64 result;
	glGetInteger64i_v(p_parameter, p_index, &result);
	return static_cast<int64_t>(result);
}

std::string AmberRendering::Context::Driver::GetString(GLenum p_parameter)
{
	const GLubyte* result = glGetString(p_parameter);
	return result ? reinterpret_cast<const char*>(result) : std::string();
}

std::string AmberRendering::Context::Driver::GetString(GLenum p_parameter, uint32_t p_index)
{
	const GLubyte* result = glGetStringi(p_parameter, p_index);
	return result ? reinterpret_cast<const char*>(result) : std::string();
}

void AmberRendering::Context::Driver::InitGlew()
{
	const GLenum error = glewInit();

	if (error != GLEW_OK)
	{
		std::string message = "Error Init GLEW: ";
		std::string glewError = reinterpret_cast<const char*>(glewGetErrorString(error));

		AB_LOG_INFO(message + glewError);
	}
}

void AmberRendering::Context::Driver::GLDebugMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::string output;

	output += "OpenGL Debug Message:\n";
	output += "Debug message (" + std::to_string(id) + "): " + message + "\n";

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:				output += "Source: API";				break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		output += "Source: Window System";		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	output += "Source: Shader Compiler";	break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:		output += "Source: Third Party";		break;
	case GL_DEBUG_SOURCE_APPLICATION:		output += "Source: Application";		break;
	case GL_DEBUG_SOURCE_OTHER:				output += "Source: Other";				break;
	}

	output += "\n";

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               output += "Type: Error";				break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: output += "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  output += "Type: Undefined Behaviour";	break;
	case GL_DEBUG_TYPE_PORTABILITY:         output += "Type: Portability";			break;
	case GL_DEBUG_TYPE_PERFORMANCE:         output += "Type: Performance";			break;
	case GL_DEBUG_TYPE_MARKER:              output += "Type: Marker";				break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          output += "Type: Push Group";			break;
	case GL_DEBUG_TYPE_POP_GROUP:           output += "Type: Pop Group";			break;
	case GL_DEBUG_TYPE_OTHER:               output += "Type: Other";				break;
	}

	output += "\n";

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:			output += "Severity: High";				break;
	case GL_DEBUG_SEVERITY_MEDIUM:			output += "Severity: Medium";			break;
	case GL_DEBUG_SEVERITY_LOW:				output += "Severity: Low";				break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	output += "Severity: Notification";		break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:			AB_LOG_ERROR(output);	break;
	case GL_DEBUG_SEVERITY_MEDIUM:			AB_LOG_WARNING(output);	break;
	case GL_DEBUG_SEVERITY_LOW:				AB_LOG_INFO(output);		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	AB_LOG_INFO(output);			break;
	}
}