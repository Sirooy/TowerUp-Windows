#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform vec2 u_LightPosition;

void main()
{
	const float SIZE = 5.0f;

	vec2 pos0 = gl_in[0].gl_Position.xy;
	vec2 pos1 = gl_in[1].gl_Position.xy;
	vec2 pos2 = gl_in[2].gl_Position.xy;
	vec2 pos01 = pos0 + normalize(pos0 - u_LightPosition) * SIZE;
	vec2 pos11 = pos1 + normalize(pos1 - u_LightPosition) * SIZE;
	vec2 pos21 = pos2 + normalize(pos2 - u_LightPosition) * SIZE;

	gl_Position = vec4(pos0, 0.0, 1.0);  EmitVertex();
	gl_Position = vec4(pos1, 0.0, 1.0);  EmitVertex();
	gl_Position = vec4(pos01, 0.0, 1.0); EmitVertex();
	EndPrimitive();

	gl_Position = vec4(pos01, 0.0, 1.0); EmitVertex();
	gl_Position = vec4(pos11, 0.0, 1.0); EmitVertex();
	gl_Position = vec4(pos1, 0.0, 1.0);  EmitVertex();
	EndPrimitive();

	gl_Position = vec4(pos0, 0.0, 1.0);  EmitVertex();
	gl_Position = vec4(pos2, 0.0, 1.0);  EmitVertex();
	gl_Position = vec4(pos21, 0.0, 1.0); EmitVertex();
	EndPrimitive();

	gl_Position = vec4(pos21, 0.0, 1.0); EmitVertex();
	gl_Position = vec4(pos01, 0.0, 1.0); EmitVertex();
	gl_Position = vec4(pos0, 0.0, 1.0);  EmitVertex();
	EndPrimitive();

	gl_Position = vec4(pos0, 0.0, 1.0); EmitVertex();
	gl_Position = vec4(pos1, 0.0, 1.0); EmitVertex();
	gl_Position = vec4(pos2, 0.0, 1.0); EmitVertex();
	EndPrimitive();
}