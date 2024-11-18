#include "Amberpch.h"

#include "AmberEngine/Resources/RawShaders.h"

std::pair<std::string, std::string> AmberEngine::Resources::RawShader::GetGrid()
{
	std::pair<std::string, std::string> source;

	source.first = R"(
#version 430 core

layout (location = 0) in vec3 geo_Pos;
layout (location = 1) in vec2 geo_TexCoords;
layout (location = 2) in vec3 geo_Normal;

layout (std140) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
    float   ubo_Time;
};

out VS_OUT
{
    vec3 FragPos;
    vec2 TexCoords;
} vs_out;

uniform float _GridScale = 1.0;

void main()
{
    vs_out.FragPos      = vec3(ubo_Model * vec4(geo_Pos, 1.0));
    vs_out.TexCoords    = vs_out.FragPos.xz * _GridScale;

    gl_Position = ubo_Projection * ubo_View * vec4(vs_out.FragPos, 1.0);
}
)";

	source.second = R"(
#version 430 core

out vec4 FRAGMENT_COLOR;

layout (std140) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
    float   ubo_Time;
};

in VS_OUT
{
    vec3 FragPos;
    vec2 TexCoords;
} fs_in;

uniform vec3 u_Color;

uniform float GridDiv = 10.0;
uniform float GridBias = 0.5;
uniform float LineWidth = 1.0;
uniform float MajorLineWidth = 2.0;

vec4 LineColor = vec4(0.176f, 0.176f, 0.176f, 0.5);


void main()
{
  
  float gridDiv = max(round(GridDiv), 2.0);

               
                float logLength = (0.5 * log(dot(ubo_ViewPos, ubo_ViewPos)))/log(gridDiv) - GridBias;

               
                float logA = floor(logLength);
                float logB = logA + 1.0;
                float blendFactor = fract(logLength);

               
                float uvScaleA = pow(gridDiv, logA);
                float uvScaleB = pow(gridDiv, logB);

               
                vec2 UVA = fs_in.TexCoords / uvScaleA;
                vec2 UVB = fs_in.TexCoords / uvScaleB;

                
                float logC = logA + 2.0;
                float uvScaleC = pow(gridDiv, logC);
                vec2 UVC = fs_in.TexCoords / uvScaleC;

              
                vec2 gridUVA = 1.0 - abs(fract(UVA) * 2.0 - 1.0);
                vec2 gridUVB = 1.0 - abs(fract(UVB) * 2.0 - 1.0);
                vec2 gridUVC = 1.0 - abs(fract(UVC) * 2.0 - 1.0);

               
                float lineWidthA = LineWidth * (1-blendFactor);
                float lineWidthB = mix(MajorLineWidth, LineWidth, blendFactor);
                float lineWidthC = MajorLineWidth * blendFactor;

                
                float lineFadeA = clamp(lineWidthA, 0.0, 1.0);
                float lineFadeB = clamp(lineWidthB, 0.0, 1.0);
                float lineFadeC = clamp(lineWidthC, 0.0, 1.0);

               
                vec2 uvLength = max(vec2(length(vec2(dFdx(fs_in.TexCoords.x), dFdy(fs_in.TexCoords.x))), length(vec2(dFdx(fs_in.TexCoords.y), dFdy(fs_in.TexCoords.y)))), 0.00001);

                
                vec2 lineAAA = uvLength / uvScaleA;
                vec2 lineAAB = uvLength / uvScaleB;
                vec2 lineAAC = uvLength / uvScaleC;

               
                vec2 grid2A = smoothstep((lineWidthA + 1.5) * lineAAA, (lineWidthA - 1.5) * lineAAA, gridUVA);
                vec2 grid2B = smoothstep((lineWidthB + 1.5) * lineAAB, (lineWidthB - 1.5) * lineAAB, gridUVB);
                vec2 grid2C = smoothstep((lineWidthC + 1.5) * lineAAC, (lineWidthC - 1.5) * lineAAC, gridUVC);

             
                float gridA = clamp(grid2A.x + grid2A.y, 0.0, 1.0) * lineFadeA;
                float gridB = clamp(grid2B.x + grid2B.y, 0.0, 1.0) * lineFadeB;
                float gridC = clamp(grid2C.x + grid2C.y, 0.0, 1.0) * lineFadeC;

               
                float grid = clamp(gridA + max(gridB, gridC), 0.0, 1.0);

    FRAGMENT_COLOR = vec4(u_Color, grid);
}
)";

	return source;
}

std::pair<std::string, std::string> AmberEngine::Resources::RawShader::GetGizmo()
{
	std::pair<std::string, std::string> source;

	source.first = R"(
#version 430 core

layout (location = 0) in vec3 geo_Pos;
layout (location = 2) in vec3 geo_Normal;
layout (location = 1) in vec2 geo_TexCoords;

layout (std140) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
    float   ubo_Time;
};

out VS_OUT
{
    vec3 Color;
} vs_out;

uniform bool u_IsBall;
uniform bool u_IsPickable;
uniform int u_HighlightedAxis;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4
    (
        oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
        oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
        oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
        0.0,                                0.0,                                0.0,                                1.0
    );
}

void main()
{
    mat4 instanceModel = ubo_Model;

    if (gl_InstanceID == 1) 
        instanceModel *= rotationMatrix(vec3(0, 1, 0), radians(-90)); /* X axis */
    else if (gl_InstanceID == 2) 
        instanceModel *= rotationMatrix(vec3(1, 0, 0), radians(90)); /* Y axis */

    float distanceToCamera = distance(ubo_ViewPos, instanceModel[3].xyz);

	vec3 pos = geo_Pos;

    vec3 fragPos = vec3(instanceModel * vec4(pos * distanceToCamera * 0.1f, 1.0));

	if (u_IsPickable)
	{
		int blueComponent = 0;

		if (gl_InstanceID == 1)
			blueComponent = 252;

		if (gl_InstanceID == 2)
			blueComponent = 253;

		if (gl_InstanceID == 0)
			blueComponent = 254;

		vs_out.Color = vec3(1.0f, 1.0f, blueComponent / 255.0f);
	}
	else
	{
		if (u_IsBall)
		{
			vs_out.Color = vec3(1.0f);
		}
		else
		{
			float red	= float(gl_InstanceID == 1); // X
			float green = float(gl_InstanceID == 2); // Y
			float blue	= float(gl_InstanceID == 0); // Z

			if (!u_IsPickable && ((gl_InstanceID == 1 && u_HighlightedAxis == 0) || (gl_InstanceID == 2 && u_HighlightedAxis == 1) || (gl_InstanceID == 0 && u_HighlightedAxis == 2)))
			{
				vs_out.Color = vec3(1.0f, 1.0f, 0.0f);
			}	
			else
			{
				vs_out.Color = vec3(red, green, blue);
			}
		}
	}

    gl_Position = ubo_Projection * ubo_View * vec4(fragPos, 1.0);
}
)";

	source.second = R"(
#version 430 core

out vec4 FRAGMENT_COLOR;

in VS_OUT
{
    vec3 Color;
} fs_in;

uniform bool u_IsPickable;

void main()
{
	FRAGMENT_COLOR = vec4(fs_in.Color, 1.0f);
})";

	return source;
}

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
