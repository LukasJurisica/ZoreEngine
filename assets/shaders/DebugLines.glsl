#shaderstage vertex
#version 430 core

// Uniform Data
layout(std140, binding = 0) uniform dynamicShaderData { mat4 vp_mat; mat4 inv_vp_mat; vec3 cam_pos; float time; };
uniform ivec3 offset;

// Vertex Data
layout(location = 0) in ivec3 pos;
out vec2 uv;

void main() {
	gl_Position = vp_mat * vec4(pos + offset, 1.f);
}




#shaderstage fragment
#version 430 core

// Uniform Data
layout(std140, binding = 1) uniform staticShaderData { vec2 inv_res; };
layout(binding = 1) uniform sampler2D depth;

// Fragment Data
out vec4 FragColor;

// Constants
const float bias = -0.0001;

void main() {
	vec2 uv = gl_FragCoord.xy * inv_res;
	float d = texture2D(depth, uv).x;
	float opacity = mix(1.0, 0.1, (d - gl_FragCoord.z) < bias);
	FragColor = vec4(0.1, 0.1, 0.1, opacity);
} 