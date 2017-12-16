// Susanna Riccardi, Patrick Balestra
#include "Particle_emitter.h"
#include "Particle.h"


ParticleEmitter::ParticleEmitter(Point3d pos){
	this->position = pos;
	this->n_particles = 500;
	init();
}

void ParticleEmitter::init(){
	for(int i = 0; i < this->n_particles; i++){
		this->particles.push_back(Particle(this->position));
	}
}

void ParticleEmitter::emit_particles(){
	for (int i = 0; i < this->n_particles; i++){
		this->particles.at(i).drawParticle();
	}
}


