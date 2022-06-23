#shaderstage vertex
#version 430 core

layout(location = 0) in int vertexID;
layout(location = 1) in uvec2 string;
layout (std140, binding = 0) uniform shaderData { mat4 vp_mat; mat4 ivp_mat; vec3 cameraPos; float time; vec2 res; };
out vec2 uv;

const vec2 offsets[4] = vec2[4]( vec2(0, 1), vec2(0, 0), vec2(1, 1), vec2(1, 0) );
const vec2 charSize = vec2(50, 80) * 2;

void main() {
	uint base_x = (string.x >> 16) & 0xFFFF;
	uint base_y = (string.x >> 0 ) & 0xFFFF;
	
	uint width =  (string.y >> 24) & 0xFF;
	uint scale =  (string.y >> 16) & 0xFF;
	uint charOffset = (string.y  ) & 0xFFFF;

	uv = offsets[vertexID];
	vec2 p = vec2(base_x * res.x, base_y * res.y) * 2;
	vec2 w = vec2(uv.x * res.x * charSize.x * width, uv.y * res.y * charSize.y) * scale;
	
	uv.x *= width;

	gl_Position = vec4(p + w - 1, 0.0, 1.0);
}




#shaderstage fragment
#version 430 core

in vec2 uv;
out vec4 FragColor;

vec3 Eval3(float p) {
	vec3 p3 = fract(vec3(0.1031, 0.1030, 0.0973) * p);
	p3 += dot(p3, p3.yzx + 33.33f);
	return fract((p3.xxy + p3.yzz) * p3.zyx);
}

void main() {
	FragColor = vec4(Eval3(floor(uv.x)), 1.0);
	//FragColor = vec4(uv.x, 0.0, uv.y, 1.0);
} 