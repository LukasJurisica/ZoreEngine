#shaderstage vertex
#version 430 core

layout (std140, binding = 0) uniform shaderData {
	mat4 vp_mat;
	vec3 cameraPos;
	float time;
};

uniform vec4 model;

layout(location = 0) in ivec2 position;

void main() {
	gl_Position = vp_mat * vec4((position * model.w) + model.xy, model.z, 1.0);
}





#shaderstage fragment
#version 430 core
out vec4 FragColor;

uniform vec4 color;

void main() 
{
    FragColor = color;
} 