#version 410

uniform sampler2D tex_position;
uniform sampler2D tex_color;
uniform sampler2D tex_normal;

layout(location = 0) out vec4 out_color;

const vec3 light_position = vec3(0,0.4,2);

uniform vec3 camera_position;

in vec2 tex_coords;

void main() {
	vec3 position = texture(tex_position, tex_coords).xyz;
	vec3 color = texture(tex_color, tex_coords).rgb;
	vec3 normal = texture(tex_normal, tex_coords).xyz;

    vec3 lighting = color * 0.05;

	vec3 light_dir = normalize(light_position - position);
	vec3 diffuse = max(dot(normal, light_dir), 0)*color;

	vec3 camera_dir = normalize(camera_position - position);
	vec3 half_dir = normalize(light_dir + camera_dir);
	vec3 specular = pow(max(dot(normal, half_dir), 0), 16) * color;

	lighting += diffuse;
	lighting += specular;

	out_color = vec4(lighting, 1.0);
	out_color = vec4(color, 1.0);
}