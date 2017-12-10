#ifndef TERRAIN_H
#define TERRAIN_H



#include "Base.h"
#include "PerlinNoise.hpp"
#include "noise.h"

namespace Terrain {

// **** Generation parameters (tweak these)***

int octaves = 8;
float persistence = 0.5;
float exponent = 1.0;
float heigth_range = 300.0;
int world_dim = 600;

// ******************************



void drawTerrain()
{

    float cur_x,z;
    float x[2]; //keeps last and next x
    glDisable(GL_LIGHT0);
    GLfloat light_ambient[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {0, 2.0, 1, 0.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    //glColor3f(0.0, 0.0, 1.0);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
    //glEnable ( GL_COLOR_MATERIAL );
    glEnable(GL_LIGHT0);

    // 1000 strips, 1000 triangles long
    x[0] = 0.0;
    x[1] = x[0] + 1.0;
    cur_x = x[0];
    float freq = 0.5;
    for(int i=0; i<world_dim; i++)
    {
        //glNormal3f(1.0, 1.0, 1.0);

        glBegin(GL_TRIANGLE_STRIP);
            z=0.0;
            Point3d off = Point3d(1.0, 1.0, 0.0);
            siv::PerlinNoise perlin(12345);
            for(int j=0; j<world_dim; j++)
            {
//                Point3d p = Point3d(cur_x, perlin.octaveNoise0_1(cur_x  * freq, -z  * freq, 8) * 2 - 20 , z + 100);
//                float hL = perlin.octaveNoise0_1((p_xy - off_xz).x() * freq, (p_xy - off_xz).y()  * freq, 8);
//                float hR = perlin.octaveNoise0_1((p_xy + off_xz).x() * freq, (p_xy + off_xz).y() * freq, 8);
//                float hD = perlin.octaveNoise0_1((p_xy - off_zy).x() * freq, (p_xy - off_zy).y() * freq, 8 );
//                float hU = perlin.octaveNoise0_1((p_xy + off_zy).x() * freq, (p_xy + off_zy).y() * freq, 8);
//                Point2d p_xy = Point2d(p.x(), p.y());
//                Point2d off_xz = Point2d(off.x(), off.z());
//                Point2d off_zy = Point2d(off.z(), off.y());


                // **** Parameters form elevation
                float nx = 2.0*cur_x/((float)world_dim) - 0.5;
                float ny = -2.0*z/((float)world_dim) - 0.5;
                float elevation = perlin_noise_2D(nx, ny, octaves, persistence);
                elevation = pow(elevation, exponent);
                elevation = elevation*heigth_range;

                Point3d p = Point3d(cur_x, elevation , z);

                Point2d p_xy = Point2d(p.x(), p.y());
                Point2d off_xz = Point2d(off.x(), off.z());
                Point2d off_zy = Point2d(off.z(), off.y());
                float hL = perlin_noise_2D((p_xy - off_xz).x(), (p_xy - off_xz).y() , octaves, persistence);
                float hR = perlin_noise_2D((p_xy + off_xz).x(), (p_xy + off_xz).y(), octaves, persistence);
                float hD = perlin_noise_2D((p_xy - off_zy).x(), (p_xy - off_zy).y(), octaves, persistence);
                float hU = perlin_noise_2D((p_xy + off_zy).x(), (p_xy + off_zy).y(), octaves, persistence);


                // deduce terrain normal
                Point3d n = Point3d(hL - hR, hD - hU, 2.0).normalized();
                glNormal3f(n.x(), n.y(), n.z());
                glTexCoord2f(((int)z+1)%2,j%2);
                glVertex3f(p.x()-world_dim/4.0, p.y()-5, p.z()+world_dim/4.0);
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
