#shaderstage vertex

uniform vec4 camera;

layout(location = 0) in uvec4 quad;

out flat vec4 colour;

void main() {
	vec2 pos = vec2((quad[0] >> 16) & 0xffff, (quad[0] >> 0) & 0xffff);
	vec2 size = vec2((quad[1] >> 16) & 0xffff, (quad[1] >> 0) & 0xffff);
	uint depth = quad[2];

	uint r = (quad[3] >> 24) & 0xff;
	uint g = (quad[3] >> 16) & 0xff;
	uint b = (quad[3] >> 8 ) & 0xff;
	uint a = (quad[3] >> 0 ) & 0xff;
	colour = vec4(r, g, b, a) / 255.0;
	
	vec2 uv = vec2(gl_VertexID >> 1, gl_VertexID & 1);
	pos = ((uv * size) + pos - camera.xy) * camera.zw;
	gl_Position = vec4(pos, -(float(depth) / 16.0) + 1.0, 1.0);
}




#shaderstage fragment

in vec4 colour;

out vec4 FragColor;

void main() {
	FragColor = colour;
} 