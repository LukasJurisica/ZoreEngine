#version 130

// Vertex buffer
in vec3 vertex;
in vec3 normal;
in vec3 color;
in vec3 uv;

// Uniform (global) buffer
uniform mat4 world_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

uniform mat4 pvwMat;

// Attributes forwarded to the fragment shader
out vec4 color_interp;

void main()
{
    gl_Position = pvwMat * vec4(vertex, 1.0);
    color_interp = vec4(color, 1.0);
}