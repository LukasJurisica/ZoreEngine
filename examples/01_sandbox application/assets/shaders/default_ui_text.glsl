#shaderstage vertex

uniform vec4 camera;

out vec2 uv;

void main() {
	uv = vec2(gl_VertexID >> 1, gl_VertexID & 1);
	gl_Position = vec4((uv * 500 - camera.xy) * camera.zw, 0.0, 1.0);
}




#shaderstage fragment

layout(binding = 1) uniform sampler2DArray glyphs;

in vec2 uv;

out vec4 FragColour;

void main() {
	vec4 tex = texture(glyphs, vec3(uv.x, 1 - uv.y, 1));
	FragColour = vec4(tex.rgb, 1.0);
} 