#shaderstage vertex
#version 430 core

layout (std140, binding = 0) uniform shaderData { mat4 vp_mat; mat4 v_mat; mat4 p_mat; vec3 cameraPos; float time; };
layout (std140, binding = 1) uniform modelData { ivec4 offsets[32]; };
uniform ivec3 pos;

out vec2 uv;

void main() {
	int dir = gl_InstanceID;
	uv = vec2(gl_VertexID >> 1, gl_VertexID & 1);
	vec3 position = offsets[(dir * 4) + gl_VertexID].xyz + pos;

	gl_Position = vp_mat * vec4(position, 1.f);
}




#shaderstage fragment
#version 430 core

in vec2 uv;
out vec4 FragColor;

void main() {
	float t = 1.0 / 128.0;
	if (uv.x > t && uv.x < (1 - t) && uv.y > t && uv.y < (1.0 - t))
		discard;
	FragColor = vec4(0.0, 0.0, 0.0, 1.0);
} 