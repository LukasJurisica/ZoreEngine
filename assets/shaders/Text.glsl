#shaderstage vertex
#version 430 core

// Uniform Data
layout(std140, binding = 0) uniform dynamicShaderData { mat4 vp_mat; mat4 inv_vp_mat; vec3 cam_pos; float time; };
layout(std140, binding = 1) uniform staticShaderData { vec2 inv_res; };

// Vertex Data
layout(location = 0) in uvec2 string;
out vec2 uv;
flat out float screenPxRange;
flat out uint charOffset;

// Constants
const int pxRange = 2;
const float baseScale = 2.5;
const vec2 charSize = vec2(20, 32) * baseScale;

void main() {
	uint base_x = string.x & 0xFFFF;
	uint base_y = (string.x >> 16) & 0xFFFF;
	
	uint scale = (string.y >> 24) & 0xFF;
	uint width = (string.y >> 16) & 0xFF;
	charOffset = string.y & 0xFFFF;
 
	uv = vec2(gl_VertexID >> 1, 1 - (gl_VertexID & 1));
	vec2 p = vec2(base_x * inv_res.x, base_y * inv_res.y) * 2;
	vec2 s = vec2(uv.x * inv_res.x * charSize.x * width, uv.y * inv_res.y * charSize.y) * scale * 2;
	screenPxRange = baseScale * pxRange * scale;
	uv.x *= width;

	// Italisization
	//if (charOffset > 0)
	//	s.x += (uv.y * 2 - 1) * res.x * 15;

	gl_Position = vec4(p + s - 1, 0.0, 1.0);
}




#shaderstage fragment
#version 430 core

// Uniform Data
layout(std140, binding = 1) uniform staticShaderData { vec2 inv_res; };
layout(binding = 2) uniform sampler2DArray glyphs;
layout(std430, binding = 0) buffer charBuffer { uint chars[]; };

// Fragment Data
in vec2 uv;
flat in float screenPxRange;
flat in uint charOffset;
out vec4 FragColor;

// Constants
//#define BACKGROUND_COLOUR vec3(0.0, 0.0, 1.0)

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