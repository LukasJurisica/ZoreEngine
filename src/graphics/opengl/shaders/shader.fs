#version 130

in vec4 color_interp;

void main()
{
    gl_FragColor = color_interp;
}