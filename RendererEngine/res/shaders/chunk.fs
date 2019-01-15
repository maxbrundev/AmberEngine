#version 430 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D texture1;

void main()
{   vec3 norm = normalize(Normal);
    //FragColor = vec4(norm * 0.5 + 0.5, 1.0);
    FragColor = texture(texture1, TexCoord);
}