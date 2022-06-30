#shaderstage vertex
#version 430 core

layout(location = 0) in int vertexID;
layout(location = 1) in uvec2 string;
layout (std140, binding = 0) uniform shaderData { mat4 vp_mat; mat4 ivp_mat; vec3 cameraPos; float time; vec2 res; };
out vec2 uv;
flat out float screenPxRange;
flat out uint charOffset;

const int pxRange = 2;
const float baseScale = 2.5;
const vec2 charSize = vec2(20, 32) * baseScale;

void main() {
	uint base_x = string.x & 0xFFFF;
	uint base_y = (string.x >> 16) & 0xFFFF;
	
	uint scale = (string.y >> 24) & 0xFF;
	uint width = (string.y >> 16) & 0xFF;
	charOffset = string.y & 0xFFFF;
 
	uv = vec2(vertexID >> 1, 1 - (vertexID & 1));
	vec2 p = vec2(base_x * res.x, base_y * res.y) * 2;
	vec2 s = vec2(uv.x * res.x * charSize.x * width, uv.y * res.y * charSize.y) * scale * 2;
	screenPxRange = baseScale * pxRange * scale;
	uv.x *= width;

	// Italisization
	//if (charOffset > 0)
	//	s.x += (uv.y * 2 - 1) * res.x * 15;

	gl_Position = vec4(p + s - 1, 0.0, 1.0);
}




#shaderstage fragment
#version 430 core

//#define BACKGROUND_COLOUR vec3(0.0, 0.0, 1.0)

in vec2 uv;
flat in float screenPxRange;
flat in uint charOffset;
layout(binding = 2) uniform sampler2DArray glyphs;
layout(std430, binding = 0) buffer charBuffer { uint chars[]; };
out vec4 FragColor;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
	uint index = int(floor(uv.x)) + charOffset;
	uint val = (chars[index>>2] >> ((index % 4) * 8)) & 0xFF;
	vec3 msd = texture(glyphs, vec3(fract(uv.x), uv.y, val - 32)).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange * (sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

	#ifdef BACKGROUND_COLOUR
	vec3 color = mix(vec3(1.0, 0.0, 0.0), BACKGROUND_COLOUR, opacity);
	FragColor = vec4(color, 1.0);
	#else
	FragColor = vec4(vec3(1.0), opacity);
	#endif
} 