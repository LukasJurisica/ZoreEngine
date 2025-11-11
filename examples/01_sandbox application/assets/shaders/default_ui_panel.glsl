#shaderstage vertex

layout(set = 0, binding = 0) uniform camera_buffer {
    vec4 camera;
};

layout(location=0) in ivec4 quad;

layout(location=0) out flat vec4 colour;

void main() {
	vec2 pos = vec2(quad[0] >> 16, quad[0] & 0xffff);
	vec2 size = vec2(quad[1] >> 16, quad[1] & 0xffff);
	uint hovered = quad[3] & 1;
	uint pressed = (quad[3] >> 1) & 1;
	uint depth = quad[3] >> 16;

	uint r = (quad[2] >> 24) & 0xff;
	uint g = (quad[2] >> 16) & 0xff;
	uint b = (quad[2] >> 8 ) & 0xff;
	uint a = (quad[2] >> 0 ) & 0xff;
	
	if (pressed == 1)
		colour = vec4(0.0, 0.0, 1.0, 1.0);
	else if (hovered == 1)
		colour = vec4(0.0, 1.0, 0.0, 1.0);
	else
		colour = vec4(r, g, b, a) / 255.0;
		
	vec2 uv = vec2(gl_VertexID >> 1, gl_VertexID & 1);
	pos = ((uv * size) + pos - camera.xy) * camera.zw;
	gl_Position = vec4(pos, -(float(depth) / 16.0) + 0.999, 1.0);
}




#shaderstage fragment

layout(location=0) in vec4 colour;

layout(location=0) out vec4 FragColor;

void main() {
	FragColor = colour;
} 