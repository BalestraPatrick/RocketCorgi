// Susanna Riccardi, Patrick Balestra
#include "Base.h"
#include "Particle.h"
#include <math.h>
#include <stdio.h>

Particle::Particle(Point3d startPosition){
	this->life = this->start_life;
	this->position = startPosition;

	float neg = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float randomX = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.7f)));

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
	this->original_position = this->position;
}


void Particle::drawParticle(Point3d camera_pos) {
	moveParticle();
	glPushAttrib(GL_LIGHTING_BIT);

	float colour = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float transparency = this->life/this->start_life;


	Point3d normal = (this->position - camera_pos);
	normal.normalize();

	GLfloat red[] = {1.0f, 0.1, 0.0f, transparency};
	GLfloat orange[] = {0.9, 0.5, 0.0f, transparency};
	GLfloat yellow[] = {0.9, 0.7, 0.2f, transparency};
	GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};

	if(colour < 0.2){
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, red);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, red);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, red);
	} else if (colour < 0.4){
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, orange);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, orange);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, orange);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, orange);
	} else {
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, yellow);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, yellow);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, yellow);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, yellow);
	}


		float sizeX = this->position.x()+this->size;
		float sizeMinusX = this->position.x()-this->size;
		float sizeY = this->position.y()+this->size;
		float sizeMinusY = this->position.y()-this->size;
		float sizeZ = this->position.z()+this->size;
		float sizeMinusZ = this->position.z()-this->size;


	glShadeModel(GL_FLAT);

	glBegin(GL_POLYGON);
	glNormal3d(normal.x(), normal.y(), normal.z());
	glVertex3d(sizeMinusX, this->position.y(), this->position.z());
	glVertex3d(this->position.x(), sizeY, this->position.z());
	glVertex3d(sizeX, this->position.y(), this->position.z());
	glVertex3d(this->position.x(), sizeMinusY, this->position.z());
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3d(normal.x(), normal.y(), normal.z());
	glVertex3d(this->position.x(), this->position.y(), sizeMinusZ);
	glVertex3d(this->position.x(), sizeY, this->position.z());
	glVertex3d(this->position.x(), this->position.y(), sizeZ);
	glVertex3d(this->position.x(), sizeMinusY, this->position.z());
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

