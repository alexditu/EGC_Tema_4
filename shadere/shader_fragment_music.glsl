#version 330
layout(location = 0) out vec4 out_color;

uniform sampler2D textura1;
uniform sampler2D textura2;
uniform int has_alpha;

in vec2 texcoord;
in vec3 vertex_to_fragment_color;

void main(){

	vec3 tex1 = texture(textura1, texcoord).xyz;
	vec3 tex2 = texture(textura2, texcoord).xyz;

	//if(has_alpha>0.5) if((tex2.r<0.1) && (tex2.g<0.1) && (tex2.b<0.1)) discard;

	if (tex1.x >= 0.1 && tex1.y >= 0.1) discard;

	out_color = vec4(tex1, 1) + vec4(vertex_to_fragment_color, 0);
}