#shader vertex
#version 460 core

layout (location = 0) in vec3 geo_Pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec3        FragPos;

} vs_out;

void main()
{
    vs_out.FragPos = vec3(model * vec4(geo_Pos, 1.0));
    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}

#shader fragment
#version 460 core

out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
} fs_in;

void main()
{
	float depth = gl_FragCoord.z;

    FragColor = vec4(vec3(depth), 1.0);
}