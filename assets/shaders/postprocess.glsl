#shaderstage vertex
#version 430 core

layout(location = 0) in int vertexID;
out vec2 uv;

const vec2 offsets[4] = vec2[4]( vec2(-1.0, 1.0), vec2(-1.0,-1.0), vec2( 1.0, 1.0), vec2( 1.0,-1.0) );

void main() {
	vec2 position = offsets[vertexID];
	uv = (position + 1.0) * 0.5;
	gl_Position = vec4(position, 0.0, 1.0);
}





#shaderstage fragment
#version 430 core

in vec2 uv;
layout(binding = 0) uniform sampler2DArray screen;
uniform vec2 resolution;
out vec4 FragColor;

#define FXAA_REDUCE_MIN  (1.0 / 128.0)
#define FXAA_REDUCE_MUL  (1.0 / 8.0)
#define FXAA_SPAN_MAX     8.0

float r2l(vec3 rgb) {
	return dot(rgb, vec3(0.299, 0.587, 0.114));
}

void main() {
    vec4 rgbM = texture(screen, vec3(uv, 0));


    vec2 ssc = uv * 2 - 1;
    ssc.y *= resolution.x / resolution.y;


    if (sqrt(ssc.x * ssc.x + ssc.y * ssc.y) < 0.002)
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    else {
        FragColor = rgbM;
        //FragColor = vec4(mix(rgbM.rgb, vec3(0.8), rgbM.a), 1.f);
    }

    // Returning here skips anti aliasing
    return;

    float lumaM  = r2l(rgbM.rgb);
    float lumaNW = r2l(textureOffset(screen, vec3(uv, 0), ivec2(-1, 1)).rgb);
    float lumaNE = r2l(textureOffset(screen, vec3(uv, 0), ivec2( 1, 1)).rgb);
    float lumaSW = r2l(textureOffset(screen, vec3(uv, 0), ivec2(-1,-1)).rgb);
    float lumaSE = r2l(textureOffset(screen, vec3(uv, 0), ivec2( 1,-1)).rgb);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    vec2 dir = vec2(((lumaSW + lumaSE) - (lumaNW + lumaNE)), ((lumaNW + lumaSW) - (lumaNE + lumaSE)));
    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = min(vec2(FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX), dir * rcpDirMin)) * resolution;

    vec3 rgbA = 0.5 * (texture(screen, vec3(uv + dir * (1.0/3.0 - 0.5), 0)).xyz + texture(screen, vec3(uv + dir * (2.0/3.0 - 0.5), 0)).xyz);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (texture(screen, vec3(uv + dir * (0.0/3.0 - 0.5), 0)).xyz + texture(screen, vec3(uv + dir * (3.0/3.0 - 0.5), 0)).xyz);
    
    float lumaB = r2l(rgbB);
    if ((lumaB < lumaMin) || (lumaB > lumaMax))
        FragColor = vec4(rgbA, 1.f);
    else
        FragColor = vec4(rgbB, 1.f);
} 