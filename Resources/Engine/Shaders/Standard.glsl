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
    float   ubo_Time;
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
    float   ubo_Time;
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

/* Lights information sent by the engine */
layout(std430, binding = 0) buffer LightSSBO
{
    mat4 ssbo_Lights[];
};

/* Uniforms */
uniform vec2        u_TextureTiling           = vec2(1.0, 1.0);
uniform vec2        u_TextureOffset           = vec2(0.0, 0.0);
uniform vec4 u_Diffuse    = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 u_Specular   = vec4(1.0, 1.0, 1.0, 1.0);
uniform float u_Shininess = 100.0;
uniform float       u_HeightScale             = 0.0;
uniform bool        u_EnableNormalMapping     = false;
uniform sampler2D   u_DiffuseMap;
uniform sampler2D   u_SpecularMap;
uniform sampler2D   u_NormalMap;
uniform sampler2D   u_HeightMap;
uniform sampler2D   u_MaskMap;

/* Global variables */
vec3 g_Normal;
vec2 g_TexCoords;
vec3 g_ViewDir;
vec4 g_DiffuseTexel;
vec4 g_SpecularTexel;
vec4 g_HeightTexel;
vec4 g_NormalTexel;

out vec4 FragColor;

vec3 UnPackColor(float p_Target)
{
    return vec3
    (
      /* '& 0xff' to get the last 8 bits of the value */
      /* Then divide by 255 to get a value between 0 - 1 */
      float((uint(p_Target) >> 24) & 0xff) * 0.0039215686274509803921568627451,
      float((uint(p_Target) >> 16) & 0xff) * 0.0039215686274509803921568627451,
      float((uint(p_Target) >> 8)  & 0xff) * 0.0039215686274509803921568627451
    );
}

bool PointInAABB(vec3 p_Point, vec3 p_AabbCenter, vec3 p_AabbHalfSize)
{
    return
    (
        p_Point.x > p_AabbCenter.x - p_AabbHalfSize.x && p_Point.x < p_AabbCenter.x + p_AabbHalfSize.x &&
        p_Point.y > p_AabbCenter.y - p_AabbHalfSize.y && p_Point.y < p_AabbCenter.y + p_AabbHalfSize.y &&
        p_Point.z > p_AabbCenter.z - p_AabbHalfSize.z && p_Point.z < p_AabbCenter.z + p_AabbHalfSize.z
    );
}

mat3 fromEuler(vec3 euler) {
    // Convert degrees to radians
    euler = euler * 3.14159265358979323846 / 180.0;

    float cosX = cos(euler.x);
    float sinX = sin(euler.x);
    float cosY = cos(euler.y);
    float sinY = sin(euler.y);
    float cosZ = cos(euler.z);
    float sinZ = sin(euler.z);

    // Define rotation matrices according to the right-hand rule
    mat3 rotX = mat3(
        1.0, 0.0, 0.0,
        0.0, cosX, sinX,
        0.0, -sinX, cosX
    );

    mat3 rotY = mat3(
        cosY, 0.0, -sinY,
        0.0, 1.0, 0.0,
        sinY, 0.0, cosY
    );

    mat3 rotZ = mat3(
        cosZ, sinZ, 0.0,
        -sinZ, cosZ, 0.0,
        0.0, 0.0, 1.0
    );

    // The combination order of Z, Y, X is typical for many applications
    // But this can be adjusted if another order is required
    return rotZ * rotY * rotX;  // Z first, then Y, then X
}

bool PointInOBB(vec3 p_Point, vec3 p_ObbCenter, vec3 p_ObbHalfSize, vec3 p_OBBRotation) {

    mat3 rotMatrix = fromEuler(p_OBBRotation);
    vec3 localPoint = rotMatrix * (p_Point - p_ObbCenter);

    return (
        localPoint.x >= -p_ObbHalfSize.x && localPoint.x <= p_ObbHalfSize.x &&
        localPoint.y >= -p_ObbHalfSize.y && localPoint.y <= p_ObbHalfSize.y &&
        localPoint.z >= -p_ObbHalfSize.z && localPoint.z <= p_ObbHalfSize.z
    );
}

vec2 ParallaxMapping(vec3 p_ViewDir)
{
    const vec2 parallax = p_ViewDir.xy * u_HeightScale * texture(u_HeightMap, g_TexCoords).r;
    return g_TexCoords - vec2(parallax.x, 1.0 - parallax.y);
}

vec3 BlinnPhong(vec3 p_lightDir, vec3 p_lightColor, float p_luminosity)
{
    const vec3 lightDir             = normalize(p_lightDir);
    const vec3 halfwayDir           = normalize(p_lightDir + g_ViewDir);
    const float diffuseCoefficient  = max(dot(lightDir, g_Normal), 0.0);
    const float specularCoefficient = pow(max(dot(halfwayDir, g_Normal), 0.0), u_Shininess * 2.0);

    vec3 diffuse  = g_DiffuseTexel.rgb * diffuseCoefficient * p_lightColor * p_luminosity;
    vec3 specular = g_SpecularTexel.rgb * specularCoefficient * p_lightColor * p_luminosity;

    return diffuse + specular;
}

vec3 Phong(vec3 p_lightDir, vec3 p_lightColor, float p_luminosity)
{
    const vec3 lightDir             = normalize(p_lightDir);
    const vec3 reflectDir           = reflect(-lightDir, g_Normal);
    const float diffuseCoefficient  = max(dot(lightDir, g_Normal), 0.0);
    const float specularCoefficient = pow(max(dot(reflectDir, g_ViewDir), 0.0), u_Shininess);

    vec3 diffuse  = g_DiffuseTexel.rgb * diffuseCoefficient * p_lightColor * p_luminosity;
    vec3 specular = g_SpecularTexel.rgb * specularCoefficient * p_lightColor * p_luminosity;

    return diffuse + specular;
}

float LuminosityFromAttenuation(mat4 p_Light)
{
    const vec3  lightPosition   = p_Light[0].rgb;
    const float constant        = p_Light[0][3];
    const float linear          = p_Light[1][3];
    const float quadratic       = p_Light[2][3];

    const float distanceToLight = length(lightPosition - fs_in.FragPos);
    const float attenuation     = (constant + linear * distanceToLight + quadratic * (distanceToLight * distanceToLight));
    return 1.0 / attenuation;
}

vec3 CalcPointLight(mat4 p_Light)
{
    const vec3 lightPosition  = p_Light[0].rgb;
    const vec3 lightColor     = UnPackColor(p_Light[2][0]);
    const float intensity     = p_Light[3][3];

    const vec3  lightDirection  = normalize(lightPosition - fs_in.FragPos);
    const float luminosity      = LuminosityFromAttenuation(p_Light);

    return BlinnPhong(lightDirection, lightColor, intensity * luminosity);
}

vec3 CalculateDirectionalLight(mat4 p_light)
{
    return BlinnPhong(-p_light[1].rgb, UnPackColor(p_light[2][0]), p_light[3][3]);
}

vec3 CalcAmbientBoxLight(mat4 p_Light)
{
    const vec3  lightPosition   = p_Light[0].rgb;
    const vec3  lightColor      = UnPackColor(p_Light[2][0]);
    const float intensity       = p_Light[3][3];
    const vec3  size            = vec3(p_Light[0][3], p_Light[1][3], p_Light[2][3]);
    const vec3  rotation            = vec3(p_Light[0][3], p_Light[1][3], p_Light[2][3]);

    return PointInOBB(fs_in.FragPos, lightPosition, size, p_Light[1].rgb) ? g_DiffuseTexel.rgb * lightColor * intensity : vec3(0.0);
}

vec3 CalcAmbientSphereLight(mat4 p_Light)
{
    const vec3  lightPosition   = p_Light[0].rgb;
    const vec3  lightColor      = UnPackColor(p_Light[2][0]);
    const float intensity       = p_Light[3][3];
    const float radius          = p_Light[0][3];

    return distance(lightPosition, fs_in.FragPos) <= radius ? g_DiffuseTexel.rgb * lightColor * intensity : vec3(0.0);
}

vec3 CalcSpotLight(mat4 p_Light)
{
    /* Extract light information from light mat4 */
    const vec3  lightPosition   = p_Light[0].rgb;
    const vec3  lightForward    = p_Light[1].rgb;
    const vec3  lightColor      = UnPackColor(p_Light[2][0]);
    const float intensity       = p_Light[3][3];
    const float cutOff          = cos(radians(p_Light[3][1]));
    const float outerCutOff     = cos(radians(p_Light[3][1] + p_Light[3][2]));

    const vec3  lightDirection  = normalize(lightPosition - fs_in.FragPos);
    const float luminosity      = LuminosityFromAttenuation(p_Light);

    /* Calculate the spot intensity */
    const float theta           = dot(lightDirection, normalize(-lightForward)); 
    const float epsilon         = cutOff - outerCutOff;
    const float spotIntensity   = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
    
    return BlinnPhong(lightDirection, lightColor, intensity * spotIntensity * luminosity);
}

void main()
{
  g_TexCoords     = u_TextureOffset + vec2(mod(fs_in.TexCoords.x * u_TextureTiling.x, 1), mod(fs_in.TexCoords.y * u_TextureTiling.y, 1));

   if (u_HeightScale > 0)
        g_TexCoords = ParallaxMapping(normalize(fs_in.TangentViewPos - fs_in.TangentFragPos));

   if (texture(u_MaskMap, g_TexCoords).r != 0.0)
   {
      g_ViewDir       = normalize(ubo_ViewPos - fs_in.FragPos);
      g_DiffuseTexel  = texture(u_DiffuseMap,  g_TexCoords) * u_Diffuse;
      g_SpecularTexel = texture(u_SpecularMap, g_TexCoords) * u_Specular;


      if (u_EnableNormalMapping)
        {
            g_Normal = texture(u_NormalMap, g_TexCoords).rgb;
            g_Normal = normalize(g_Normal * 2.0 - 1.0);   
            g_Normal = normalize(fs_in.TBN * g_Normal);
        }
        else
        {
            g_Normal = normalize(fs_in.Normal);
        }

      vec3 lightSum = vec3(0.0);

      for (int i = 0; i < ssbo_Lights.length(); ++i)
      {
          switch(int(ssbo_Lights[i][3][0]))
          {
              case 0: lightSum += CalcPointLight(ssbo_Lights[i]); break;
              case 1: lightSum += CalculateDirectionalLight(ssbo_Lights[i]);  break;
              case 2: lightSum += CalcAmbientBoxLight(ssbo_Lights[i]);    break;
              case 3: lightSum += CalcAmbientSphereLight(ssbo_Lights[i]); break;
              case 4: lightSum += CalcSpotLight(ssbo_Lights[i]); break;
          }
      }

      FragColor = vec4(lightSum, g_DiffuseTexel.a);
    }
    else
    {
        FragColor = vec4(0.0);
    }
}
