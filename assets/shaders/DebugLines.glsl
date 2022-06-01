#shaderstage vertex
#version 430 core

layout (std140, binding = 0) uniform shaderData { mat4 vp_mat; mat4 v_mat; mat4 p_mat; vec3 cameraPos; float time; };
uniform ivec3 offset;
layout(location = 0) in ivec3 pos;

void main() {
	vec3 p = (pos - 0.5) * 1.0015 + 0.5;
	gl_Position = vp_mat * vec4(p + offset, 1.f);
}




#shaderstage fragment
#version 430 core

out vec4 FragColor;

void main() {
	FragColor = vec4(0.0, 0.0, 0.0, 1.0);
} 