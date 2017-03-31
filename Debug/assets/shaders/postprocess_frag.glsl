#version 410

uniform sampler2D tex_color;
uniform sampler2D tex_velocity;

in vec2 tex_coords;

layout(location = 0) out vec4 out_color;

const float samples = 21.0;
const float size_mult = 0.01;

void main() {
	vec3 color = vec3(0);
	vec2 velocity = texture(tex_velocity, tex_coords).xy;
	float speed = length(velocity)*size_mult;
	vec2 line_dir = normalize(velocity);

	
	if(speed > 0.000001) {
		float t = -speed;
		color += texture(tex_color, tex_coords + t*line_dir).rgb/samples;
		for(int i = 0; i < int(samples); i++) {
			t += 2.0*speed/samples;
			color += texture(tex_color, tex_coords + t*line_dir).rgb/samples;
		}
	} else {
		color = texture(tex_color, tex_coords).rgb;
	}
	//speed = clamp(length(velocity), 0.0, 1.0);
	

	//color = vec3(abs(velocity), 0.0);

	out_color = vec4(color, 1.0);
}