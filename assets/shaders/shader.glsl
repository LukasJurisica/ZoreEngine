#shaderstage vertex
#version 430 core

layout (std140, binding = 0) uniform shaderData {
	mat4 vp_mat;
	vec3 cameraPos;
	float time;
};

layout(location = 0) in ivec2 position;

void main() {
	vec2 pos = position + vec2(sin(time), cos(time));
	gl_Position = vp_mat * vec4(pos * 0.5, -1.0, 1.0);
}





#shaderstage fragment
#version 430 core
out vec4 FragColor;

void main() 
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 