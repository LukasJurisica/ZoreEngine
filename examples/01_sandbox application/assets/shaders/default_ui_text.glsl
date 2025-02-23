#shaderstage vertex

uniform vec4 camera;

layout(location = 0) in ivec4 quad;

out flat vec4 colour;
out flat uint glyph;
out vec2 uv;

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
	colour = vec4(r, g, b, a) / 255.0;
	glyph = (quad[3] >> 8) & 0xFF;
	
	uv = vec2(gl_VertexID >> 1, gl_VertexID & 1);
	pos = ((uv * size) + pos - camera.xy) * camera.zw;
	gl_Position = vec4(pos, -(float(depth) / 16.0) + 0.999, 1.0);
}




#shaderstage fragment

layout(binding = 1) uniform usampler2DArray glyphs;

in flat vec4 colour;
in flat uint glyph;
in vec2 uv;

out vec4 FragColour;

void main() {
	vec4 tex = texture(glyphs, vec3(uv.x, uv.y, glyph));
	if (tex.r == 0.0)
		discard;
	vec3 c = colour.rgb * (tex.r / 3.0);
	FragColour = vec4(c, 1.0);
	
	//const vec3 colours[3] = { vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0) };
	//FragColour = vec4(colours[int(tex.r) - 1], 1.0);
} 