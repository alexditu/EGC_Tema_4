#version 330
layout(location = 0) out vec4 out_color;

uniform sampler2D textura1;
uniform sampler2D textura2;
uniform int has_alpha;

in vec2 texcoord;

// pentru iluminare Phong
uniform vec3 light_position;
uniform vec3 eye_position;
uniform int material_shininess;
uniform float material_kd;
uniform float material_ks;

in vec3 world_pos;
in vec3 world_normal;

void main(){

	vec3 tex1 = texture(textura1, texcoord).xyz;
	vec3 tex2 = texture(textura2, texcoord).xyz;

	if(has_alpha>0.5) if((tex2.r<0.1) && (tex2.g<0.1) && (tex2.b<0.1)) discard;

	

	// iluminare Phong
	float light=0;

	//TODO

	vec3 L = normalize ( light_position - world_pos);
	vec3 V = normalize ( eye_position - world_pos);
	vec3 H = normalize(L+V);

	float difuza = material_kd * max (dot(world_normal,L), 0);

	float primesteLumina;
	if (dot(world_normal,L) > 0) {
		primesteLumina = 1;
	} else {
		primesteLumina = 0;
	}

	float speculara = material_ks * primesteLumina * pow(max(dot(world_normal,H), 0) ,material_shininess);
	
	float ambientala = 0.2 * material_kd;
	light = difuza + speculara + ambientala;

	float lightx, lighty, lightz;
	lightx = speculara + ambientala + (tex1.x * max (dot(world_normal,L), 0));
	lighty = speculara + ambientala + (tex1.y * max (dot(world_normal,L), 0));
	lightz = speculara + ambientala + (tex1.z * max (dot(world_normal,L), 0));

	out_color = vec4(lightx,lighty, lightz,1);
}