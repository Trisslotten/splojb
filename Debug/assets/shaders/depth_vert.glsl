#version 410
//Inkluderar denna cuz Pipeline
out gl_PerVertex
{
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
};

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 texs;

void main()
{
    gl_Position = vec4(pos, 1.0f);
    texs = tex;
}
