#shader vertex
#version 460 core

layout (location = 0) in vec3 geo_Pos;
layout (location = 1) in vec2 geo_TexCoords;
layout (location = 2) in vec3 geo_Normal;

/* Global information sent by the engine */
layout (std140) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};

out VS_OUT
{
    vec3        FragPos;
    vec3        Normal;
    vec2        TexCoords;

} vs_out;

void main()
{
    vs_out.FragPos = vec3(ubo_Model * vec4(geo_Pos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(ubo_Model))) * geo_Normal;
    vs_out.TexCoords = geo_TexCoords;

    gl_Position = ubo_Projection * ubo_View * vec4(vs_out.FragPos, 1.0);
}

#shader fragment
#version 460 core

out vec4 FragColor;

/* Global information sent by the engine */
layout (std140) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};

in VS_OUT
{
    vec3        FragPos;
    vec3        Normal;
    vec2        TexCoords;
} fs_in;

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

uniform sampler2D u_DiffuseMap;
uniform sampler2D u_SpecularMap;
uniform float u_Shininess = 100.0;

uniform vec4 u_Diffuse = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	vec4 textureColor = texture(u_DiffuseMap, fs_in.TexCoords) * u_Diffuse;

    // ambient
    vec3 ambient = light.ambient * textureColor.rgb;

    // diffuse
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
	
    vec3 diffuse = light.diffuse * diff * textureColor.rgb;

    // specular
    vec3 viewDir = normalize(ubo_ViewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),u_Shininess);
    vec3 specular = light.specular * spec * texture(u_SpecularMap, fs_in.TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}