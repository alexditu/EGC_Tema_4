#include "Square.h"



Square::Square(void) {
	color.x = color.y = color.z = 0;
	createSquare();
}
Square::~Square(void)
{
	
}



Square::Square(glm::vec3 c) {
	color.x = c.x;
	color.y = c.y;
	color.z = c.z;
	createSquare();
}

void Square::createSquare() {
	psize = 1;
	VertexFormat v;
	v.position_x = -psize;
	v.position_y = psize;
	v.texcoord_x = 0;
	v.texcoord_y = 1;
	
	v.position_z = 0;

	v.normal_x = color.x;
	v.normal_y = color.y;
	v.normal_z = color.z;
	vertices.push_back(v);

	v.position_x = psize;
	v.position_y = psize;
	v.position_z = 0;
	v.texcoord_x = 1;
	v.texcoord_y = 1;
	vertices.push_back(v);

	v.position_x = psize;
	v.position_y = -psize;
	v.position_z = 0;
	v.texcoord_x = 1;
	v.texcoord_y = 0;
	vertices.push_back(v);

	v.position_x = -psize;
	v.position_y = -psize;
	v.position_z = 0;
	v.texcoord_x = 0;
	v.texcoord_y = 0;
	vertices.push_back(v);
	float cx = 0, cy = 0;
	for (int i = 0; i < vertices.size(); i++) {
		cx += vertices[i].position_x;
		cy += vertices[i].position_y;
	}

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);

	//vertex array object -> un obiect ce reprezinta un container pentru starea de desenare
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//vertex buffer object -> un obiect in care tinem vertecsii
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

	//index buffer object -> un obiect in care tinem indecsii
	glGenBuffers(1,&ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//legatura intre atributele vertecsilor si pipeline, datele noastre sunt INTERLEAVED.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)0);						//trimite pozitii pe pipe 0
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(VertexFormat),(void*)(2*sizeof(float)*3));	//trimite texcoords pe pipe 2

	num_indices = indices.size();

}
