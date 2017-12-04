#ifndef TERRAIN_H
#define TERRAIN_H


#include "noise.h"
#include "Base.h"
namespace Terrain {

void drawTerrain()
{

    int world_dim = 100;
    // **** Generation parameters (tweak these)***

    int octaves = 6;
    float persistence = 0.5;
    float exponent = 2.15;
    float heigth_range = 500.0;

    // ******************************

    float cur_x,z;
    float x[2]; //keeps last and next x

    glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE);
    glShadeModel(GL_FLAT);

    // 1000 strips, 1000 triangles long
    x[0] = -world_dim/2.0;
    x[1] = x[0] + 1.0;
    cur_x = x[0];
    for(int i=0; i<world_dim; i++)
    {
        glNormal3f(0.0, 1.0, 0.0);
        glBegin(GL_TRIANGLE_STRIP);
            z=0.0;
            for(int j=0; j<world_dim; j++)
            {
                // **** Parameters form elevation
                float nx = (float)i/(float)world_dim - 0.5;
                float ny = (float)j/(float)world_dim - 0.5;
//                float elevation = noise(nx, ny);
                float elevation = perlin_noise_2D(nx, ny, octaves, persistence);
                elevation = pow(elevation, exponent);

                elevation = elevation*heigth_range;
                // ****
//                std::cout << elevation << " " << endl;
                glVertex3f(cur_x, elevation, z);
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
