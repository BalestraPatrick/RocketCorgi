#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <QtOpenGL>
#include "Point3.h"
#include "Point2.h"
#include "Particle.h"
#include <vector>

class ParticleEmitter {
	public:
		ParticleEmitter(Point3d pos);
		void emitParticles(Point3d camera_pos);
		void init();

	private:
		Point3d position;
		std::vector<Particle> particles;
        int nParticles;
};

#endif
