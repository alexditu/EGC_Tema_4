#version 330

layout(location = 0) in vec3 in_position1;
layout(location = 1) in vec3 in_normal1;			
layout(location = 2) in vec2 in_texcoord1;

layout(location = 3) in vec3 in_position2;
layout(location = 4) in vec3 in_normal2;			
layout(location = 5) in vec2 in_texcoord2;

layout(location = 6) in vec3 in_position3;
layout(location = 7) in vec3 in_normal3;			
layout(location = 8) in vec2 in_texcoord3;

uniform mat4 model_matrix, view_matrix, projection_matrix;

out vec2 texcoord;

// pentru iluminare Phong
out vec3 world_pos;
out vec3 world_normal;

// pentru animatie
uniform float time;
uniform int state;

void main(){

	texcoord = in_texcoord1;

	//gl_Position = projection_matrix*view_matrix*model_matrix*vec4(in_position3,1);

	//world_pos = (model_matrix * vec4(in_position3,1)).xyz;
	//world_normal = normalize( mat3(model_matrix) * in_normal3);

	vec3 pos;
	vec3 norm;
	if (state == 0) {
		pos = in_position1;
		norm = in_normal1;
	} else {
		if (state == 1) {
			pos = in_position1 * (1 - time) + in_position2 * time;
			norm = in_normal1 * (1 - time) + in_normal2 * time;
		} else {
			if (state == 2) {
				pos = in_position2 * (1 - time) + in_position3 * time;
				norm = in_normal2 * (1 - time) + in_normal3 * time;
			} else {
				pos = in_position3 * (1 - time) + in_position1 * time;
				norm = in_normal3 * (1 - time) + in_normal1 * time;
			}
		}
	}

	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(pos,1);

	world_pos = (model_matrix * vec4(pos,1)).xyz;
	world_normal = normalize( mat3(model_matrix) * norm);
}
