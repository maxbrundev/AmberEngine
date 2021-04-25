#shader vertex
#version 460 core

layout (location = 0) in vec3 geo_Pos;
layout (location = 1) in vec2 geo_TexCoords;
layout (location = 2) in vec3 geo_Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec3        FragPos;
    vec3        Normal;
    vec2        TexCoords;

} vs_out;

void main()
{
    vs_out.FragPos = vec3(model * vec4(geo_Pos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * geo_Normal;
    vs_out.TexCoords = geo_TexCoords;

    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}

#shader fragment
#version 460 core

out vec4 FragColor;

uniform vec3 viewPos;

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
uniform float u_Shininess = 32.0f;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(u_DiffuseMap, fs_in.TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(u_DiffuseMap, fs_in.TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),u_Shininess);
    vec3 specular = light.specular * spec * texture(u_SpecularMap, fs_in.TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}