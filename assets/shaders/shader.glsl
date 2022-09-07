#shaderstage vertex
#version 430 core

// Uniform Data
layout(std140, binding = 0) uniform dynamicShaderData { mat4 vp_mat; mat4 inv_vp_mat; vec3 cam_pos; float time; };
uniform vec4 model;

// Constants
const vec2 position[4] = vec2[4](vec2(-1, 1), vec2(-1,-1), vec2( 1, 1), vec2( 1,-1));

void main() {
	gl_Position = vp_mat * vec4((position[gl_VertexID] * model.w) + model.xy, model.z, 1.0);
}




#shaderstage fragment
#version 430 core

// Uniform Data
uniform vec4 color;

// Fragment Data
out vec4 FragColor;

void main() {
    FragColor = color;
} 