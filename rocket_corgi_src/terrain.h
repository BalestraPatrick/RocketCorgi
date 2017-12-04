#ifndef TERRAIN_H
#define TERRAIN_H



#include "Base.h"
#include "PerlinNoise.hpp"
#include "noise.h"

namespace Terrain {



void drawTerrain()
{

    int world_dim = 600;


    float cur_x,z;
    float x[2]; //keeps last and next x

    glPolygonMode(GL_FRONT_AND_BACK ,GL_SMOOTH);
    glShadeModel(GL_SMOOTH);

    // 1000 strips, 1000 triangles long
    x[0] = -world_dim/2.0;
    x[1] = x[0] + 1.0;
    cur_x = x[0];
    for(int i=0; i<world_dim; i++)
    {
        //glNormal3f(1.0, 1.0, 1.0);

        glBegin(GL_TRIANGLE_STRIP);
            z=0.0;
            Point3d off = Point3d(1.0, 1.0, 0.0);
            siv::PerlinNoise perlinNoise(42);
            for(int j=0; j<world_dim; j++)
            {
                Point3d p = Point3d(cur_x, noise(cur_x, z) , z + 100);
                Point2d p_xy = Point2d(p.x(), p.y());
                Point2d off_xz = Point2d(off.x(), off.z());
                Point2d off_zy = Point2d(off.z(), off.y());
                float hL = noise((p_xy - off_xz).x(), (p_xy - off_xz).y());
                float hR = noise((p_xy + off_xz).x(), (p_xy + off_xz).y());
                float hD = noise((p_xy - off_zy).x(), (p_xy - off_zy).y());
                float hU = noise((p_xy + off_zy).x(), (p_xy + off_zy).y());

                // deduce terrain normal
                Point3d n = Point3d(hL - hR, hD - hU, 2.0).normalized();
                glNormal3f(n.x(), n.y(), n.z());
                glVertex3f(p.x(), p.y(), p.z());
                z = z - (j%2); //once every 2 triangles
                cur_x = x[(j+1)%2]; //alternate between two points
            }
        glEnd();
        // move to next strip
        x[0]++;
        x[1]++;
    }
}

}

#endif // TERRAIN_H
