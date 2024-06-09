#include "Amberpch.h"

#include "AmberEngine/Context/Driver.h"

AmberEngine::Context::Driver::Driver(const Settings::DriverSettings& p_driverSettings)
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

void AmberEngine::Context::Driver::SetRasterizationLinesWidth(float p_width)
{
	glLineWidth(p_width);
}

void AmberEngine::Context::Driver::SetRasterizationMode(Rendering::Settings::ERasterizationMode p_rasterizationMode)
{
	glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(p_rasterizationMode));
}

void AmberEngine::Context::Driver::SetCapability(Rendering::Settings::ERenderingCapability p_capability, bool p_value)
{
	(p_value ? glEnable : glDisable)(static_cast<GLenum>(p_capability));
}

bool AmberEngine::Context::Driver::GetCapability(Rendering::Settings::ERenderingCapability p_capability)
{
	return glIsEnabled(static_cast<GLenum>(p_capability));
}

void AmberEngine::Context::Driver::SetStencilAlgorithm(Rendering::Settings::EComparisonAlgorithm p_algorithm, int32_t p_reference, uint32_t p_mask)
{
	glStencilFunc(static_cast<GLenum>(p_algorithm), p_reference, p_mask);
}

void AmberEngine::Context::Driver::SetDepthAlgorithm(Rendering::Settings::EComparisonAlgorithm p_algorithm)
{
	glDepthFunc(static_cast<GLenum>(p_algorithm));
}

void AmberEngine::Context::Driver::SetStencilMask(uint32_t p_mask)
{
	glStencilMask(p_mask);
}

void AmberEngine::Context::Driver::SetStencilOperations(Rendering::Settings::EOperation p_stencilFail, Rendering::Settings::EOperation p_depthFail, Rendering::Settings::EOperation p_bothPass)
{
	glStencilOp(static_cast<GLenum>(p_stencilFail), static_cast<GLenum>(p_depthFail), static_cast<GLenum>(p_bothPass));
}

void AmberEngine::Context::Driver::SetCullFace(Rendering::Settings::ECullFace p_cullFace)
{
	glCullFace(static_cast<GLenum>(p_cullFace));
}

void AmberEngine::Context::Driver::SetDepthWriting(bool p_enable)
{
	glDepthMask(p_enable);
}

void AmberEngine::Context::Driver::SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha)
{
	glColorMask(p_enableRed, p_enableGreen, p_enableBlue, p_enableAlpha);
}

void AmberEngine::Context::Driver::SetColorWriting(bool p_enable)
{
	glColorMask(p_enable, p_enable, p_enable, p_enable);
}

void AmberEngine::Context::Driver::ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Rendering::Settings::EPixelDataFormat format, Rendering::Settings::EPixelDataType type, void* data)
{
	glReadPixels(x, y, width, height, static_cast<GLenum>(format), static_cast<GLenum>(type), data);
}

bool AmberEngine::Context::Driver::GetBool(GLenum p_parameter)
{
	GLboolean result;
	glGetBooleanv(p_parameter, &result);
	return static_cast<bool>(result);
}

bool AmberEngine::Context::Driver::GetBool(GLenum p_parameter, uint32_t p_index)
{
	GLboolean result;
	glGetBooleani_v(p_parameter, p_index, &result);
	return static_cast<bool>(result);
}

int AmberEngine::Context::Driver::GetInt(GLenum p_parameter)
{
	GLint result;
	glGetIntegerv(p_parameter, &result);
	return static_cast<int>(result);
}

int AmberEngine::Context::Driver::GetInt(GLenum p_parameter, uint32_t p_index)
{
	GLint result;
	glGetIntegeri_v(p_parameter, p_index, &result);
	return static_cast<int>(result);
}

float AmberEngine::Context::Driver::GetFloat(GLenum p_parameter)
{
	GLfloat result;
	glGetFloatv(p_parameter, &result);
	return static_cast<float>(result);
}

float AmberEngine::Context::Driver::GetFloat(GLenum p_parameter, uint32_t p_index)
{
	GLfloat result;
	glGetFloati_v(p_parameter, p_index, &result);
	return static_cast<float>(result);
}

double AmberEngine::Context::Driver::GetDouble(GLenum p_parameter)
{
	GLdouble result;
	glGetDoublev(p_parameter, &result);
	return static_cast<double>(result);
}

double AmberEngine::Context::Driver::GetDouble(GLenum p_parameter, uint32_t p_index)
{
	GLdouble result;
	glGetDoublei_v(p_parameter, p_index, &result);
	return static_cast<double>(result);
}

int64_t AmberEngine::Context::Driver::GetInt64(GLenum p_parameter)
{
	GLint64 result;
	glGetInteger64v(p_parameter, &result);
	return static_cast<int64_t>(result);
}

int64_t AmberEngine::Context::Driver::GetInt64(GLenum p_parameter, uint32_t p_index)
{
	GLint64 result;
	glGetInteger64i_v(p_parameter, p_index, &result);
	return static_cast<int64_t>(result);
}

std::string AmberEngine::Context::Driver::GetString(GLenum p_parameter)
{
	const GLubyte* result = glGetString(p_parameter);
	return result ? reinterpret_cast<const char*>(result) : std::string();
}

std::string AmberEngine::Context::Driver::GetString(GLenum p_parameter, uint32_t p_index)
{
	const GLubyte* result = glGetStringi(p_parameter, p_index);
	return result ? reinterpret_cast<const char*>(result) : std::string();
}

void AmberEngine::Context::Driver::InitGlew()
{
	const GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::string message = "Error Init GLEW: ";
		std::string glewError = reinterpret_cast<const char*>(glewGetErrorString(error));

		std::cout << message + glewError << std::endl;
	}
}

void AmberEngine::Context::Driver::GLDebugMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam)
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
	output += '\n';

	std::cout << output;
}