#version 450

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 TexCoord;

layout(location=0) out vec4 outColor;
layout(location=1) out vec2 outUV;


layout(set = 0, binding=0) uniform MODEL
{
	mat4 world;
	mat4 view;
	mat4 proj;
};

layout(set = 0, binding = 1) uniform MATERIAL
{
	vec4 albedo;
	float   metallic;
    float   roughness;
};

void main()
{
	mat4 pvw	= proj * view * world;
	gl_Position = pvw * vec4(Position, 1.0);
	outColor	= albedo;
	outUV		= TexCoord;
}
