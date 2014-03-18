#pragma once
#include "dependente\glew\glew.h"
#include "dependente\glm\glm.hpp"
#include <vector>
using namespace std;
class Square
{
	struct VertexFormat{
		float position_x, position_y, position_z;				//pozitia unui vertex (x,y,z)	
		float normal_x, normal_y, normal_z;						//vom invata ulterior, nu este folosit in acest lab
		float texcoord_x, texcoord_y;							//vom invata ulterior, nu este folosit in acest lab
		VertexFormat(){
			position_x = position_y =position_z=0;
			normal_x = normal_y = normal_z=0;
			texcoord_x = texcoord_y=0;
		}
		VertexFormat(float px, float py, float pz ){
			position_x =px;		position_y =py;		position_z =pz;
			normal_x =normal_y= normal_z =0;
			texcoord_x=	texcoord_y=0;
		}
	};

public:
	unsigned int vao, vbo, ibo, num_indices;
	vector<VertexFormat> vertices;
	vector<unsigned int> indices;
	float psize;
	float xc, yc, v, g;
	glm::vec3 color;

public:

	Square(void);
	~Square(void);
	Square(glm::vec3 c);

	void createSquare();

};

