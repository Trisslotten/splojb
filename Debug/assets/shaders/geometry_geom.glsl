#version 410

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
};

in gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
} gl_in[];

out ToFrag {
	vec3 normal;
	vec3 frag_pos;
	vec2 uv;
} out_data;

in FromVertex {
	vec3 normal;
	vec3 pos;
	vec2 uv;
} in_data[];

uniform mat4 model;

void main()
{
	vec3 v1 = in_data[1].pos - in_data[0].pos;
	vec3 v2 = in_data[2].pos - in_data[0].pos;

	vec3 normal = normalize(cross(v1, v2));
	mat4 vector_matrix = transpose(inverse(model));
	int i;
	for (i = 0; i < gl_in.length(); i++)
	{
		gl_Position = gl_in[i].gl_Position;
		if(i==0) {
			out_data.uv = vec2(0,0);
		} else if(i == 1) {
			out_data.uv = vec2(1,0);
		} else if(i ==2) {
			out_data.uv = vec2(0,1);
		}
		out_data.frag_pos = in_data[i].pos;
		out_data.normal = normal;
		EmitVertex();
	}
	EndPrimitive();
}
