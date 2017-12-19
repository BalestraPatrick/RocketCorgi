// Susanna Riccardi, Patrick Balestra
#include "ParticleEmitter.h"
#include "Particle.h"


ParticleEmitter::ParticleEmitter(Point3d pos){
	this->position = pos;
	this->nParticles = 500;
	init();
}

void ParticleEmitter::init(){
    for(int i = 0; i < this->nParticles; i++){
		this->particles.push_back(Particle(this->position));
	}
}

void ParticleEmitter::emitParticles(Point3d camera_pos){
    for (int i = 0; i < this->nParticles; i++){
		this->particles.at(i).drawParticle(camera_pos);
	}
}


