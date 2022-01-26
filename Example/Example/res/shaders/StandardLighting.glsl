#shader vertex
#version 430 core

layout (location = 0) in vec3 geo_Pos;
layout (location = 1) in vec2 geo_TexCoords;
layout (location = 2) in vec3 geo_Normal;
layout (location = 3) in vec3 geo_Tangent;
layout (location = 4) in vec3 geo_Bitangent;

/* Global information sent by the engine */
layout (std140) uniform EngineUBO
{
    mat4    ubo_Model;
    mat4    ubo_View;
    mat4    ubo_Projection;
    vec3    ubo_ViewPos;
};

/* Information passed to the fragment shader */
out VS_OUT
{
    vec3        FragPos;
    vec3        Normal;
    vec2        TexCoords;
    vec3        TangentFragPos;
    flat vec3   TangentViewPos;
    mat3        TBN;
} vs_out;

void main()
{
  vs_out.TBN = mat3
  (
      normalize(vec3(ubo_Model * vec4(geo_Tangent,   0.0))),
      normalize(vec3(ubo_Model * vec4(geo_Bitangent, 0.0))),
      normalize(vec3(ubo_Model * vec4(geo_Normal,    0.0)))
  );

    mat3 TBNi = transpose(vs_out.TBN);

    vs_out.FragPos = vec3(ubo_Model * vec4(geo_Pos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(ubo_Model))) * geo_Normal;
    vs_out.TexCoords = geo_TexCoords;

    vs_out.TangentFragPos   = TBNi * vs_out.FragPos;
    vs_out.TangentViewPos   = TBNi * ubo_ViewPos;

    gl_Position = ubo_Projection * ubo_View * vec4(vs_out.FragPos, 1.0);
}

#shader fragment
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
    vec2        TexCoords;
    vec3        TangentFragPos;
    flat vec3   TangentViewPos;
    mat3        TBN;
} fs_in;

struct Light {
    vec3 direction;
    vec3 color;
    float intensity;
};

/* Uniforms */
uniform sampler2D u_DiffuseMap;
uniform sampler2D u_SpecularMap;
uniform vec4 u_Diffuse    = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 u_Specular   = vec4(1.0, 1.0, 1.0, 1.0);
uniform float u_Shininess = 100.0;
uniform int u_DebugNormal = 0;

uniform Light light;

/* Constant variables */
const vec3 c_lightPosition = vec3(-1000.0, 1000.0, 1000.0);
const vec3 c_lightDiffuse  = vec3(1.0, 1.0, 1.0);
const vec3 c_lightAmbient  = vec3(0.3, 0.3, 0.3);

/* Global variables */
vec3 g_Normal;
vec2 g_TexCoords;
vec3 g_ViewDir;
vec4 g_DiffuseTexel;
vec4 g_SpecularTexel;

out vec4 FragColor;

vec3 BlinnPhong(vec3 p_lightDir, vec3 p_lightColor, float p_luminosity)
{
    const vec3 lightDir             = normalize(-p_lightDir);
    const vec3 halfwayDir           = normalize(-p_lightDir + g_ViewDir);
    const float diffuseCoefficient  = max(dot(lightDir, g_Normal), 0.0);
    const float specularCoefficient = pow(max(dot(halfwayDir, g_Normal), 0.0), u_Shininess * 2.0);

    vec3 diffuse  = g_DiffuseTexel.rgb * diffuseCoefficient * p_lightColor * p_luminosity;
    vec3 specular = g_SpecularTexel.rgb * specularCoefficient * p_lightColor * p_luminosity;

    return diffuse + specular;
}

vec3 Phong(vec3 p_lightDir, vec3 p_lightColor, float p_luminosity)
{
    const vec3 lightDir             = normalize(-p_lightDir);
    const vec3 reflectDir           = reflect(-lightDir, g_Normal);
    const float diffuseCoefficient  = max(dot(lightDir, g_Normal), 0.0);
    const float specularCoefficient = pow(max(dot(reflectDir, g_ViewDir), 0.0), u_Shininess);

    vec3 diffuse  = g_DiffuseTexel.rgb * diffuseCoefficient * p_lightColor * p_luminosity;
    vec3 specular = g_SpecularTexel.rgb * specularCoefficient * p_lightColor * p_luminosity;

    return diffuse + specular;
}

vec3 Lambert(vec3 p_position, vec3 p_diffuseColor, vec3 p_ambientColor)
{
    const vec3 lightDir            = normalize(p_position - fs_in.FragPos);
    const float diffuseCoefficient = max(dot(lightDir, g_Normal), 0.0);

    vec3 diffuse  = g_DiffuseTexel.rgb * diffuseCoefficient * p_diffuseColor;
    diffuse += vec3(g_DiffuseTexel.rgb * p_ambientColor);
    return diffuse;
}

vec3 CalculateDirectionalLight(Light p_light)
{
    return BlinnPhong(p_light.direction, p_light.color, p_light.intensity);
}

void main()
{
  g_Normal        = normalize(fs_in.Normal);
  g_TexCoords     = vec2(fs_in.TexCoords);
  g_ViewDir       = normalize(ubo_ViewPos - fs_in.FragPos);
  g_DiffuseTexel  = texture(u_DiffuseMap,  g_TexCoords) * u_Diffuse;
  g_SpecularTexel = texture(u_SpecularMap, g_TexCoords) * u_Specular;

  vec3 lightSum = vec3(0.0);

  if(u_DebugNormal == 0)
  {
    lightSum += CalculateDirectionalLight(light);
    //lightSum += Lambert(c_lightPosition, c_lightDiffuse, c_lightAmbient);

    //Quick Ambiant
    lightSum += vec3(g_DiffuseTexel.rgb * light.color * c_lightAmbient);
  }
  else
  {
    lightSum = g_Normal * 0.5 + 0.5;
  }

  FragColor = vec4(lightSum, 1.0f);
}
