#version 410 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

uniform mat4 model;
uniform mat4 view_projection;

out ToFrag {
	vec3 normal;
	vec3 frag_pos;
	vec2 uv;
} out_data;

void main() {
	out_data.normal = (transpose(inverse(model))*vec4(vertex_normal, 1.0)).xyz;
	out_data.frag_pos = (model*vec4(vertex_position, 1.0)).xyz;
	out_data.uv = vertex_uv;

	gl_Position = view_projection*model*vec4(vertex_position, 1.0); 
}
