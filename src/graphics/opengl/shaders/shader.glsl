#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

uniform mat4 u_viewProjectionMatrix;

out vec4 colour_interp;

void main()
{
	colour_interp = vec4(colour, 1.0);
    gl_Position = u_viewProjectionMatrix * vec4(position, 1.0);
}

#type fragment
#version 330 core

in vec4 colour_interp;

layout(location = 0) out vec4 colour;

void main()
{
   colour = colour_interp;
}