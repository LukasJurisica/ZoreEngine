#version 330 core

in vec4 colour_interp;

layout(location = 0) out vec4 colour;

void main()
{
   colour = colour_interp;
}