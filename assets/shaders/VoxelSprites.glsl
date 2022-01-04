#shaderstage vertex
#version 430 core

layout (std140, binding = 0) uniform shaderData { mat4 vp_mat; vec3 cameraPos; float time; };
layout (std140, binding = 1) uniform modelData { ivec4 offsets[32]; };
uniform ivec3 chunkPos;

layout(location = 0) in int vertexID;
layout(location = 1) in uvec2 face;
out flat unsigned int spriteID;
out vec2 uv;

void main() {
	unsigned int x = (face.x >> 26) & 0x3F;
	unsigned int y = (face.x >> 17) & 0x1FF;
	unsigned int z = (face.x >> 11) & 0x3F;

	// Send blockID to fragment shader
	spriteID = (face.y >> 16) & 0xFFFF;
	// Determine the direction/normal vector of the face being drawn, and send to fragmant shader
	unsigned int dir = face.y & 0x7;
	// Send UV coordinate to fragment shader
	uv = vec2(floor(vertexID >> 1), vertexID & 1);
	// Determine vertex offset, and send to fragment shader
	vec3 position = offsets[(dir * 4) + vertexID].xyz + vec3(x, y, z) + chunkPos;

	gl_Position = vp_mat * vec4(position, 1.f);
}




#shaderstage fragment
#version 430 core

const vec3 color[8] = vec3[8]( vec3(0.3, 0.3, 0.3), vec3(0, 0, 1), vec3(0.403, 0.831, 0.023), vec3(0, 1, 1), vec3(1, 0, 0), vec3(1, 0, 1), vec3(1, 1, 0), vec3(0.631, 0.666, 0.639) );

in flat unsigned int spriteID;
in vec2 uv;
out vec4 FragColor;

void main() {
	//vec3 c = color[spriteID];
	//c.xy *= (uv * 0.5) + 0.5;
	FragColor = vec4(uv.x, uv.y, 1.0, 1.0);
} 