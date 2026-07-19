#pragma once


#include "AmberRendering/Context/Driver.h"

#include "AmberRendering/Data/Frustum.h"
#include "AmberRendering/Resources/IMesh.h"
#include "AmberRendering/Settings/ECullingOptions.h"
#include "AmberRendering/Settings/EPrimitiveMode.h"

namespace AmberRendering::Resources
{
	class Model;
	class Mesh;
	class Texture;
	class Material;
}

namespace AmberRendering::Entities
{
	class Camera;
}

namespace AmberRendering::Core
{
	class Renderer
	{
	public:
		struct FrameInfo
		{
			uint64_t batchCount    = 0;
			uint64_t instanceCount = 0;
			uint64_t polyCount     = 0;
			uint64_t vertexCount   = 0;
		};

		Renderer(AmberRendering::Context::Driver& p_driver);
		~Renderer() = default;

		Renderer(const Renderer& other)            = delete;
		Renderer(Renderer&& other)                 = delete;
		Renderer& operator=(const Renderer& other) = delete;
		Renderer& operator=(Renderer&& other)      = delete;

		void SetClearColor(float p_red, float p_green, float p_blue, float p_alpha = 1.0f) const;
		void Clear(bool p_colorBuffer = true, bool p_depthBuffer = true, bool p_stencilBuffer = true) const;
		void Clear(Entities::Camera& p_camera, bool p_colorBuffer = true, bool p_depthBuffer = true, bool p_stencilBuffer = true) const;

		uint8_t FetchGLState() const;
		void ApplyStateMask(uint8_t p_mask);
		void SetState(uint8_t p_state);

		void SetViewport(uint32_t p_x, uint32_t p_y, uint32_t p_width, uint32_t p_height) const;

		void ClearFrameInfo();

		void Draw(AmberRendering::Resources::IMesh& p_mesh, Settings::EPrimitiveMode p_primitiveMode = Settings::EPrimitiveMode::TRIANGLES, uint32_t p_instances = 1);

		std::vector<AmberRendering::Resources::Mesh*> GetMeshesInFrustum
		(
			const AmberRendering::Resources::Model& p_model,
			const AmberRendering::Geometry::BoundingSphere& p_modelBoundingSphere,
			const AmberMaths::Transform& p_modelTransform,
			const AmberRendering::Data::Frustum& p_frustum,
			AmberRendering::Settings::ECullingOptions p_cullingOptions
		);

		AmberRendering::Context::Driver& GetDriver() const;
		const FrameInfo& GetFrameInfo() const;

	private:
		AmberRendering::Context::Driver& m_driver;
		FrameInfo m_frameInfo;
		uint8_t m_state;
	};
}
