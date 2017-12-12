#ifndef TERRAIN_H
#define TERRAIN_H



#include "Base.h"
#include "PerlinNoise.hpp"
#include "noise.h"

namespace Terrain {


std::vector<PointArray> terrainStrips;
std::vector<PointArray> terrainNormals;


void generateTerrain(int world_dim) {

    float cur_x,z;
    float x[2]; //keeps last and next x


    // 1000 strips, 1000 triangles long
    x[0] = -world_dim/2.0;
    x[1] = x[0] + 1.0;
    cur_x = x[0];

    Point3d off = Point3d(1.0, 1.0, 0.0);
    siv::PerlinNoise perlin(12345);
    for(int i=0; i<world_dim; i++)
    {
        terrainStrips.push_back(PointArray());
        terrainNormals.push_back(PointArray());
            z=0.0;
            for(int j=0; j<world_dim; j++)
            {
                Point3d p = Point3d(cur_x, perlin.octaveNoise(cur_x  * 0.1, -z  * 0.1, 8) * 2 , z + 100);
                Point2d p_xy = Point2d(p.x(), p.y());
                Point2d off_xz = Point2d(off.x(), off.z());
                Point2d off_zy = Point2d(off.z(), off.y());
                float hL = perlin.octaveNoise((p_xy - off_xz).x() * 0.1, (p_xy - off_xz).y()  * 0.1, 8);

                float hR = perlin.octaveNoise((p_xy + off_xz).x() * 0.1, (p_xy + off_xz).y() * 0.1, 8);
                float hD = perlin.octaveNoise((p_xy - off_zy).x() * 0.1, (p_xy - off_zy).y() * 0.1, 8 );
                float hU = perlin.octaveNoise((p_xy + off_zy).x() * 0.1, (p_xy + off_zy).y() * 0.1, 8);

                // deduce terrain normal
                Point3d n = Point3d(hL - hR, hD - hU, 2.0).normalized();
                terrainNormals[i].push_back(n);
                terrainStrips[i].push_back(p);
                z = z - (j%2); //once every 2 triangles
                cur_x = x[(j+1)%2]; //alternate between two points
            }
        // move to next strip
        x[0]++;
        x[1]++;
    }
}


void drawTerrain()
{

    glDisable(GL_LIGHT0);
    GLfloat light_ambient[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {0, 0, 1, 0.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    //glColor3f(0.0, 0.0, 1.0);
//    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
    //glEnable ( GL_COLOR_MATERIAL );
    glEnable(GL_LIGHT0);


    for(int i=0; i<terrainStrips.size(); i++)
    {
        glBegin(GL_TRIANGLE_STRIP);

            for(int j=0; j<terrainStrips[0].size(); j++)
            {
                Point3d p = terrainStrips[i][j];
                Point3d n = terrainNormals[i][j];
                glNormal3f(n.x(), n.y(), n.z());
                glVertex3f(p.x(), p.y(), p.z());
            }
        glEnd();
    }
    glDisable(GL_LIGHT0);

}

}

#endif // TERRAIN_H
