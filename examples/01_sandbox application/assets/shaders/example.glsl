#shaderstage vertex

uniform vec4 camera;

void main() {
	vec2 uv = vec2(gl_VertexID >> 1, gl_VertexID & 1);
	vec2 pos = (uv - 0.5 - camera.xy) * camera.zw;
	gl_Position = vec4(pos, 0.f, 1.f);
}




#shaderstage fragment

out vec4 FragColor;

void main() {
	FragColor = vec4(0.0, 0.0, 1.0, 1.0);
} 