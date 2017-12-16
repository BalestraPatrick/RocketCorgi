// Susanna Riccardi, Patrick Balestra
#include "Base.h"
#include "Particle.h"
#include <math.h>
#include <stdio.h>

Particle::Particle(Point3d startPosition){
	this->life = this->start_life;
	this->size = 1;
	this->position = startPosition;
//	this->direction = Point3d(1, 0, 0);

	float neg = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float randomX = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if(neg < 0.5){
		randomX = -randomX;
	}
	float randomY = -static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	neg = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float randomZ = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if(neg < 0.5){
		randomZ = -randomZ;
	}


	this->direction = Point3d(randomX, randomY, randomZ);
//	this->position.setCoords(0, 2, 5);
	this->original_position = this->position;
}


void Particle::drawParticle() {
	moveParticle();
	glPushAttrib(GL_LIGHTING_BIT);

	float red = 0.4 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.0-0.4)));
	float green = 0.2 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.4-0.2)));

	float transparency = this->life/this->start_life;


	GLfloat orange[] = {red, green, 0.0f, transparency};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, orange);

	glShadeModel(GL_FLAT);
	glBegin(GL_TRIANGLES);


	float sizeX = this->position.x()+this->size;
	float sizeMinusX = this->position.x()-this->size;
	float sizeY = this->position.y()+this->size;
	float sizeMinusY = this->position.y()-this->size;
	float sizeZ = this->position.z()+this->size;
	float sizeMinusZ = this->position.z()-this->size;


		// Bottom face
		glNormal3d(0, 0, 1);    // Normal of the face
		glVertex3d(sizeMinusX, sizeMinusY, sizeZ);  // Triangle
		glVertex3d(sizeX, sizeMinusY, sizeZ);
		glVertex3d(sizeX, sizeMinusY, sizeMinusZ);

		glVertex3d(sizeMinusX, sizeMinusY, sizeZ);  // Triangle
		glVertex3d(sizeMinusX, sizeMinusY, sizeMinusZ);
		glVertex3d(sizeX, sizeMinusY, sizeMinusZ);

		// Top face
		glNormal3d(0, 0, 1);    // Normal of the face
		glVertex3d(sizeMinusX, sizeY, sizeZ);   // Triangle
		glVertex3d(sizeX, sizeY, sizeZ);
		glVertex3d(sizeX, sizeY, sizeMinusZ);

		glVertex3d(sizeMinusX, sizeY, sizeZ);   // Triangle
		glVertex3d(sizeMinusX, sizeY, sizeMinusZ);
		glVertex3d(sizeX, sizeY, sizeMinusZ);

		// Right face
		glNormal3d(-1, 0, 0);   // Normal of the face
		glVertex3d(sizeX, sizeMinusY, sizeZ);   // Triangle
		glVertex3d(sizeX, sizeMinusY, sizeMinusZ);
		glVertex3d(sizeX, sizeY, sizeZ);

		glVertex3d(sizeX, sizeY, sizeZ);    // Triangle
		glVertex3d(sizeX, sizeMinusY, sizeMinusZ);
		glVertex3d(sizeX, sizeY, sizeMinusZ);

		// Left face
		glNormal3d(-1, 0, 0);   // Normal of the face
		glVertex3d(sizeMinusX, sizeMinusY, sizeZ);  // Triangle
		glVertex3d(sizeMinusX, sizeMinusY, sizeMinusZ);
		glVertex3d(sizeMinusX, sizeY, sizeZ);

		glVertex3d(sizeMinusX, sizeY, sizeZ);   // Triangle
		glVertex3d(sizeMinusX, sizeMinusY, sizeMinusZ);
		glVertex3d(sizeMinusX, sizeY, sizeMinusZ);

		// Front face
		glNormal3d(0, 0, 1);    // Normal of the face
		glVertex3d(sizeMinusX, sizeMinusY, sizeZ);  // Triangle
		glVertex3d(sizeX, sizeMinusY, sizeZ);
		glVertex3d(sizeMinusX, sizeY, sizeZ);

		glVertex3d(sizeX, sizeMinusY, sizeZ);   // Triangle
		glVertex3d(sizeMinusX, sizeY, sizeZ);
		glVertex3d(sizeX, sizeY, sizeZ);

		// Back face
		glNormal3d(0, 0, 1);    // Normal of the face
		glVertex3d(sizeMinusX, sizeMinusY, sizeMinusZ); // Triangle
		glVertex3d(sizeX, sizeMinusY, sizeMinusZ);
		glVertex3d(sizeMinusX, sizeY, sizeMinusZ);

		glVertex3d(sizeX, sizeMinusY, sizeMinusZ);  // Triangle
		glVertex3d(sizeMinusX, sizeY, sizeMinusZ);
		glVertex3d(sizeX, sizeY, sizeMinusZ);

	glEnd();
	glPopAttrib();
}

void Particle::moveParticle(){
	this->life = this->life -1;

	float percentage = 1.0f - ((float)this->life)/((float)this->start_life);
	float reset = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);


	if(reset < percentage || percentage > 1){
		this->life = this->start_life;
		this->position = this->original_position;
	}
	this->position = this->position+direction*0.5;
}

