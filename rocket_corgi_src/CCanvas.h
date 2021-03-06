/************************************************************************/
/* Guards                                                               */
/************************************************************************/
#ifndef CCANVAS_H
#define CCANVAS_H

#include <iostream>
#include <QtOpenGL>
#include <QGLWidget>
#include <QTimer>

#include "Base.h"
#include "texture.hpp"

#include "ObjModel.h"
#include "PlyModel.h"
#include "Skybox.h"
#include "Sphere.h"

using namespace std;

//const string PROJECT_FOLDER = "/Users/patrickbalestra/Documents/Github/RocketCorgi";
 const string PROJECT_FOLDER = "/Users/luca/Documents/USI/FS_2017/ComputerGraphics/project/RocketCorgi";
//const string PROJECT_FOLDER = "/Users/BMW/Documents/Git/RocketCorgi";
//const string PROJECT_FOLDER = "/Users/SusannaChuck/Documents/computer_graphics";
//const string PROJECT_FOLDER = "/Users/lara/Documents/USI/5th/ComputerGraphics/Project";
/************************************************************************/
/* Canvas to draw                                                       */
/************************************************************************/
class CCanvas : public QGLWidget
{
  Q_OBJECT

public:
	explicit CCanvas(QWidget *parent = 0) : QGLWidget(parent),
        textureCorgiFur(PROJECT_FOLDER + "/rocket_corgi_src/textures/dog.jpg"),
        textureEngine(PROJECT_FOLDER + "/rocket_corgi_src/textures/steel2.jpg"),
        textureGoggles(PROJECT_FOLDER + "/rocket_corgi_src/textures/glass2.jpg"),
        textureCandyCane(PROJECT_FOLDER + "/rocket_corgi_src/textures/candy_cane.png"),
        textureEarth(PROJECT_FOLDER + "/rocket_corgi_src/textures/earth.jpg"),
        textureOcean(PROJECT_FOLDER + "/rocket_corgi_src/textures/ocean.jpeg"),

        corgiFront(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Corgi_Front.obj"),
        corgiBack(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Corgi_Rear.obj"),
        harness(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Harness.obj"),
        goggles(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Goggles.obj"),
        topRocketLeft(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Engine_Front1.obj"),
        bottomRocketLeft(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Engine_Rear1.obj"),
        topRocketRight(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Engine_Front.obj"),
        bottomRocketRight(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Engine_Rear.obj"),
        candyCane(PROJECT_FOLDER + "/models/CandyCaneOBJ/CandyCane.obj"),
        earth(PROJECT_FOLDER + "/models/EarthOBJ/earth.obj"),
        ocean(PROJECT_FOLDER + "/models/EarthOBJ/ocean.obj"),
        skyCloud(PROJECT_FOLDER + "/skyboxes/Clouds2", "bmp"),
        skyGalaxy(PROJECT_FOLDER + "/skyboxes/Galaxy", "png"),
        sun(20,20)
	{
		QTimer *timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
		timer->start(10);
	}

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

private:
	void lookAt(const GLdouble eyex,
				const GLdouble eyey,
				const GLdouble eyez,
				const GLdouble centerx,
				const GLdouble centery,
				const GLdouble centerz,
				const GLdouble upx,
				const GLdouble upy,
				const GLdouble upz);

	void glPerspective(const GLdouble fovy,
					   const GLdouble aspect,
					   const GLdouble zNear,
					   const GLdouble zFar);


	enum View {
		Perspective = 0,    // View the scene from a perspective (from above, from a side, or whatever)
		Cockpit             // View the scene from the train cockpit (if you want, or whatever other view)
	};

	void setView(View _view);

    void renderCorgi(void);



    // Models and textures
    Texture textureCorgiFur;
    Texture textureEngine;
    Texture textureGoggles;

    // Textures for CandyCane
    Texture textureCandyCane;

    // Textures for Earth
    Texture textureEarth;
    Texture textureOcean;

    /* Models loaded from .obj format */
    // Models for Corgi
	ObjModel corgiFront;
	ObjModel corgiBack;
	ObjModel goggles;
	ObjModel harness;
	ObjModel topRocketRight;
	ObjModel bottomRocketRight;
	ObjModel topRocketLeft;
	ObjModel bottomRocketLeft;
    
    // Models for CandyCane
    ObjModel candyCane;

    // Models for Earth
    ObjModel earth;
    ObjModel ocean;

    // Skybox(es)
    Skybox skyCloud;
    Skybox skyGalaxy;

    PointArray candies;


    // primary light position
    GLfloat lightpos[4];
    // The Sun
    Sphere sun;

    int positionCandyX[200];
    int positionCandyY[200];

	// Model loaded from .ply format
//    PlyModel modelTrain2;
};

#endif
