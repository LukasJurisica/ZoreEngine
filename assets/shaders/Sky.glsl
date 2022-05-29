#shaderstage vertex
#version 430 core

layout(location = 0) in int vertexID;
layout(std140, binding = 0) uniform shaderData { mat4 vp_mat; mat4 V; mat4 P; vec3 cp; float t; };
out vec3 pos;
out float time;

const vec2 position[4] = vec2[4](vec2(-1, 1), vec2(-1,-1), vec2( 1, 1), vec2( 1,-1));

void main() {
	gl_Position = vec4(position[vertexID], 1.0, 1.0);
	pos = transpose(mat3(V)) * (inverse(P) * gl_Position).xyz;
	time = t;
}




#shaderstage fragment
#version 430 core
in vec3 pos;
in float time;
out vec4 color;

vec3 n_pos;

vec3 getSun() {
	float angle = time / 10;
	
	float sun = 1.0 - distance(vec3(sin(angle), -cos(angle), 0.0), n_pos);
	float glow = sun;

	sun = clamp(sun, 0.0, 1.0);
	sun = pow(sun, 100) * 100;
	sun = clamp(sun, 0.0, 1.0);

	glow = pow(glow,6.0) * 1.0;
    glow = clamp(glow,0.0,1.0);

	sun += glow;

	return vec3(1.0,0.6,0.05) * sun;
}

vec3 getSky() {
	float mu = dot(n_pos, vec3(0.0, 1.0, 0.0));
	mu = mu * 0.5 + 0.5;
	return mix(vec3(0.2, 0.1, 0.2), vec3(0.2, 0.4, 0.8), mu);

	//float atmosphere = 1 - dot(n_pos, vec3(0.0, 1.0, 0.0));
	//vec3 skyColor = vec3(0.2, 0.4, 0.8);
    
	//float scatter = pow(time / 10, 1.0 / 15.0);
	//scatter = 1.0 - clamp(scatter,0.8,1.0);
    
	//vec3 scatterColor = mix(vec3(1.0),vec3(1.0,0.3,0.0) * 1.5,scatter);
	//return mix(skyColor, vec3(scatterColor), atmosphere / 1.3);
}

void main() {
	n_pos = normalize(pos);
	color = vec4(getSun() + getSky(), 1.0);
}