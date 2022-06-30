#shaderstage vertex
#version 430 core

layout(location = 0) in int vertexID;
layout (std140, binding = 0) uniform shaderData { mat4 vp_mat; mat4 ivp_mat; vec3 cameraPos; float time; vec2 res; };
out vec2 uv;
flat out vec2 resolution;

void main() {
	uv = vec2(vertexID >> 1, 1 - (vertexID & 1));
	vec2 position = uv * 2 - 1;
	resolution = res;
	gl_Position = vec4(position, 0.0, 1.0);
}





#shaderstage fragment
#version 430 core

in vec2 uv;
flat in vec2 resolution;
layout(binding = 0) uniform sampler2DArray screen;
out vec4 fragColor;

#define ENABLE_FXAA
#define EDGE_THRESHOLD_MIN 0.0312
#define EDGE_THRESHOLD_MAX 0.125
#define ITERATIONS 12
#define SUBPIXEL_QUALITY 0.75
const float quality[12] = float[12] (1.0, 1.0, 1.0, 1.0, 1.0, 1.5, 2.0, 2.0, 2.0, 2.0, 4.0, 8.0);

float Luminance(vec3 rgb) {
	return dot(rgb, vec3(0.299, 0.587, 0.114));
	//return dot(rgb, vec3(0.2126729, 0.7151522, 0.0721750));
}

vec3 Fxaa() {
	vec3 colorCenter = texture(screen, vec3(uv, 0)).rgb;
	
	// Luma at the current fragment
	float lumaCenter = Luminance(colorCenter);

	// Luma at the four direct neighbours of the current fragment.
	float lumaDown 	= Luminance(textureLodOffset(screen, vec3(uv, 0), 0.0, ivec2( 0,-1)).rgb);
	float lumaUp 	= Luminance(textureLodOffset(screen, vec3(uv, 0), 0.0, ivec2( 0, 1)).rgb);
	float lumaLeft 	= Luminance(textureLodOffset(screen, vec3(uv, 0), 0.0, ivec2(-1, 0)).rgb);
	float lumaRight = Luminance(textureLodOffset(screen, vec3(uv, 0), 0.0, ivec2( 1, 0)).rgb);

	// Find the maximum, minimum, and delta luma around the current fragment.
	float lumaMin = min(lumaCenter, min(min(lumaDown, lumaUp), min(lumaLeft, lumaRight)));
	float lumaMax = max(lumaCenter, max(max(lumaDown, lumaUp), max(lumaLeft, lumaRight)));
	float lumaRange = lumaMax - lumaMin;

	// If the luma variation is lower that a threshold (or if we are in a really dark area), we are not on an edge, don't perform any AA.
	if (lumaRange < max(EDGE_THRESHOLD_MIN, lumaMax * EDGE_THRESHOLD_MAX))
		return colorCenter;

	// Query the 4 remaining corners lumas.
	float lumaDownLeft 	= Luminance(textureLodOffset(screen, vec3(uv, 0), 0.0, ivec2(-1,-1)).rgb);
	float lumaUpRight 	= Luminance(textureLodOffset(screen, vec3(uv, 0), 0.0, ivec2( 1, 1)).rgb);
	float lumaUpLeft 	= Luminance(textureLodOffset(screen, vec3(uv, 0), 0.0, ivec2(-1, 1)).rgb);
	float lumaDownRight = Luminance(textureLodOffset(screen, vec3(uv, 0), 0.0, ivec2( 1,-1)).rgb);

	// Combine the four edges lumas (using intermediary variables for future computations with the same values).
	float lumaDownUp = lumaDown + lumaUp;
	float lumaLeftRight = lumaLeft + lumaRight;

	// Same for corners
	float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
	float lumaDownCorners = lumaDownLeft + lumaDownRight;
	float lumaRightCorners = lumaDownRight + lumaUpRight;
	float lumaUpCorners = lumaUpRight + lumaUpLeft;

	// Compute an estimation of the gradient along the horizontal and vertical axis.
	float edgeHorizontal = abs(-2.0 * lumaLeft + lumaLeftCorners) + abs(-2.0 * lumaCenter + lumaDownUp )   * 2.0 + abs(-2.0 * lumaRight + lumaRightCorners);
	float edgeVertical   = abs(-2.0 * lumaUp + lumaUpCorners)     + abs(-2.0 * lumaCenter + lumaLeftRight) * 2.0 + abs(-2.0 * lumaDown + lumaDownCorners);

	// Is the local edge horizontal or vertical ?
	bool isHorizontal = (edgeHorizontal >= edgeVertical);

	// Choose the step size (one pixel) accordingly.
	float stepLength = isHorizontal ? resolution.y : resolution.x;

	// Select the two neighboring texels lumas in the opposite direction to the local edge.
	float luma1 = isHorizontal ? lumaDown : lumaLeft;
	float luma2 = isHorizontal ? lumaUp : lumaRight;
	// Compute gradients in this direction.
	float gradient1 = luma1 - lumaCenter;
	float gradient2 = luma2 - lumaCenter;

	// Which direction is the steepest ?
	bool is1Steepest = abs(gradient1) >= abs(gradient2);

	// Gradient in the corresponding direction, normalized.
	float gradientScaled = 0.25*max(abs(gradient1),abs(gradient2));

	// Average luma in the correct direction.
	float lumaLocalAverage = 0.0;
	if(is1Steepest){
		// Switch the direction
		stepLength = - stepLength;
		lumaLocalAverage = 0.5*(luma1 + lumaCenter);
	} else {
		lumaLocalAverage = 0.5*(luma2 + lumaCenter);
	}

	// Shift UV in the correct direction by half a pixel.
	vec2 currentUv = uv;
	if(isHorizontal)
		currentUv.y += stepLength * 0.5;
	else
		currentUv.x += stepLength * 0.5;

	// Compute offset (for each iteration step) in the right direction.
	vec2 offset = isHorizontal ? vec2(resolution.x, 0.0) : vec2(0.0, resolution.y);
	// Compute UVs to explore on each side of the edge, orthogonally. The QUALITY allows us to step faster.
	vec2 uv1 = currentUv - offset * quality[0];
	vec2 uv2 = currentUv + offset * quality[0];

	// Read the lumas at both current extremities of the exploration segment, and compute the delta wrt to the local average luma.
	float lumaEnd1 = Luminance(textureLod(screen, vec3(uv1, 0), 0.0).rgb);
	float lumaEnd2 = Luminance(textureLod(screen, vec3(uv2, 0), 0.0).rgb);
	lumaEnd1 -= lumaLocalAverage;
	lumaEnd2 -= lumaLocalAverage;

	// If the luma deltas at the current extremities is larger than the local gradient, we have reached the side of the edge.
	bool reached1 = abs(lumaEnd1) >= gradientScaled;
	bool reached2 = abs(lumaEnd2) >= gradientScaled;
	bool reachedBoth = reached1 && reached2;

	// If the side is not reached, we continue to explore in this direction.
	if(!reached1)
		uv1 -= offset * quality[1];
	if(!reached2)
		uv2 += offset * quality[1];

	// If both sides have not been reached, continue to explore.
	if(!reachedBoth){
	
		for(int i = 2; i < ITERATIONS; i++){
			// If needed, read luma in 1st direction, compute delta.
			if(!reached1){
				lumaEnd1 = Luminance(textureLod(screen, vec3(uv1, 0), 0.0).rgb);
				lumaEnd1 = lumaEnd1 - lumaLocalAverage;
			}
			// If needed, read luma in opposite direction, compute delta.
			if(!reached2){
				lumaEnd2 = Luminance(textureLod(screen, vec3(uv2, 0), 0.0).rgb);
				lumaEnd2 = lumaEnd2 - lumaLocalAverage;
			}
			// If the luma deltas at the current extremities is larger than the local gradient, we have reached the side of the edge.
			reached1 = abs(lumaEnd1) >= gradientScaled;
			reached2 = abs(lumaEnd2) >= gradientScaled;
			reachedBoth = reached1 && reached2;
		
			// If the side is not reached, we continue to explore in this direction, with a variable quality.
			if(!reached1)
				uv1 -= offset * quality[i];
			if(!reached2)
				uv2 += offset * quality[i];
		
			// If both sides have been reached, stop the exploration.
			if(reachedBoth)
				break;
		}
	
	}

	// Compute the distances to each side edge of the edge (!).
	float distance1 = isHorizontal ? (uv.x - uv1.x) : (uv.y - uv1.y);
	float distance2 = isHorizontal ? (uv2.x - uv.x) : (uv2.y - uv.y);

	// In which direction is the side of the edge closer ?
	bool isDirection1 = distance1 < distance2;
	float distanceFinal = min(distance1, distance2);

	// Thickness of the edge.
	float edgeThickness = (distance1 + distance2);

	// Is the luma at center smaller than the local average ?
	bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;

	// If the luma at center is smaller than at its neighbour, the delta luma at each end should be positive (same variation).
	bool correctVariation1 = (lumaEnd1 < 0.0) != isLumaCenterSmaller;
	bool correctVariation2 = (lumaEnd2 < 0.0) != isLumaCenterSmaller;

	// Only keep the result in the direction of the closer side of the edge.
	bool correctVariation = isDirection1 ? correctVariation1 : correctVariation2;

	// UV offset: read in the direction of the closest side of the edge.
	float pixelOffset = - distanceFinal / edgeThickness + 0.5;

	// If the luma variation is incorrect, do not offset.
	float finalOffset = correctVariation ? pixelOffset : 0.0;

	// Sub-pixel shifting
	// Full weighted average of the luma over the 3x3 neighborhood.
	float lumaAverage = (1.0/12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);
	// Ratio of the delta between the global average and the center luma, over the luma range in the 3x3 neighborhood.
	float subPixelOffset1 = clamp(abs(lumaAverage - lumaCenter)/lumaRange,0.0,1.0);
	float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;
	// Compute a sub-pixel offset based on this delta.
	float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;

	// Pick the biggest of the two offsets.
	finalOffset = max(finalOffset,subPixelOffsetFinal);

	// Compute the final UV coordinates.
	vec2 finalUv = uv;
	if(isHorizontal)
		finalUv.y += finalOffset * stepLength;
	else
		finalUv.x += finalOffset * stepLength;

	// Read the color at the new UV coordinates, and use it.
	return textureLod(screen, vec3(finalUv, 0), 0.0).rgb;
}

void main() {

	#ifdef ENABLE_FXAA
		fragColor = vec4(Fxaa(), 1.0);
		//fragColor = vec4(uv.x, 0.0, uv.y, 1.0);
	#else
		fragColor = vec4(texture(screen, vec3(uv, 0)).rgb, 1.0);
	#endif

	//fragColor = vec4(mix(rgbM.rgb, vec3(0.8), rgbM.a), 1.f); // FOG

    vec2 ssc = uv * 2 - 1;
    ssc.y *= resolution.x / resolution.y;
    if (sqrt(ssc.x * ssc.x + ssc.y * ssc.y) < 0.002)
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
} 