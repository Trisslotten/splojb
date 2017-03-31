#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 tex_coords;

uniform mat4 model;
uniform mat4 view_projection;

uniform mat4 prev_model;
uniform mat4 prev_view_projection;

out ToFrag {
	vec3 normal;
	vec3 pos;
	vec2 uv;
	vec4 proj_pos;
	vec4 prev_proj_pos;
} out_data;


void main()
{
    vec4 world = model * vec4(position, 1.0);
	out_data.pos = world.xyz;
    out_data.uv = tex_coords;
	out_data.uv.y = 1-out_data.uv.y;
	out_data.normal = (transpose(inverse(model))*vec4(in_normal, 0)).xyz;
	out_data.prev_proj_pos = prev_view_projection * prev_model * vec4(position, 1.0);
    gl_Position = view_projection * world;

	out_data.proj_pos = gl_Position;
}