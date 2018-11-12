#version 460 core
out vec4 FragColor;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoord;

uniform vec3 viewPos;

// texture sampler
uniform sampler2D texture3;
uniform Light light;

void main()
{
     // ambient
    vec3 ambient = light.ambient * texture(texture3, TexCoord).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(texture3, TexCoord).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess);
    vec3 specular = light.specular * spec * texture(texture3, TexCoord).rgb;  
        

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}