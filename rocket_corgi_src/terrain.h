#ifndef TERRAIN_H
#define TERRAIN_H



#include "Base.h"
#include "PerlinNoise.hpp"
#include "noise.h"

namespace Terrain {


Point3d scale = Point3d(10.0, 4.0, 10.0);

int octaves = 10;
float fac = 0.01;
float hscale = 10.0;
float exponent = 2.0;

int Dim = 0;

static siv::PerlinNoise perlin(026700375);

std::vector<PointArray> terrainStrips;
std::vector<PointArray> terrainNormals;


double getElevation(float x, float y){
    Point2d noiseCoords = Point2d(x * fac, y * fac);
    double h = perlin.octaveNoise(noiseCoords.x() ,noiseCoords.y(), octaves)*hscale;
    h = pow(h, exponent);

    return h;
}

float getHeight(int x, int y){
    float limit_x = scale.x()*Dim/2.0;
    float limit_y = scale.z()*Dim/2.0;
    if(x < -limit_x || x > limit_x) return 0.0;
    if(y < -limit_y || y > limit_y) return 0.0;

    PointArray strip = terrainStrips[(int)floor(x/scale.x() + Dim/2.0)];
    Point3d p = strip[(int)floor(y/scale.y() + Dim/2.0)*2 + 2];
    return p.y();
}

void scaleUp(Point3d &p){
    p[0] *= scale.x();
    p[1] *= scale.y();
    p[2] *= scale.z();
}

void generateTerrain(int world_dim) {

    Dim = world_dim;
    float cur_x,z;
    float x[2]; //keeps last and next x


    // 1000 strips, 1000 triangles long
    x[0] = -world_dim/2.0;
    x[1] = x[0] + 1.0;
    cur_x = x[0];

    Point3d off = Point3d(1.0, 0.0, 1.0);

    for(int i=0; i<world_dim; i++)
    {
        terrainStrips.push_back(PointArray());
        terrainNormals.push_back(PointArray());
            z=world_dim/2.0;
            for(int j=0; j<2*world_dim; j++)
            {
                /* Get y component by noise function */
                double elevation = getElevation(cur_x, z);
                Point3d p = Point3d(cur_x, elevation , z);
                /* spread out the terrain */
                scaleUp(p);

                /* calculate normal by differential */
                Point2d p_xz = Point2d(p.x(), p.z());
                Point2d off_xy = Point2d(off.x(), off.y());
                Point2d off_yz = Point2d(off.y(), off.z());
                float hL = perlin.octaveNoise((p_xz - off_xy).x() * fac, -(p_xz - off_xy).y() * fac, 8);
                float hR = perlin.octaveNoise((p_xz + off_xy).x() * fac, -(p_xz + off_xy).y() * fac, 8);
                float hD = perlin.octaveNoise((p_xz - off_yz).x() * fac, -(p_xz - off_yz).y() * fac, 8 );
                float hU = perlin.octaveNoise((p_xz + off_yz).x() * fac, -(p_xz + off_yz).y() * fac, 8);

                // deduce terrain normal
                Point3d n = Point3d(hL - hR, 1.0, hD - hU).normalized();
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
//    glPopAttrib();

}

}

#endif // TERRAIN_H
