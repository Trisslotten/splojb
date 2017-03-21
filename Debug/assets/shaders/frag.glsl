#version 410 core
 
out vec4 color;

in ToFrag {
	vec3 normal;
	vec3 frag_pos;
	vec2 uv;
} in_data;

const vec3 light_pos = vec3(10.0,0.0,2.0);

float rand(vec3 co){
    return fract(sin(dot(co ,vec3(12.9898,78.233,3.3767))) * 43758.5453);
}

void main () {
	
	vec3 l = normalize(light_pos - in_data.frag_pos);
	vec3 n = normalize(in_data.normal);
	float diff = dot(n, l);
	color = vec4(diff*vec3(1.0), 1.0);
}

