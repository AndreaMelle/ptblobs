#version 430 core

const vec2 quadVertices[4] = vec2[] (
	vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0)
);

const vec2 quadUVs[4] = vec2[] (
	vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0)
);

out VS_OUT
{
  vec2 UV;
} vs_out;

void main()
{
	vs_out.UV = quadUVs[gl_VertexID];
	gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
}