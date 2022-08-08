#version 450 core

out vec4 fragColor;

in vec2 texCoord__;
in vec3 normal__;

struct MDirectionLight {
	bool active;
	vec3 direction;
	vec3 diffuse;
	vec3 specular;
};

struct MPointLight {
	bool active;
	float constant;
	float linear;
	float quadratic;
	vec3 position;
	vec3 ambient;
	vec3 specular;
};

uniform MDirectionLight dirLight;
uniform MPointLight pointLights[3];

uniform sampler2D tex_diffuse;
uniform sampler2D tex_specular;
uniform sampler2D tex_normal;

vec3 CalcDirectionLight(MDirectionLight light, vec3 normal, vec3 view_dir) {
	
}

vec3 CalcPointLight(MPointLight light, vec3 normal, vec3 view_dir) {
	
}

void main() {
	fragColor = vec4(
}