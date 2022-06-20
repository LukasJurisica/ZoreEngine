#shaderstage vertex
#version 430 core

layout(location = 0) in int vertexID;
layout(std140, binding = 0) uniform shaderData { mat4 vp_mat; mat4 ivp_mat; vec3 cp; float t; };
out vec3 pos;
out float time;
out float sunAngle;
out vec3 sunVec;

const vec2 position[4] = vec2[4](vec2(-1, 1), vec2(-1, -1), vec2(1, 1), vec2(1, -1));

void main() {
	gl_Position = vec4(position[vertexID], 1.0, 1.0);
	pos = (ivp_mat * gl_Position).xyz;
	time = t;
    sunAngle = (time + 20) / 20;
    sunVec = vec3(sin(sunAngle), -cos(sunAngle), 0.0);
}




#shaderstage fragment
#version 430 core
in vec3 pos;
in float time;
in vec3 sunVec;
in float sunAngle;
out vec4 color;

#define SKY_DENSITY_D 0.35
#define SKY_HORIZON_F 1.50
#define SKY_HORIZON_N 1.00
#define SKY_DENSITY_N 0.65
#define SKY_EXPOSURE_N 0.00
#define SKY_R 96
#define SKY_G 160
#define SKY_B 255
#define SKY_I 1.00

#define LIGHT_MER 255
#define LIGHT_MEG 160
#define LIGHT_MEB 80
#define LIGHT_MEI 1.20
#define LIGHT_DR 192
#define LIGHT_DG 216
#define LIGHT_DB 255
#define LIGHT_DI 1.40
#define LIGHT_NR 96
#define LIGHT_NG 192
#define LIGHT_NB 255
#define LIGHT_NI 1.00


vec3 n_pos;
vec3 up_vec  = vec3(0.0, 1.0, 0.0);
vec3 skyColSqrt = vec3(SKY_R, SKY_G, SKY_B) * SKY_I / 255.0;
vec3 skyCol = skyColSqrt * skyColSqrt;
vec3 fogCol = skyColSqrt * skyColSqrt;
float sunVisibility  = clamp((dot( sunVec, up_vec) + 0.2), 0.0, 1.0);
float moonVisibility = clamp((dot(-sunVec, up_vec) + 0.2), 0.0, 1.0);
float rainStrength = 0.0;

float mySunVisibility = clamp(dot(sunVec, up_vec), 0.0, 1.0);

float dfade = 1.0 - pow(1.0 - mySunVisibility, 1.5);

vec3 lightME         = vec3(LIGHT_MER, LIGHT_MEG, LIGHT_MEB) * LIGHT_MEI / 255.0;
vec3 lightDay        = vec3(LIGHT_DR,   LIGHT_DG,  LIGHT_DB) * LIGHT_DI  / 255.0;
vec3 lightNight      = vec3(LIGHT_NR,   LIGHT_NG,  LIGHT_NB) * LIGHT_NI  * 0.3 / 255.0;
vec3 lightSun        = mix(lightME, lightDay, dfade);

vec3 GetSun() {
	float sun = 1.0 - distance(sunVec, n_pos);
	float glow = sun;

	sun = clamp(sun, 0.0, 1.0);
	sun = pow(sun, 100) * 100;
	sun = clamp(sun, 0.0, 1.0);

	return vec3(1.0,0.6,0.05) * sun;
}

vec3 GetSky() {
	float VoU = clamp(dot(n_pos, up_vec), -1.0, 1.0);
    float VoL = clamp(dot(n_pos, sunVec), -1.0, 1.0);

	// Base Sky Colour (DAY)
	float gradientCurve = mix(SKY_HORIZON_F, SKY_HORIZON_N, VoL);
    float baseGradient = exp(-(1.0 - pow(1.0 - max(VoU, 0.0), gradientCurve)) / SKY_DENSITY_D);
	vec3 sky = skyCol * baseGradient / (SKY_I * SKY_I);

	// Sunrise and Sunset
	float sunMix = (VoL * 0.5 + 0.5) * pow(clamp(1.0 - VoU, 0.0, 1.0), 2.0 - sunVisibility) * pow(1.0 - mySunVisibility * 0.6, 3.0);
    float horizonMix = pow(1.0 - abs(VoU), 2.5) * 0.125 * (1.0 - mySunVisibility * 0.5);
    float lightMix = (1.0 - (1.0 - sunMix) * (1.0 - horizonMix));
    vec3 lightSky = pow(lightSun, vec3(4.0 - sunVisibility)) * baseGradient;
    lightSky = lightSky / (1.0 + lightSky * rainStrength);
    sky = mix( sqrt(sky * (1.0 - lightMix)), sqrt(lightSky), lightMix );
    //sky *= sky;

	// Night
	float nightExposure = exp2(-3.5 + SKY_EXPOSURE_N);
    float nightGradient = exp(-max(VoU, 0.0) / SKY_DENSITY_N);
    vec3 nightSky = lightNight * lightNight * nightGradient * nightExposure;
    sky = mix(nightSky, sky, smoothstep(0, 1, sunVisibility));

    return sky;
}

void main() {
	n_pos = normalize(pos);
	color = vec4(GetSky() + GetSun(), 1.0);

	color.rgb = sqrt(color.rgb);

	//color.rgb = pow(color.rgb, vec3(1.0 / 2.2));

	//color.rgb = sqrt(max(color.rgb, vec3(0.0)));
}