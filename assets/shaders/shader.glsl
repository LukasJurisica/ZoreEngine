#shaderstage vertex
#version 430 core

layout (std140, binding = 0) uniform shaderData {
	mat4 vp_mat;
	vec3 cameraPos;
	float time;
};

const vec2 position[4] = vec2[4](
	vec2(-1, 1), vec2(-1,-1), vec2( 1, 1), vec2( 1,-1)
);

layout(location = 0) in int vertexID;

uniform vec4 model;

void main() {
	gl_Position = vp_mat * vec4((position[vertexID] * model.w) + model.xy, model.z, 1.0);
}




#shaderstage fragment
#version 430 core
out vec4 FragColor;

uniform vec4 color;

void main() 
{
    FragColor = color;
} 