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

using namespace std;
//const string PROJECT_FOLDER = "/Users/patrickbalestra/Documents/Github/RocketCorgi";
//const string PROJECT_FOLDER = "/Users/luca/Documents/USI/FS_2017/ComputerGraphics/project/RocketCorgi";
//const string PROJECT_FOLDER = "/Users/BMW/Documents/Git/RocketCorgi";

const string PROJECT_FOLDER = "/Users/lara/Documents/USI/5th/ComputerGraphics/Project";
/************************************************************************/
/* Canvas to draw                                                       */
/************************************************************************/
class CCanvas : public QGLWidget
{
  Q_OBJECT

public:
	explicit CCanvas(QWidget *parent = 0) : QGLWidget(parent),
		textureCorgiFur(PROJECT_FOLDER + "/rocket_corgi_src/textures/fur2.jpg"),
		textureEngine(PROJECT_FOLDER + "/rocket_corgi_src/textures/steel2.jpg"),
		textureGoggles(PROJECT_FOLDER + "/rocket_corgi_src/textures/greenglass1.jpg"),
        corgiFront(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Corgi_Front.obj"),
        corgiBack(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Corgi_Rear.obj"),
        harness(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Harness.obj"),
        goggles(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Goggles.obj"),
        topRocketLeft(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Engine_Front1.obj"),
        bottomRocketLeft(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Engine_Rear1.obj"),
        topRocketRight(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Engine_Front.obj"),
        bottomRocketRight(PROJECT_FOLDER + "/models/RocketCorgiOBJ/downsampled/Engine_Rear.obj")
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


	// Models and textures
    Texture textureCorgiFur;
    Texture textureEngine;
    Texture textureGoggles;
	// Model loaded from .obj format
	ObjModel corgiFront;
	ObjModel corgiBack;
	ObjModel goggles;
	ObjModel harness;
	ObjModel topRocketRight;
	ObjModel bottomRocketRight;
	ObjModel topRocketLeft;
	ObjModel bottomRocketLeft;


	// Model loaded from .ply format
//    PlyModel modelTrain2;
};

#endif
