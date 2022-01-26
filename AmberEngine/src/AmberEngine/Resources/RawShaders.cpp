#include "Amberpch.h"

#include "AmberEngine/Resources/RawShaders.h"

std::pair<std::string, std::string> AmberEngine::Resources::RawShader::GetNormalVisualizer()
{
	std::pair<std::string, std::string> source;

	source.first = R"(
    #version 430 core

    layout (location = 0) in vec3 geo_Pos;
    layout (location = 1) in vec2 geo_TexCoords;
    layout (location = 2) in vec3 geo_Normal;

    layout (std140) uniform EngineUBO
    {
        mat4 ubo_Model;
        mat4 ubo_View;
        mat4 ubo_Projection;
        vec3 ubo_ViewPos;
    };

    out VS_OUT
    {
       vec3 FragPos;
       vec3 Normal;
    } vs_out;
    
    void main()
    {
         vs_out.FragPos = vec3(ubo_Model * vec4(geo_Pos, 1.0));
         vs_out.Normal  = mat3(transpose(inverse(ubo_Model))) * geo_Normal;

         gl_Position = ubo_Projection * ubo_View * vec4(vs_out.FragPos, 1.0);
    }
    )";

	source.second = R"(
    #version 430 core
    
    /* Global information sent by the engine */
    layout (std140) uniform EngineUBO
    {
        mat4    ubo_Model;
        mat4    ubo_View;
        mat4    ubo_Projection;
        vec3    ubo_ViewPos;
    };

    /* Information passed from the fragment shader */
    in VS_OUT
    {
        vec3        FragPos;
        vec3        Normal;
    } fs_in;

    vec3 g_Normal;

    out vec4 FragColor;

    void main()
    {
         vec3 g_Normal = normalize(fs_in.Normal);
         vec3 normalColor = g_Normal * 0.5 + 0.5;

         FragColor = vec4(normalColor, 1.0f);
    }
    )";

	return source;
}
