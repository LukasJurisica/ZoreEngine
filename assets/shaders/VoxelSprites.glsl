#shaderstage vertex
#version 430 core

layout(location = 0) in int vertexID;
layout(location = 1) in uvec2 face;
layout (std140, binding = 0) uniform shaderData { mat4 vp_mat; mat4 ivp_mat; vec3 cameraPos; float time; vec2 res; };
layout (std140, binding = 1) uniform modelData { ivec4 offsets[32]; };
uniform ivec3 chunkPos;
out flat unsigned int spriteID;
out vec2 uv;

void main() {
	unsigned int x = (face.x >> 26) & 0x3F;
	unsigned int y = (face.x >> 17) & 0x1FF;
	unsigned int z = (face.x >> 11) & 0x3F;

	spriteID = (face.y >> 16) & 0xFFFF;
	unsigned int dir = face.y & 0x7;
	uv = vec2(vertexID >> 1, vertexID & 1);
	vec3 position = offsets[(dir * 4) + vertexID].xyz + vec3(x, y, z) + chunkPos;

	gl_Position = vp_mat * vec4(position, 1.f);
}




#shaderstage fragment
#version 430 core

in flat unsigned int spriteID;
in vec2 uv;
layout(binding = 1) uniform sampler2DArray sprites;
out vec4 FragColor;

const vec3 color[8] = vec3[8]( vec3(0.3, 0.3, 0.3), vec3(0, 0, 1), vec3(0.403, 0.831, 0.023), vec3(0, 1, 1), vec3(1, 0, 0), vec3(1, 0, 1), vec3(1, 1, 0), vec3(0.631, 0.666, 0.639) );

void main() {
	vec4 colour = texture(sprites, vec3(uv, spriteID - 1));
	if (colour.a < 0.1)
		discard;
	FragColor = colour;
} 