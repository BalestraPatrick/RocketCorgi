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

    Point3d off = Point3d(1.0, 0.0, 1.0);
    siv::PerlinNoise perlin(12345);
    for(int i=0; i<world_dim; i++)
    {
        terrainStrips.push_back(PointArray());
        terrainNormals.push_back(PointArray());
            z=0.0;
            for(int j=0; j<world_dim; j++)
            {
                Point3d p = Point3d(cur_x, perlin.octaveNoise(cur_x  * 0.1, (z + 100)  * 0.1, 8) * 2 , z + 100);
                Point2d p_xz = Point2d(p.x(), p.z());
                Point2d off_xy = Point2d(off.x(), off.y());
                Point2d off_yz = Point2d(off.y(), off.z());
                float hL = perlin.octaveNoise((p_xz - off_xy).x() * 0.1, -(p_xz - off_xy).y() * 0.1, 8);
                float hR = perlin.octaveNoise((p_xz + off_xy).x() * 0.1, -(p_xz + off_xy).y() * 0.1, 8);
                float hD = perlin.octaveNoise((p_xz - off_yz).x() * 0.1, -(p_xz - off_yz).y() * 0.1, 8 );
                float hU = perlin.octaveNoise((p_xz + off_yz).x() * 0.1, -(p_xz + off_yz).y() * 0.1, 8);

                // deduce terrain normal
                Point3d n = Point3d(hL - hR, 2.0, hD - hU).normalized();
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
    glPushAttrib(GL_LIGHTING_BIT);
    GLfloat white[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat cyan[] = {0.f, .8f, .8f, 1.f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cyan);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, white);
//    GLfloat shininess[] = {50};
    GLfloat amb[]  = {0.2f, 0.2f, 0.2f};
    GLfloat diff[] = {0.7f, 0.7f, 0.7f};
    GLfloat spec[] = {0.1f, 0.1f, 0.1f};
    GLfloat shin = 0.0001;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shin);
//    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    for(int i=0; i<terrainStrips.size(); i++)
    {
        glBegin(GL_TRIANGLE_STRIP);

            for(int j=0; j<terrainStrips[0].size(); j++)
            {
                Point3d p = terrainStrips[i][j];
                Point3d n = terrainNormals[i][j];
                glNormal3f(n.x(), n.y(), -n.z());
                glVertex3f(p.x(), p.y(), p.z());
            }
        glEnd();
    }
    glPopAttrib();

}

}

#endif // TERRAIN_H
