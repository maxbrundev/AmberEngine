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
uniform sampler2D texture1;

out flat uint blockType;

const ivec2 c_atlasSize = textureSize(texture1, 0);
const float c_textureResolution = 16.0f;
const float c_textureWidth = c_atlasSize.x / c_textureResolution;
const float c_textureHeight = c_atlasSize.y / c_textureResolution;

vec3 BlockOffset;

const vec3 chunkHalfSize = vec3(8.0f, 8.0f, 8.0f);

vec3 CalculateBlockOffset()
{
	float posX = float(data[gl_InstanceID] % 16);
    float posY = float((data[gl_InstanceID] >> 8) % 16);
    float posZ = float((data[gl_InstanceID] >> 16) % 16);

    return vec3(posX, posY, posZ);
}


bool IsTopFace()
{
    return aNormal.y > 0;
}

bool IsBottomFace()
{
    return aNormal.y < 0;
}

bool IsSideFace()
{
    return aNormal.y == 0;
}

bool IsXFace()
{
    return aNormal.y == 0 && aNormal.x != 0;
}

bool IsZFace()
{
    return aNormal.y == 0 && aNormal.z != 0;
}


vec2 GetTexture(uint p_x, uint p_y)
{
    vec2 result = aTexCoord;
    
    result.x /= c_textureWidth;
    result.y /= c_textureHeight;

    result.x += p_x / c_textureWidth;
    result.y += (c_textureHeight - 1 - p_y) / c_textureHeight;

    return result;
}

vec2 GetTripleTexture(uint p_topX, uint p_topY, uint p_bottomX, uint p_bottomY, uint p_sideX, uint p_sideY)
{
            if (IsTopFace())       return GetTexture(p_topX,    p_topY);
    else    if (IsBottomFace())    return GetTexture(p_bottomX, p_bottomY);
    else    if (IsSideFace())      return GetTexture(p_sideX,   p_sideY);
}

vec2 CalculateAtlasTextureCoordinates()
{
    switch (blockType)
    {
        case 1: return GetTripleTexture(0,0, 2,0, 1,0);     // 01: GRASS
        case 2: return GetTexture(2,0);                     // 02: DIRT
        case 3: return GetTexture(3,1);                     // 03: GRAVEL
        case 4: return GetTexture(0,1);                     // 04: STONE
        case 5: return GetTexture(4,1);                     // 05: COBBLESTONE
        case 6: return GetTripleTexture(1,3, 1,3, 0,3);     // 06: WOOD
        case 7: return GetTexture(2,2);                     // 07: WOODEN_PLANKS
        case 8: return GetTexture(2,1);                     // 08: SAND
        case 9: return GetTexture(1,2);                     // 09: GLASS
        case 10: return GetTexture(2,3);                    // 10: BRICK
        case 11: return GetTexture(3,0);                    // 11: STONE_BRICK
        case 12: return GetTexture(4,0);                    // 12: LEAVES
        case 13: return GetTripleTexture(1,1, 5,5, 5,5);    // 13: WATER
        case 14: return GetTexture(0,2);                    // 14: LAVA
        case 15: return GetTripleTexture(5,5, 5,5, 3,2);    // 15: RED_FLOWER
        case 16: return GetTripleTexture(5,5, 5,5, 3,3);    // 16: YELLOW_FLOWER
        case 17: return GetTexture(5,0);                    // 17: COAL_ORE
    }

    return vec2(0, 0);
}

void main()
{
	blockType = (data[gl_InstanceID] >> 24) % 16;
	BlockOffset = CalculateBlockOffset();
	FragPos = vec3(model * vec4(aPos + BlockOffset, 1.0));

	Normal = mat3(transpose(inverse(model))) * aNormal;  

    TexCoord = CalculateAtlasTextureCoordinates();

	gl_Position = projection * view * vec4(FragPos, 1.0);
}