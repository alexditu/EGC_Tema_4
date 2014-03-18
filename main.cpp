//-------------------------------------------------------------------------------------------------
// Descriere: fisier main
//
// Autor: student
// Data: today
//-------------------------------------------------------------------------------------------------

//incarcator de meshe
#include "lab_mesh_loader.hpp"
//geometrie: drawSolidCube, drawWireTeapot...
#include "lab_geometry.hpp"
//incarcator de shadere
#include "lab_shader_loader.hpp"
//interfata cu glut, ne ofera fereastra, input, context opengl
#include "lab_glut.hpp"
//texturi
#include "lab_texture_loader.hpp"
//time
#include <ctime>
#include "Square.h"


class Laborator : public lab::glut::WindowListener{

//variabile
private:
	glm::mat4 model_matrix;				//matrici modelare pentru cele 3 obiecte
	glm::mat4 view_matrix, projection_matrix;											//matrici 4x4 pt modelare vizualizare proiectie
	unsigned int gl_program_shader, gl_music_shader;									//id-ul de opengl al obiectului de tip program shader
	unsigned int screen_width, screen_height;

	//meshe					
	unsigned int vbo, ibo, vao, num_indices;				//containere opengl pentru vertecsi, indecsi si stare
	unsigned int girl_texture, music_texture;

	// lumina & material
	glm::vec3 light_position;
	glm::vec3 eye_position;
	unsigned int material_shininess;
	float material_kd;
	float material_ks;

	// pt camera
	float cam_x, cam_y, cam_z;

	// pentru animatie
	float time;
	int no_of_frames;
	int state, t_state;
	float time_per_state;

	// pentru notele muzicale:
	vector <Square> notes;
	unsigned int no_of_notes;
	clock_t start_clock, crt_clock;

//metode
public:
	
	//constructor .. e apelat cand e instantiata clasa
	Laborator(){
		//setari pentru desenare, clear color seteaza culoarea de clear pentru ecran (format R,G,B,A)
		glClearColor(0.5,0.5,0.5,1);
		glClearDepth(1);			//clear depth si depth test (nu le studiem momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	//sunt folosite pentru a determina obiectele cele mai apropiate de camera (la curs: algoritmul pictorului, algoritmul zbuffer)
		//incarca un shader din fisiere si gaseste locatiile matricilor relativ la programul creat
		gl_program_shader = lab::loadShader("shadere\\shader_vertex.glsl", "shadere\\shader_fragment.glsl");
		gl_music_shader = lab::loadShader("shadere\\shader_vertex_music.glsl", "shadere\\shader_fragment_music.glsl");
		
		//girl
		lab::loadObj("resurse\\girl_sleep.obj", "resurse\\girl_surprise.obj", "resurse\\girl_annoyed.obj",vao, vbo, ibo, num_indices);	
		girl_texture = lab::loadTextureBMP("resurse\\girl_texture.bmp");
		model_matrix = glm::mat4(1,0,0,0,  0,1,0,0, 0,0,1,0, 0,0,0,1);

		//music:
		music_texture = lab::loadTextureBMP("resurse\\music.bmp");
		
		//matrici de modelare si vizualizare
		// pt fata:
		cam_x = 0;
		cam_y = 8;
		cam_z = 6;
		view_matrix = glm::lookAt(glm::vec3(cam_x, cam_y, cam_z), glm::vec3(0,8,0), glm::vec3(0,1,0));

		//desenare wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//lumina & material
		eye_position = glm::vec3(0,10,50);
		light_position = glm::vec3(10,7,25);
		material_shininess = 20;
		material_kd = 0.2;
		material_ks = 0.2;

		// pentru animatie
		state = 0;
		t_state = 0;

		// initializare note muzicale:
		no_of_notes = 500;
		Square p;
		for (unsigned i = 0; i < no_of_notes; i++) {
			glm::vec3 color;
			color.x = (rand() % 100) / 100.0;
			color.y = (rand() % 100) / 100.0;
			color.z = (rand() % 100) / 100.0;
			p = Square(color);

			p.xc = (rand() % 26) - 25; // xc = [-25, -15]
			p.yc = (rand() % 26) + 15; //yc = [15, 25]
			p.v = ((rand() % 6) + 10) / 10.0;

			notes.push_back(p);
		}

		//time:
		start_clock = clock();
		time_per_state = 8;
	}

	//destructor .. e apelat cand e distrusa clasa
	~Laborator(){
		//distruge shader
		glDeleteProgram(gl_program_shader);

		//distruge obiecte
		glDeleteBuffers(1,&vbo);
		glDeleteBuffers(1,&ibo);
		glDeleteVertexArrays(1,&vao);		
		glDeleteTextures(1,&girl_texture);
		glDeleteTextures(1,&music_texture);
	}

	
	//--------------------------------------------------------------------------------------------
	//functii de cadru ---------------------------------------------------------------------------

	//functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa updatam situatia scenei ( modelam/simulam scena)
	void notifyBeginFrame(){

	}
	//functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame(){
		
		//pe tot ecranul
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//foloseste shaderul
		glUseProgram(gl_program_shader);
		
		//trimite variabile uniforme la shader
		view_matrix = glm::lookAt(glm::vec3(cam_x, cam_y, cam_z), glm::vec3(0,8,0), glm::vec3(0,1,0));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"view_matrix"),1,false,glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"model_matrix"),1,false,glm::value_ptr(model_matrix));

		//TODO		
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, girl_texture);
		glUniform1i(glGetUniformLocation(gl_program_shader, "textura1"), 1);
		glUniform1i(glGetUniformLocation(gl_program_shader, "has_alpha"), 0);

		// iluminare Phong
		glUniform3f(glGetUniformLocation(gl_program_shader, "light_position"),light_position.x, light_position.y, light_position.z);
		glUniform3f(glGetUniformLocation(gl_program_shader, "eye_position"),eye_position.x, eye_position.y, eye_position.z);
		glUniform1i(glGetUniformLocation(gl_program_shader, "material_shininess"),material_shininess);
		glUniform1f(glGetUniformLocation(gl_program_shader, "material_kd"),material_kd);
		glUniform1f(glGetUniformLocation(gl_program_shader, "material_ks"),material_ks);

		crt_clock = clock();
		time = (float)(crt_clock - start_clock) / CLOCKS_PER_SEC;

		float state_time = (float) (time - t_state * time_per_state) / time_per_state;
		 
		float gravity = -0.098;
		
		// trimitere timp la shader
		glUniform1f(glGetUniformLocation(gl_program_shader, "time"),state_time);
		glUniform1i(glGetUniformLocation(gl_program_shader, "state"),state);
		if (time - t_state * time_per_state >= time_per_state) {
			++state %= 4;
			t_state ++;
		}

		// desenare obiect
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);

		if (state == 1 || state == 2) {
			// utilizare shader pt note muzicale
			glUseProgram(gl_music_shader);
			glUniformMatrix4fv(glGetUniformLocation(gl_music_shader,"view_matrix"),1,false,glm::value_ptr(view_matrix));
			glUniformMatrix4fv(glGetUniformLocation(gl_music_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
			glUniformMatrix4fv(glGetUniformLocation(gl_music_shader,"model_matrix"),1,false,glm::value_ptr(model_matrix));

			// desenare obiect: patrat nota muzicala
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, music_texture);
			glUniform1i(glGetUniformLocation(gl_music_shader, "textura1"), 1);

			//glm::mat4 move_mat = glm::translate(model_matrix, glm::vec3(-4, 10, 0));
			glUniformMatrix4fv(glGetUniformLocation(gl_music_shader,"model_matrix"),1,false,glm::value_ptr(model_matrix));

			// trimitere var pt calc noii pozitii
			float music_time;
			if (state == 1) {
				music_time = time - t_state * time_per_state;
			} else {
				music_time = time_per_state + time - t_state * time_per_state;
			}

			for (unsigned i = 0; i < notes.size(); i++) {
				glUniform1f(glGetUniformLocation(gl_music_shader, "xc"), notes[i].xc);
				glUniform1f(glGetUniformLocation(gl_music_shader, "yc"), notes[i].yc);
				glUniform1f(glGetUniformLocation(gl_music_shader, "time"), music_time);
				glUniform1f(glGetUniformLocation(gl_music_shader, "velocity"), notes[i].v);
				glUniform1f(glGetUniformLocation(gl_music_shader, "gravity"), gravity);
		
				glBindVertexArray(notes[i].vao);
				glDrawElements(GL_TRIANGLES, notes[i].num_indices, GL_UNSIGNED_INT, 0);
			}
		}

		
	
	}
	//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame(){}
	//functei care e chemata cand se schimba dimensiunea ferestrei initiale
	void notifyReshape(int width, int height, int previos_width, int previous_height){
		//reshape
		if(height==0) height=1;
		float aspect = (float)width / (float)height;
		projection_matrix = glm::perspective(75.0f, aspect,0.1f, 10000.0f);
	}


	//--------------------------------------------------------------------------------------------
	//functii de input output --------------------------------------------------------------------
	
	//tasta apasata
	void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == 27) lab::glut::close();	//ESC inchide glut si 
		if(key_pressed == 32) {
			//SPACE reincarca shaderul si recalculeaza locatiile (offseti/pointeri)
			glDeleteProgram(gl_program_shader);
			gl_program_shader = lab::loadShader("shadere\\shader_vertex.glsl", "shadere\\shader_fragment.glsl");
		}
		if(key_pressed == 'w'){
			static bool wire =true;
			wire=!wire;
			glPolygonMode(GL_FRONT_AND_BACK, (wire?GL_LINE:GL_FILL));
		}

	}
	//tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y){	}
	//tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
		if(key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();
	}
	//tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y){}
	//drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y){ }
	//am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y){ }
	//am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y){ }
	//scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){ }

};

int main(){
	//initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("lab shadere 5 - texturi"),800,600,100,100,true);
	lab::glut::ContextInfo context(3,3,false);
	lab::glut::FramebufferInfo framebuffer(true,true,true,true);
	lab::glut::init(window,context, framebuffer);

	//initializeaza GLEW (ne incarca functiile openGL, altfel ar trebui sa facem asta manual!)
	glewExperimental = true;
	glewInit();
	std::cout<<"GLEW:initializare"<<std::endl;

	//creem clasa noastra si o punem sa asculte evenimentele de la GLUT
	//DUPA GLEW!!! ca sa avem functiile de OpenGL incarcate inainte sa ii fie apelat constructorul (care creeaza obiecte OpenGL)
	Laborator mylab;
	lab::glut::setListener(&mylab);

	//taste
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tSPACE ... reincarca shadere"<<std::endl<<"\tw ... toggle wireframe"<<std::endl;

	//run
	lab::glut::run();

	return 0;
}