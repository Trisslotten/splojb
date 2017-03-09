#version 410 core
 
out vec4 color;

in ToFrag {
	vec3 normal;
	vec3 frag_pos;
	vec2 uv;
} in_data;

void main () {
	color = vec4(1.0,1.0,1.0,1.0);
}

