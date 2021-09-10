#shaderstage vertex
#version 430 core

layout(location = 0) in ivec2 position;

out vec2 uv;

void main() {
	uv = (position + 1) * 0.5;
	gl_Position = vec4(position, 0.0, 1.0);
}





#shaderstage fragment
#version 430 core

in vec2 uv;
layout(binding = 0) uniform sampler2D tex;
out vec4 FragColor;

void main() 
{
    FragColor = texture(tex, uv);
} 