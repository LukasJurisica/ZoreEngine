//VERTEX SHADER
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
uniform mat4 u_viewProjectionMatrix;
out vec4 color_interp;

void main() {
	color_interp = vec4(color, 1.0);
	gl_Position = u_viewProjectionMatrix * vec4(position, 1.0);
}
#type vertex






// FRAGMENT SHADER
#version 330 core

in vec4 color_interp;
layout(location = 0) out vec4 color;

void main() {
	color = color_interp;
}
#type fragment