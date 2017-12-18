#ifndef MATERIALS_H
#define MATERIALS_H

#include <QtOpenGL>

namespace Materials
{
/* Default OpenGL material */
GLfloat defaultAmb[] = {0.2, 0.2, 0.2, 1.0};
GLfloat defaultDiff[] = {0.8, 0.8, 0.8, 1.0};
GLfloat defaultSpec[] = {0.0, 0.0, 0.0, 1.0};
GLfloat defaultEmis[] = {0.0, 0.0, 0.0, 1.0};
GLfloat defaultShin = 0.0;
/* *** */

/* Earth material */
GLfloat earthAmb[] = {0.2, 0.2, 0.2, 1.0};
GLfloat earthDiff[] = {0.8, 0.8, 0.8, 1.0};
GLfloat earthSpec[] = {0.3, 0.3, 0.3, 1.0};
GLfloat earthEmis[] = {0.2, 0.2, 0.2, 1.0};
GLfloat earthShin = 0.01;

/* Corgi body material */
GLfloat corgiAmb[] = {0.4, 0.2, 0.2, 1.0};
GLfloat corgiDiff[] = {0.9, 0.8, 0.8, 1.0};
GLfloat corgiSpec[] = {0.3, 0.3, 0.3, 1.0};
GLfloat corgiEmis[] = {0.0, 0.0, 0.0, 1.0};
GLfloat corgiShin = 0.01;
/* *** */

/* Rockets material */
GLfloat rocketAmb[] = {0.2, 0.2, 0.2, 1.0};
GLfloat rocketDiff[] = {0.3, 0.3, 0.3, 1.0};
GLfloat rocketSpec[] = {0.8, 0.8, 0.8, 1.0};
GLfloat rocketEmis[] = {0.0, 0.0, 0.0, 1.0};
GLfloat rocketShin = 25.0;
/* *** */

/* Terrain Material */
GLfloat terrainAmb[] = {0.f, .4f, .4f, 1.f};
GLfloat terrainDiff[] = {0.8, 0.8, 0.8, 1.0};
GLfloat terrainSpec[] = {0.0, 0.0, 0.0, 1.0};
GLfloat terrainEmis[] = {0.0, 0.0, 0.0, 1.0};
GLfloat terrainShin = 0.0;
/* *** */

/* Sun Material */
GLfloat sunAmb[]  = {1.0f, 1.0f, 0.2f};
GLfloat sunDiff[] = {0.1f, 0.1f, 0.1f};
GLfloat sunSpec[] = {0.1f, 0.1f, 0.1f};
GLfloat sunEmis[] = {0.9f, 0.87f, 0.78f};
GLfloat sunShin = 0.0001;
/* *** */

/* Skybox Material */
GLfloat skyAmb[]  = {1.0f, 1.0f, 1.0f};
GLfloat skyDiff[] = {0.1f, 0.1f, 0.1f};
GLfloat skySpec[] = {0.1f, 0.1f, 0.1f};
GLfloat slyEmis[] = {0.0, 0.0, 0.0, 1.0};
GLfloat skyShin = 0.0001;
/* *** */

void resetDefault(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, defaultAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, defaultDiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &defaultShin);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, defaultEmis);
}

void setEarthMat(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, earthAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, earthDiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, earthSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &earthShin);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, earthEmis);
}

void setCorgiMat(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, corgiAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, corgiDiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, corgiSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &corgiShin);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, corgiEmis);
}

void setRocketsMats(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, rocketAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, rocketDiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rocketSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &rocketShin);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, rocketEmis);
}

void setTerrainMat(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, terrainAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, terrainDiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, terrainSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &terrainShin);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, terrainEmis);
}

void setSunMat(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sunAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sunDiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sunSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &sunShin);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sunEmis);
}

void setSkyMat(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, skyAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, skyDiff);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, skySpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &skyShin);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, defaultEmis);
}

}

#endif // MATERIALS_H
