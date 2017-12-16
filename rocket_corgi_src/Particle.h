#ifndef PARTICLE_H
#define PARTICLE_H

#include <QtOpenGL>
#include "Point3.h"
#include "Point2.h"

class Particle {
	public:
		Particle(Point3d pos);
		void drawParticle();
		void moveParticle();

	private:
		Point3d position;
		Point3d original_position;
		int life;
		float size;
		Point3d direction;
};

#endif
