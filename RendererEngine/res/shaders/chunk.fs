#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

in flat uint blockType;

uniform sampler2D texture1;

void main()
{   
    if(blockType == 0)
        discard;

    FragColor = texture(texture1, TexCoord);
}