#version 330

layout(location = 0) in vec3 in_position;	
layout(location = 1) in vec3 in_color;	
layout(location = 2) in vec2 in_texcoord;

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform float xc;
uniform float yc;
uniform float time;
uniform float velocity;
uniform float gravity;

out vec2 texcoord;
out vec3 vertex_to_fragment_color;

void main(){

	texcoord = in_texcoord;

	//gl_Position = projection_matrix*view_matrix*model_matrix*vec4(in_position,1);

	float t = time;

	vec4 pFinal = vec4(xc, yc, 0, 1) +

                  vec4(velocity, velocity, 0, 1) * vec4(t,t,0,1) +

                  0.5 * vec4(0,gravity,0,1) * vec4(t,t,0,1) * vec4(t,t,0,1);

	vec4 new_pos = pFinal + vec4(in_position,1);

	gl_Position = projection_matrix*view_matrix*model_matrix*new_pos;


	vertex_to_fragment_color = in_color;
}
