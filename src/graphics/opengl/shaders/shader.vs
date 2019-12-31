#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

out vec4 colour_interp;

void main()
{
	colour_interp = vec4(colour, 1.0);
    gl_Position = vec4(position, 1.0);
}