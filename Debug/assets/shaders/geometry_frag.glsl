#version 410 core

layout(location = 0) out vec3 out_position; 
layout(location = 1) out vec3 out_color;
layout(location = 2) out vec3 out_normal;
layout(location = 3) out vec2 out_velocity; 

uniform sampler2D tex_color;


uniform mat4 model;
uniform mat4 view_projection;

uniform vec3 u_axis;
uniform vec3 u_translation;
uniform float u_angle;
uniform float dt;


vec3 project(vec3 v, vec3 axis) {
	return axis*dot(v,axis)/max(dot(axis, axis), 0.0001);
}

vec3 safeNormalize(vec3 v) {
	if(v.x == 0 && v.y == 0 && v.z == 0) {
		return v;
	} else {
		return normalize(v);
	}
}

in ToFrag {
	vec3 normal;
	vec3 pos;
	vec3 scaled;
	vec2 uv;
	vec2 velocity;
} in_data;

const float mult = 100.0;

void main () {
	vec3 axis = u_axis/mult;
	vec3 translation = u_translation/mult;
	float angle = u_angle/mult;

	vec3 proj = in_data.scaled - project(in_data.scaled , axis); // r
	vec3 circle_tangent = length(proj)*safeNormalize(cross(axis, proj)); 
	vec3 velocity = (translation + circle_tangent*angle)/dt;
	vec4 proj_vel = view_projection * vec4(velocity, 0.0);
	out_velocity = proj_vel.xy;

	out_position = in_data.pos;
	out_color = texture(tex_color, in_data.uv).rgb;
	//out_color = vec3(out_velocity, 0.0);
	out_normal = normalize(in_data.normal);
}

