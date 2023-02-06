#shaderstage vertex
#version 430 core

#define PI 3.1415926538

// Uniform Data
layout(std140, binding = 0) uniform dynamicShaderData { mat4 vp_mat; mat4 inv_vp_mat; vec3 cam_pos; float time; };
layout(std140, binding = 2) uniform modelData { ivec4 offsets[32]; };
uniform ivec3 chunkPos;

// Vertex Data
layout(location = 0) in uvec2 face;
out flat unsigned int spriteID;
out vec2 uv;

vec2 hash23(vec3 p3) {
	p3 = fract(p3 * vec3(0.1031, 0.1030, 0.0973));
	p3 += dot(p3, p3.yzx + 33.33);
	return fract((vec2(p3.x) + p3.yz) * p3.zy);
}

float GetWave(vec3 pos) {
	float waveStrength = 0.01;
	float t = time * 0.2;
	float amplitude = pos.y * waveStrength; // Wavestrenmgth is per model
	float wave = sin(2.0 * PI * t + (pos.z + pos.x) * 0.8);
	float wave2 = sin(2.0 * PI * (t + pos.z + pos.x) * 2.0);
	return (wave + wave2 * 0.4) * 0.06 * amplitude;
}

void main() {
	unsigned int x = (face.x >> 26) & 0x3F;
	unsigned int y = (face.x >> 17) & 0x1FF;
	unsigned int z = (face.x >> 11) & 0x3F;

	spriteID = ((face.y >> 16) & 0xFFFF) - 1;
	unsigned int dir = face.y & 0x7;
	uv = vec2(gl_VertexID >> 1, gl_VertexID & 1);
	vec3 offset = offsets[(dir * 4) + gl_VertexID].xyz;
	vec3 position = offset + vec3(x, y, z) + chunkPos;

	// Apply random offset to plant
	position.xz += hash23(vec3(x, y, z)) * 0.4 - 0.2;

	// Apply wave motion to plant
	//position.xz += sin(time) * 0.1 * offset.y;
	position.x += GetWave(position) * offset.y;

	gl_Position = vp_mat * vec4(position, 1.f);
}




#shaderstage fragment
#version 430 core

// Uniform Data
layout(binding = 2) uniform sampler2DArray sprites;

// Fragment Data
in flat unsigned int spriteID;
in vec2 uv;
out vec4 FragColor;

// Constants
const vec3 color[8] = vec3[8]( vec3(0.3, 0.3, 0.3), vec3(0, 0, 1), vec3(0.403, 0.831, 0.023), vec3(0, 1, 1), vec3(1, 0, 0), vec3(1, 0, 1), vec3(1, 1, 0), vec3(0.631, 0.666, 0.639) );

void main() {
	vec4 colour = texture(sprites, vec3(uv, spriteID));
	if (colour.a == 0.0)
		discard;
	FragColor = colour;
} 