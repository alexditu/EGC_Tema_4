EGC - Tema 4
	Sleeping Beauty
	Ditu Alexandru Mihai 333CA
	
Cuprins
1. Cerinta
2. Utilizare
3. Implementare
4. Testare
5. Probleme aparute
6. Continutul Arhivei
7. Functionalitati

1. Cerinta
	Sa se implementeze o cursa 3D.

2. Utilizare
			
3. Implementare
	IDE:	Visual Studio 2012
	SO:		Windows 7 Professional, 64bit
	Framework-ul folosit este lab_gl_5.
	Mentionez ca nu am testat tema pe niciun alt calculator.
	
	Pe langa clasele din framework am mai adaugat urmatoarele clase:
	- Square.cpp/.h
		- in care am desenat patratul pe care voi pune textura (nota 
		  muzicala)
	- Am modificat shader-ele:
		- shader_vertex/shader_fragment sunt folosite pentru a desena
		  fata si a face interpolarile
		- shader_vertex_music/shader_fragment_music sunt folosite pentru
		  billboards (animatia notelor muzicale)
		  
	Implementarea:
		- Pentru animatia fetei am modificat functia de loadObj pentru a putea incarca
		  toate cele 3 fete simultan, apeland _loadObj de 3 ori
		- obtin astfel 3 vectori vertices si indices; vertices ii concatenez
		  intr-un singur vector mare, pentru a putea trimite toate punctele
		  o data la shader, ca atribute
		- astfel am acces in shader mereu la cele 3 puncte ce reprezinta cele 
		  3 stari
		- tot in vertex shader am trimis ca variabile uniforme: time si state,
		  unde: time, e o variabila intre [0,1] care ma ajuta sa fac interpolarea
		  intre 2 stari in funcite de variabila state
		- starile sunt:
			state == 0 : somn
			state == 1 : somn -> surpriza
			state == 2 : surpriza -> suparare
			state == 3 : suparare -> somn
		- timpul pentru fiecare stare este de 8s.
		- cat timp sunt in starile 1 si 2 afisez si notele muzicale
		- ciclul se reia la infinit
		- pentru iluminare am folosit ce am lucrat la lab-ul de iluminare
		- pentru notele muzicale, am creeat un patrat pe care am pus textura notei muzicale,
		  - ca sa se vada doar nota, am pus o conditie in care fac discard daca tex1 && tex2 sunt
		    mai mari ca 0.1
		  - pentru culoare am folosit (din structura VertexFormat) campul in_normal (ca tot exista
		    deja); avand in vedere ca negru = (0,0,0) a fost suficient sa adaug la textura culoarea
			dorita.
		  - pentru sistemul de particule pasii sunt urmatorii:
				- retin pentru fiecare particula centrul
				- trimit centrul ca variabila uniforma la shader (la fel si timpul, viteza etc.)
				- calculez noua pozitie a centrului cu formula:
					new_pos = center + v * t + 0.5 * g * t^2,
					unde: v = viteza particulei
						  g = acceleratia gravitationala
						  t = timpul
				- aceasta valoare (new_pos) o adun la in_position (unul din colturile patratului)
				  si obtin noua pozitie a coltului patratului.
				- apoi gl_Position = MVP * noua_positie, adica:
						gl_Position = projection_matrix*view_matrix*model_matrix*new_pos;
				- in total am 500 de particule care au pozitie initiala si viteze diferite
				  (calculate cu functia rand(), dar se mentin intr-un anumit interval, ca
				   sa nu se imprastie pe tot ecranul)
		- cam asta e tot, practic am respectat cerinta pas cu pas.
		- nu am implementat niciun bonus
		
4. Testare
	...
5. Probleme aparute
	...
6. Continutul arhivei
	- lab_geometry.hpp
	- lab_glut.hpp
	- lab_glut_support.hpp
	- lab_mesh_loader.hpp
	- lab_shader_loader.hpp
	- lab_texture_loader.hpp
	- main.cpp
	- shader_fragment.glsl
	- shader_vertex.glsl
	- shader_fragment_music.glsl
	- shader_vertex_music.glsl
	- Square.cpp/.h
	- resursele pentru fata (.obj si textura) si textura pentru nota	
	
7. Functionalitati
	- exact ca in filmuletul din tema
	
	
	
	
	
	
	
	
	
	
	