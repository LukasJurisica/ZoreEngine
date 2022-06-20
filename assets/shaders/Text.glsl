#shaderstage vertex
#version 430 core

layout(location = 0) in int vertexID;
layout (std140, binding = 0) uniform shaderData { mat4 vp_mat; mat4 ivp_mat; vec3 cameraPos; float time; };
uniform vec4 model;

const vec2 position[4] = vec2[4](vec2(-1, 1), vec2(-1,-1), vec2( 1, 1), vec2( 1,-1));

void main() {
	gl_Position = vp_mat * vec4((position[vertexID] * model.w) + model.xy, model.z, 1.0);
}




#shaderstage fragment
#version 430 core

uniform vec4 color;
out vec4 FragColor;

void main() {
    FragColor = color;
} 