#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 tex_coords;

uniform mat4 model;
uniform mat4 view_projection;

uniform vec3 axis;
uniform vec3 translation;
uniform float angle;
uniform float dt;

out ToFrag {
	vec3 normal;
	vec3 pos;
	vec3 scaled;
	vec2 uv;
	vec2 velocity;
} out_data;


void main()
{
    vec4 world = model * vec4(position, 1.0);
	vec4 scaled = model * vec4(position, 0.0);
	out_data.scaled = scaled.xyz;
	out_data.pos = world.xyz;
    out_data.uv = tex_coords;
	out_data.uv.y = 1-out_data.uv.y;
	out_data.normal = (transpose(inverse(model))*vec4(in_normal, 0)).xyz;
    gl_Position = view_projection * world;

}