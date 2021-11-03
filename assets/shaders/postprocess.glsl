#shaderstage vertex
#version 430 core

// Vertex Inputs
layout(location = 0) in int vertexID;

// Vertex Shader to Fragment Shader Passthrough
out vec2 uv;

// Constant Data
const vec2 offsets[4] = vec2[4]( vec2(-1, 1), vec2(-1,-1), vec2( 1, 1), vec2( 1,-1) );



void main() {
	vec2 position = offsets[vertexID];
	uv = (position + 1.0f) * 0.5f;
	gl_Position = vec4(position, 0.0f, 1.0f);
}





#shaderstage fragment
#version 430 core

// Vertex Shader to Fragment Shader Passthrough
in vec2 uv;

// Uniform Variables and Textures
layout(binding = 0) uniform sampler2D tex;
uniform vec2 resolution;

// Fragment Shader Outputs
out vec4 FragColor;

// Constant Data
#define FXAA_REDUCE_MIN  (1.0f / 128.0f)
#define FXAA_REDUCE_MUL  (1.0f / 8.0f)
#define FXAA_SPAN_MAX     8.0f



float r2l(vec3 rgb) {
	return dot(rgb, vec3(0.299, 0.587, 0.114));
}

void main() {
    vec4 rgbM = texture(tex, uv);

    // Returning here skips anti aliasing
    FragColor = rgbM;
    return;

    float lumaM  = r2l(rgbM.rgb);
    float lumaNW = r2l(textureOffset(tex, uv, ivec2(-1, 1)).rgb);
    float lumaNE = r2l(textureOffset(tex, uv, ivec2( 1, 1)).rgb);
    float lumaSW = r2l(textureOffset(tex, uv, ivec2(-1,-1)).rgb);
    float lumaSE = r2l(textureOffset(tex, uv, ivec2( 1,-1)).rgb);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    vec2 dir = vec2(((lumaSW + lumaSE) - (lumaNW + lumaNE)), ((lumaNW + lumaSW) - (lumaNE + lumaSE)));
    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = min(vec2(FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX), dir * rcpDirMin)) * resolution;

    vec3 rgbA = 0.5 * (texture(tex, uv + dir * (1.0/3.0 - 0.5)).xyz + texture(tex, uv + dir * (2.0/3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (texture(tex, uv + dir * (0.0/3.0 - 0.5)).xyz + texture(tex, uv + dir * (3.0/3.0 - 0.5)).xyz);
    
    float lumaB = r2l(rgbB);
    if ((lumaB < lumaMin) || (lumaB > lumaMax))
        FragColor = vec4(rgbA, 1.0);
    else
        FragColor = vec4(rgbB, 1.0);
} 