#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"

#include "AmberEngine/Resources/IMesh.h"
#include "AmberEngine/Rendering/Settings/EPrimitiveMode.h"

namespace AmberEngine::Resources
{
	class Model;
	class Mesh;
	class Texture;
	class Material;
}

namespace AmberEngine::Rendering::Entities
{
	class Camera;
}

namespace AmberEngine::Core::SceneSystem
{
	class Scene;
}

namespace AmberEngine::Core
{
	class API_AMBERENGINE Renderer
	{
	public:
		struct FrameInfo
		{
			uint64_t batchCount    = 0;
			uint64_t instanceCount = 0;
			uint64_t polyCount     = 0;
			uint64_t vertexCount   = 0;
		};

		Renderer(Context::Driver& p_driver);
		~Renderer() = default;

		Renderer(const Renderer& other)            = delete;
		Renderer(Renderer&& other)                 = delete;
		Renderer& operator=(const Renderer& other) = delete;
		Renderer& operator=(Renderer&& other)      = delete;

		void SetClearColor(float p_red, float p_green, float p_blue, float p_alpha = 1.0f) const;
		void Clear(bool p_colorBuffer = true, bool p_depthBuffer = true, bool p_stencilBuffer = true) const;
		void Clear(Rendering::Entities::Camera& p_camera, bool p_colorBuffer = true, bool p_depthBuffer = true, bool p_stencilBuffer = true) const;

		uint8_t FetchGLState() const;
		void ApplyStateMask(uint8_t p_mask);
		void SetState(uint8_t p_state);

		void ClearFrameInfo();
		const FrameInfo& GetFrameInfo() const;

		void Draw(Resources::IMesh& p_mesh, Rendering::Settings::EPrimitiveMode p_primitiveMode = Rendering::Settings::EPrimitiveMode::TRIANGLES, uint32_t p_instances = 1);

	private:
		Context::Driver& m_driver;
		FrameInfo m_frameInfo;
		uint8_t m_state;
	};
}
