#version 450

layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inUV;

layout(location=0) out vec4 outColor;

layout(set=1, binding=0) uniform texture2D    g_texture;
layout(set=2, binding=0) uniform sampler      g_sampler;

void main()
{
    vec4 texColor = texture(sampler2D(g_texture, g_sampler), inUV);
    
    if(texColor.a < 0.1f)
        discard;

    outColor = texColor * inColor;
}
