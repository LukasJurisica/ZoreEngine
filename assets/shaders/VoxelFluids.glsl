#shaderstage vertex
#version 430 core

layout(location = 0) in int vertexID;
layout(location = 1) in uvec2 face;
layout (std140, binding = 0) uniform shaderData { mat4 vp_mat; mat4 ivp_mat; vec3 cameraPos; float time; vec2 res; };
layout (std140, binding = 1) uniform modelData { ivec4 offsets[32]; };
uniform ivec3 chunkPos;
out flat unsigned int fluidID;
out flat unsigned int dir;
out vec3 position;

float hash13(in vec3 p3) {
	p3 = fract(p3 * 0.1031);
    p3 += dot(p3, p3.yzx + 31.32);
    return fract((p3.x + p3.y) * p3.z);
}

void main() {
	unsigned int x = (face.x >> 26) & 0x3F;
	unsigned int y = (face.x >> 17) & 0x1FF;
	unsigned int z = (face.x >> 11) & 0x3F;

	fluidID = (face.y >> 16) & 0xFFFF;
	dir = face.y & 0x7;
	position = offsets[(dir * 4) + vertexID].xyz + vec3(x, y, z) + chunkPos;

	// Apply waves
	vec3 pos = position;
	float wave = sin(6.28 * (time * 0.7 + pos.x * 0.14 + pos.z * 0.07)) + sin(6.28 * (time * 0.5 + pos.x * 0.10 + pos.z * 0.20));
	pos.y += (wave * 0.0125) - 0.1;

	gl_Position = vp_mat * vec4(pos, 1.f);
}




#shaderstage fragment
#version 430 core

in flat unsigned int fluidID;
in flat unsigned int dir;
in vec3 position;
out vec4 FragColor;

const vec4 colour[2] = vec4[2]( vec4(0.314, 0.655, 0.749, 1.0), vec4(0.812, 0.063, 0.125, 0.8) );
const int blockSubDivisions = 4;

float hash13(in vec3 p3) {
	p3 = fract(p3 * 0.1031);
    p3 += dot(p3, p3.yzx + 31.32);
    return fract((p3.x + p3.y) * p3.z);
}

void main() {
	vec3 pos = position * blockSubDivisions;
	// If we remove the 1 -, block faces will have noise that wraps around it's faces
	unsigned int offset = 1 - (dir & 1);
	unsigned int axis = dir >> 1;
	pos[axis] = round(pos[axis] - offset);
	float noise = hash13(floor(pos)) * 0.1f + 0.9f;

	FragColor = colour[fluidID] * noise;
} 