#version 410 core

layout(location = 0) out vec3 out_position; 
layout(location = 1) out vec3 out_color;
layout(location = 2) out vec3 out_normal;
layout(location = 3) out vec2 out_velocity; 

uniform sampler2D tex_color;


uniform mat4 model;
uniform mat4 view_projection;

uniform mat4 old_model;
uniform mat4 old_view_projection;

uniform float dt;


vec3 project(vec3 v, vec3 axis) {
	return axis*dot(v,axis)/max(dot(axis, axis), 0.0001);
}

in ToFrag {
	vec3 normal;
	vec3 pos;
	vec2 uv;
	vec4 proj_pos;
	vec4 prev_proj_pos;
} in_data;


void main () {
	vec2 prev_pos = (in_data.prev_proj_pos.xy/in_data.prev_proj_pos.w)*0.5+0.5;
	vec2 pos =      (in_data.proj_pos.xy/in_data.proj_pos.w)*0.5+0.5;

	out_velocity = pos/dt-prev_pos/dt;
	out_position = in_data.pos;
	out_color = texture(tex_color, in_data.uv).rgb;
	out_normal = normalize(in_data.normal);
}

