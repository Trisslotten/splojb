#version 410 core

layout (location = 0) out vec4 color;
in vec2 texs;

const mat3 gaussian = mat3(
	0.077847, 0.123317, 0.077847,
	0.123317, 0.195346, 0.123317,
	0.077847, 0.123317, 0.077847
);

float rgb2luma(vec3 rgb){
    return sqrt(dot(rgb, vec3(0.299, 0.587, 0.114)));
}

uniform sampler2D depth;
uniform sampler2D in_color;

const float limit = 1.0;
const float mult = 50.0;


void main() 
{
	float N = rgb2luma(textureOffset(in_color, texs, ivec2(0,-1)).rgb);
	float S = rgb2luma(textureOffset(in_color, texs, ivec2(0, 1)).rgb);
	float W = rgb2luma(textureOffset(in_color, texs, ivec2(-1,0)).rgb);
	float E = rgb2luma(textureOffset(in_color, texs, ivec2(1,0)).rgb);
	// horizontal and vertical difference
	vec2 diff;
	diff.x = abs(N - S)*mult;
	diff.y = abs(E - W)*mult;
	
	vec3 accum;
	if(diff.x > limit || diff.y > limit) 
	{
		for(int x = -1; x <= 1; x++) 
		{
			for(int y = -1; y <= 1; y++) 
			{
				accum += textureOffset(in_color, texs, ivec2(x,y)).rgb*gaussian[x+1][y+1];
			}
		}
	} else {
		accum = texture(in_color, texs).rgb;
	}
	color = vec4(accum, 1.0);
	//color = vec4(diff, 0.0, 1.0);
}