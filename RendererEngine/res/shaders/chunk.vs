#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;
layout (location = 1) in vec2 aTexCoord;

layout(std430, binding = 0) buffer BlockData
{
    uint data[];
};

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 BlockOffset;

const vec3 chunkHalfSize = vec3(8.0f, 8.0f, 8.0f);

vec3 CalculateBlockOffset()
{
	float posX = float(data[gl_InstanceID] % 16);
    float posY = float((data[gl_InstanceID] >> 8) % 16);
    float posZ = float((data[gl_InstanceID] >> 16) % 16);

    return vec3(posX, posY, posZ);
}

void main()
{
	BlockOffset = CalculateBlockOffset();
	FragPos = vec3(model * vec4(aPos + BlockOffset - chunkHalfSize, 1.0));

	Normal = mat3(transpose(inverse(model))) * aNormal;  
	TexCoord = aTexCoord;
    
	gl_Position = projection * view * vec4(FragPos, 1.0);
}