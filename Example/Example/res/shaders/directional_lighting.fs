#version 460 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
}; 

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform float UvXValue;
uniform float UvYValue;

void main()
{
    vec2 Uvs = TexCoords;

   if(Uvs.x > TexCoords.x * 0.5)
    {
        Uvs *= vec2(UvXValue, UvYValue);
    }
    if(Uvs.x < TexCoords.x * 0.5)
    {
        Uvs *= vec2(-UvXValue, -UvYValue);
    }

    // ambient
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, Uvs).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, Uvs).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, Uvs).rgb;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}