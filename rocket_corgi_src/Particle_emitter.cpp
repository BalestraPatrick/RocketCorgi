// Susanna Riccardi, Patrick Balestra
#include "Particle_emitter.h"
#include "Particle.h"


ParticleEmitter::ParticleEmitter(Point3d pos){
	this->position = pos;
	this->n_particles = 500;
	init();
}

void ParticleEmitter::init(){
//	this->particles.resize(this->n_particles);
	for(int i = 0; i < this->n_particles; i++){
//		Particle *temp = new Particle(this->position);
		this->particles.push_back(Particle(this->position));
	}
}

void ParticleEmitter::emit_particles(){
	for (int i = 0; i < this->n_particles; i++){
		this->particles.at(i).drawParticle();
	}
}


