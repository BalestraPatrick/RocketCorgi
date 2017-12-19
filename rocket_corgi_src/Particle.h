#ifndef PARTICLE_H
#define PARTICLE_H

#include <QtOpenGL>
#include "Point3.h"
#include "Point2.h"

class Particle {
	public:
		Particle(Point3d pos);
		void drawParticle(Point3d camera_pos);
		void moveParticle();

	private:
		Point3d position;
		Point3d original_position;
		int life;
		int start_life = 300;
		double size = 1.0;
		Point3d direction;
};

#endif
