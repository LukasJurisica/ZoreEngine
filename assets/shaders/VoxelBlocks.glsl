#shaderstage vertex
#version 430 core

layout(location = 0) in int vertexID;
layout(location = 1) in uvec2 face;
layout (std140, binding = 0) uniform shaderData { mat4 vp_mat; vec3 cameraPos; float time; };
layout (std140, binding = 1) uniform modelData { ivec4 offsets[32]; };
uniform ivec3 chunkPos;
out flat unsigned int blockID;
out flat unsigned int dir;
out flat vec4 ao;
out vec3 position;
out vec2 uv;
out float dist;

const float ao_weights[4] = float[4](1.0f, 0.8f, 0.6f, 0.4f);

void main() {
	unsigned int x = (face.x >> 26) & 0x3F;
	unsigned int y = (face.x >> 17) & 0x1FF;
	unsigned int z = (face.x >> 11) & 0x3F;

	blockID = (face.y >> 16) & 0xFFFF;
	ao = vec4( ao_weights[(face.y >> 14) & 0x3], ao_weights[(face.y >> 12) & 0x3], ao_weights[(face.y >> 10) & 0x3], ao_weights[(face.y >> 8 ) & 0x3] );
	dir = face.y & 0x7;
	uv = vec2(floor(vertexID >> 1), vertexID & 1);
	position = offsets[(dir * 4) + vertexID].xyz + vec3(x, y, z) + chunkPos;

	dist = distance(position, cameraPos);
	//float fs = 4.1f;
	//y = 1 - sqrt(pow(1 - pow(x, 2.f/a), a));

	gl_Position = vp_mat * vec4(position, 1.f);
}




#shaderstage fragment
#version 430 core

in flat unsigned int blockID;
in flat unsigned int dir;
in flat vec4 ao;
in vec3 position;
in vec2 uv;
in float dist;
out vec4 FragColor;

const vec3 colour[4] = vec3[4]( vec3(0.482, 0.247, 0.0), vec3(0.403, 0.831, 0.023), vec3(0.631, 0.666, 0.639), vec3(0.584, 0.271, 0.208) );
const float lighting[6] = float[6](0.9f, 0.9f, 0.7f, 1.0f, 0.9f, 0.9f);
const int blockSubDivisions = 4;

float hash13(in vec3 p3) {
	p3 = fract(p3 * 0.1031);
    p3 += dot(p3, p3.yzx + 31.32);
    return fract((p3.x + p3.y) * p3.z);
}

float invMix(in float a, in float b, in float t) {
	return (t - a) / (b - a);
}

void main() {
	vec3 pos = position * blockSubDivisions;
	// If we remove the 1 -, block faces will have noise that wraps around it's faces
	unsigned int offset = 1 - (dir & 1);
	unsigned int axis = dir >> 1;
	pos[axis] = round(pos[axis]) - offset;

	float noise = hash13(floor(pos)) * 0.1f + 0.9f;
	float ao_interp = mix(mix(ao[0], ao[2], uv.x), mix(ao[1], ao[3], uv.x), uv.y);

	float mult = mix(ao_interp * noise * lighting[dir], 0.75, clamp(invMix(128, 512, dist), 0.0, 1.0));

	FragColor = vec4(colour[blockID] * mult, 1.0);
} 