#version 450

layout(location=0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 TexCoord;

layout(location=0) out vec4 outColor;

layout(binding = 0) uniform MODEL
{
	mat4 world;
	mat4 view;
	mat4 proj;
};

layout(binding = 1) uniform MATERIAL
{
	vec4 albedo;
	float   metallic;
    float   roughness;
};

const vec4 lightDirection = vec4(0, -1, 1, 0);

void main()
{
	vec4 worldPosition = world * vec4(Position, 1.0);
	mat4 viewProjection = proj * view;
	gl_Position = viewProjection * worldPosition;

	// 法線の計算
	vec4 worldNormal = world * vec4(Normal, 0.0);
	worldNormal = normalize(worldNormal);

	// ライトの計算
	float light = -dot(worldNormal.xyz, lightDirection.xyz);

	outColor	= vec4(albedo.xyz * light, 1.0);
}
