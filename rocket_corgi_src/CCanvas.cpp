#include "CCanvas.h"
#include "Base.h"
#include "Sphere.h"
#include "terrain.h"
#include "materials.h"
#include "Particle.h"
#include "Particle_emitter.h"

using namespace std;

//-----------------------------------------------------------------------------

void CCanvas::initializeGL()
{
    glClearColor(1.0f, 0.0f, 1.0f, 0.5f);			   // black background
	glClearDepth(1.0f);								   // depth buffer setup
	glEnable(GL_DEPTH_TEST);						   // enables depth testing
	glDepthFunc(GL_LEQUAL);							   // the type of depth testing to do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // really nice perspective calculations
	glShadeModel(GL_SMOOTH);

	// One light source
	glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
	/*
	 * The position is transformed by the modelview matrix when glLightfv is called (just as if it were
	 * a point), and it is stored in eye coordinates. If the w component of the position is 0.0,
	 * the light is treated as a directional source. Diffuse and specular lighting calculations take
	 * the light's direction, but not its actual position, into account, and attenuation is disabled.
	 * Otherwise, diffuse and specular lighting calculations are based on the actual location of the
	 * light in eye coordinates, and attenuation is enabled. The default position is (0,0,1,0); thus,
	 * the default light source is directional, parallel to, and in the direction of the -z axis.
     */

    lightpos[0] = 0.0;
    lightpos[1] = 850.0;
    lightpos[2] = 1.0;
    lightpos[3] = 1.0;
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    GLfloat lightAmb[]  = {0.3, 0.3, 0.3, 1.0};
    GLfloat lightDiff[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lightSpec[] = {1.0, 1.0, 1.0, 1.0};

	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiff);

	/*
	 * Before you can use the texture you need to initialize it by calling the setTexture() method.
	 * Before you can use OBJ/PLY model, you need to initialize it by calling init() method.
	 */
    textureCorgiFur.setTexture();
    textureEngine.setTexture();
    textureGoggles.setTexture();
    textureCandyCane.setTexture();
    textureEarth.setTexture();
    textureOcean.setTexture();

	corgiFront.init();
	corgiBack.init();
	goggles.init();
	harness.init();
	topRocketRight.init();
	bottomRocketRight.init();
	topRocketLeft.init();
    bottomRocketLeft.init();
    candyCane.init();
    earth.init();
    ocean.init();

    Terrain::generateTerrain(600);
    // Setup the skybox(es)
    skyCloud.init();
    skyGalaxy.init();
}

//-----------------------------------------------------------------------------

void CCanvas::glPerspective(const GLdouble fovy, const GLdouble aspect, const GLdouble zNear, const GLdouble zFar)
{
	const GLdouble d = 1.0 / tan(fovy / 360.0 * PI);
	const GLdouble delta = zNear - zFar;

	GLdouble *mat = new GLdouble[16];

	mat[0] = d / aspect;
	mat[1] = 0.0;
	mat[2] = 0.0;
	mat[3] = 0.0;

	mat[4] = 0.0;
	mat[5] = d;
	mat[6] = 0.0;
	mat[7] = 0.0;

	mat[8]  = 0.0;
	mat[9]  = 0.0;
	mat[10] = (zNear + zFar) / delta;
	mat[11] = -1.0;

	mat[12] = 0.0;
	mat[13] = 0.0;
	mat[14] = 2.0 * zNear * zFar / delta;
	mat[15] = 0.0;

	glMultMatrixd(mat);

	delete[] mat;
}

void CCanvas::lookAt(const GLdouble eyeX,
                     const GLdouble eyeY,                        // VP on the course slides
                     const GLdouble eyeZ,
                     const GLdouble centerX,
                     const GLdouble centerY,                    // q on the course slides
                     const GLdouble centerZ,
                     const GLdouble upX,
                     const GLdouble upY,                            // VUP on the course slides
                     const GLdouble upZ )
{
    Point3d VP(eyeX, eyeY, eyeZ);
    Point3d q(centerX, centerY, centerZ);
    Point3d VUP(upX, upY, upZ);
    Point3d VPN = VP-q;

    Point3d _z = VPN.normalized();
    Point3d _x = (VUP^_z).normalized();
    Point3d _y = _z^_x ;

    GLdouble *mat = new GLdouble[16];                            // remember: column-major order!

    // TODO: set up the LookAt matrix correctly!
    mat[0] = _x[0];
    mat[1] = _x[1];
    mat[2] = _x[2];
    mat[3] = 0;

    mat[4] = _y[0];
    mat[5] = _y[1];
    mat[6] = _y[2];
    mat[7] = 0;

    mat[8] = _z[0];
    mat[9] = _z[1];
    mat[10] = _z[2];
    mat[11] = 0;

    mat[12] = q[0];
    mat[13] = q[1];
    mat[14] = q[2];
    mat[15] = 1;

    glMultMatrixd(mat);

    delete[] mat;
}
void CCanvas::resizeGL(int width, int height)
{
	// set up the window-to-viewport transformation
	glViewport(0, 0, width, height);

	// vertical camera opening angle
	double beta = 60.0;

	// aspect ratio
	double gamma;
	if(height > 0) gamma = width / (double)height;
	else gamma = width;

	// front and back clipping plane at
	double n = -0.01;
	double f = -1000.0;

	// frustum corners
	// double t = -tan(beta * 3.14159 / 360.0) * n;
	// double b = -t;
	// double r = gamma * t;
	// double l = -r;

	// set projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// glFrustum(l,r, b,t, -n,-f);

	// alternatively, directly from alpha and gamma
	glPerspective(beta, gamma, -n, -f);
}


double t = 90;
float engineRotation = 0;
float corgiElevation = 1;
float earthRotation = 1;
int candyRotation = 1;
void CCanvas::renderCorgi() {
    glPushMatrix();
    glTranslatef(0.0f, corgiElevation, 0);
    glRotatef(90.0f, 0.0f, 0.0f, 0.0f);
    // Drawing the object with texture
//    glPushAttrib(GL_LIGHTING_BIT);
    Materials::setCorgiMat();
    textureCorgiFur.bind();
    corgiFront.draw();
    corgiBack.draw();
    textureCorgiFur.unbind();
//    glPopAttrib();
    textureGoggles.bind();

    //we move the googles a bit forward
    glPushMatrix();
        glTranslatef(0, 2, 0);
        goggles.draw();
    glPopMatrix();

    textureGoggles.unbind();
//    glPushAttrib(GL_LIGHTING_BIT);
    Materials::setRocketsMats();
    textureEngine.bind();
    harness.draw();
    glPushMatrix();
        // too make object rotate on its axis, we move it back to the origin, rotate and translate to final position
        // note: transformations applied bottom up
        glTranslatef(engineRightFromOrigin.x(),
                     engineRightFromOrigin.y(),
                     engineRightFromOrigin.z());
        glRotatef(-engineRotation, 0.0f, 0.0f, 0.0f);
        glTranslatef(-engineRightFromOrigin.x(),
                     -engineRightFromOrigin.y(),
                     -engineRightFromOrigin.z());
        topRocketRight.draw();
        bottomRocketRight.draw();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(engineLeftFromOrigin.x(),
                     engineLeftFromOrigin.y(),
                     engineLeftFromOrigin.z());
        glRotatef(-engineRotation, 0.0f, 0.0f, 0.0f);
        glTranslatef(-engineLeftFromOrigin.x(),
                     -engineLeftFromOrigin.y(),
                     -engineLeftFromOrigin.z());
        topRocketLeft.draw();
        bottomRocketLeft.draw();
    glPopMatrix();
    textureEngine.unbind();
//    glPopAttrib();
    glTranslatef(0.0, 20.0, 0.0);
    glPopMatrix();
    if(engineRotation < 90)
        engineRotation += 1;
    else if(corgiElevation < 10000)
        corgiElevation = corgiElevation*1.06;

}

bool freeCamera = true;
double freeCameraAngleHorizontal=3.14f;
double freeCameraAngleVertical=0.0;
Point3d freeCameraDirection(cos(freeCameraAngleVertical) * sin(freeCameraAngleHorizontal),
                            sin(freeCameraAngleVertical),
                            cos(freeCameraAngleVertical) * cos(freeCameraAngleHorizontal));
Point3d freeCameraPosition(0, 1, 5);
Point3d freeCameraRight(1,0,0);
Point3d freeCameraForward(0, 0, 1);
Point3d freeCameraUpward(0, 1, 0);
Point3d freeCameraUp = freeCameraRight ^ freeCameraDirection;

float speed = 3.0f;
double deltaTime = 1.0f;

void QWidget::keyPressEvent( QKeyEvent *evt ) {

    freeCameraDirection = Point3d(cos(freeCameraAngleVertical) * sin(freeCameraAngleHorizontal),
                        sin(freeCameraAngleVertical),
                        cos(freeCameraAngleVertical) * cos(freeCameraAngleHorizontal));

    switch (evt->key()) {
        // move camera position
        case Qt::Key_J:
            freeCameraPosition += freeCameraRight * deltaTime * speed;
            break;
        case Qt::Key_L:
            freeCameraPosition -= freeCameraRight * deltaTime * speed;
            break;
        case Qt::Key_I:
            freeCameraPosition += freeCameraForward * deltaTime * speed;
            break;
        case Qt::Key_K:
            freeCameraPosition -= freeCameraForward * deltaTime * speed;
            break;
        case Qt::Key_U:
            freeCameraPosition += freeCameraUpward * deltaTime * speed;
            break;
        case Qt::Key_O:
            freeCameraPosition -= freeCameraUpward * deltaTime * speed;
            break;

        // change camera direction
        case Qt::Key_S:
            freeCameraAngleVertical -= speed * deltaTime * 0.01f;
            break;
        case Qt::Key_W:

            freeCameraAngleVertical += speed * deltaTime * 0.01f;
            break;
        case Qt::Key_A:
            freeCameraAngleHorizontal -= speed * deltaTime * 0.01f;
            break;
        case Qt::Key_D:
            freeCameraAngleHorizontal += speed * deltaTime * 0.01f;
            break;
    }
    freeCameraDirection = Point3d(cos(freeCameraAngleVertical) * sin(freeCameraAngleHorizontal),
                                 sin(freeCameraAngleVertical),
                                 cos(freeCameraAngleVertical) * cos(freeCameraAngleHorizontal));
    freeCameraUp = freeCameraRight ^ freeCameraDirection;
    freeCameraUp.normalize();
}



//-----------------------------------------------------------------------------
void CCanvas::setView(View _view) {
	switch(_view) {
	case Perspective:
        glTranslatef(1.0, -2.5, -10.0);
//		glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
		break;
	case Cockpit:
		// Maybe you want to have an option to view the scene from the train cockpit, up to you
		break;
	}
}


void CCanvas::paintGL()
{
	// clear screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set model-view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    lookAt(	freeCameraPosition.x(), freeCameraPosition.y(), freeCameraPosition.z(),
            freeCameraPosition.x() + freeCameraDirection.x(), freeCameraDirection.y()+freeCameraPosition.y(),  freeCameraPosition.z()+freeCameraDirection.z(),
            freeCameraUp.x(), freeCameraUp.y(),  freeCameraUp.z());

    /****************************************************************
    * Light position should be called here, after the lookAt to have a global light position (i.e. it does not move with camera)
    * The position is set in initialiyzeGL and stored in global variable so that also other functions can access it.
    * It is instead not necessary to redefine the values of the light (ambient, diffuse, specular) as those are state variables of GL_LIGHT0
    */
     glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    /****************************************************************/

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Setup the current view
    setView(View::Perspective);

    GLfloat matrix[16];
    glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
	/**** Axes in the global coordinate system ****/

    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(-6.0f, 0.0f, 0.0f);
    glVertex3f(6.0f, 0.0f, 0.0f);
    glEnd();
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, -6.0f, 0.0f);
    glVertex3f(0.0f, 6.0f, 0.0f);
    glEnd();
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, -6.0f);
    glVertex3f(0.0f, 0.0f, 6.0f);
    glEnd();
    glEnable(GL_LIGHTING);

	/**** Draw the terrain ***/
    glPushAttrib(GL_LIGHTING_BIT);
        Materials::setTerrainMat();
        Terrain::drawTerrain();
    glPopAttrib();
    /**** Draw the sky ***/
    glPushAttrib(GL_LIGHTING_BIT);
        Materials::setSkyMat();
        glPushMatrix();
            glScalef(450.0, 450.0, 450.0);
                skyGalaxy.draw();
            //    skyCloud.draw();
        glPopMatrix();
    glPopAttrib();

    // Draw the sun
    // PRAISE THE SUN
    glPushAttrib(GL_LIGHTING_BIT);
        Materials::setSunMat();
        glPushMatrix();
            glTranslatef(lightpos[0], lightpos[1], lightpos[2]);
            glScalef(15.0,15.0,15.0);
            sun.draw();
        glPopMatrix();
    glPopAttrib();

	/**** Setup and draw your objects ****/

	// You can stack new transformation matrix if you don't want
	// the previous transformations to apply on this object
    glPushMatrix();
	/*
	 * Obtaining the values of the current modelview matrix
	 *  GLfloat matrix[16];
	 *  glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
	*/


    // Draw the objects
    // Draw candy canes
    glPushAttrib(GL_LIGHTING_BIT);
    Materials::resetDefault();
    for (int i = -100; i < 100; i += 20) {
        for (int j = -100; j < 100; j += 20) {
            glPushMatrix();
            glTranslatef(j, 0, i);
            glRotatef(((i * j) + candyRotation % 360), 0.0f, 1.0f, 0.0f);
            textureCandyCane.bind();
            candyCane.draw();
            textureCandyCane.unbind();
            glPopMatrix();
        }
    }
    candyRotation++;
    glPopAttrib();

    // Draw the Earth
    glPushMatrix();
    glScalef(1.0f, 1.00f, 1.00f);
    glTranslatef(-5, 7, 0);
    glRotatef(earthRotation, 0.0f, 1.0f, 0.0f);
    textureEarth.bind();
    earth.draw();
    textureEarth.unbind();

    textureOcean.bind();
    ocean.draw();
    textureOcean.unbind();
    glPopMatrix();

    // Draw the Corgi
    glScalef(0.05f, 0.05f, 0.05f);
    renderCorgi();

    glTranslatef(0.0f, corgiElevation, 0);
    glRotatef(90.0f, 0.0f, 0.0f, 0.0f);
    // Drawing the object with texture
    textureCorgiFur.bind();
    corgiFront.draw();
    corgiBack.draw();
    textureCorgiFur.unbind();
    textureGoggles.bind();

    //we move the googles a bit forward
    glPushMatrix();
    glTranslatef(0, 2, 0);
    goggles.draw();
    glPopMatrix();

    textureGoggles.unbind();
    textureEngine.bind();
    harness.draw();
    glPushMatrix();

    Point3d left_engine = Point3d(engineLeftFromOrigin.x(), engineLeftFromOrigin.y()-25, engineLeftFromOrigin.z());
    Point3d right_engine = Point3d(engineRightFromOrigin.x(), engineRightFromOrigin.y()-25, engineRightFromOrigin.z());

    static ParticleEmitter right_particles(right_engine);
    static ParticleEmitter left_particles(left_engine);




    // too make object rotate on its axis, we move it back to the origin, rotate and translate to final position
    // note: transformations applied bottom up
    glTranslatef(engineRightFromOrigin.x(),
                 engineRightFromOrigin.y(),
                 engineRightFromOrigin.z());
    glRotatef(-engineRotation, 0.0f, 0.0f, 0.0f);
    glTranslatef(-engineRightFromOrigin.x(),
                 -engineRightFromOrigin.y(),
                 -engineRightFromOrigin.z());
    topRocketRight.draw();
    bottomRocketRight.draw();
    glPushMatrix();
    right_particles.emit_particles();
    left_particles.emit_particles();
    glPopMatrix();


    glPopMatrix();
    glPushMatrix();
    glTranslatef(engineLeftFromOrigin.x(),
                 engineLeftFromOrigin.y(),
                 engineLeftFromOrigin.z());
    glRotatef(-engineRotation, 0.0f, 0.0f, 0.0f);
    glTranslatef(-engineLeftFromOrigin.x(),
                 -engineLeftFromOrigin.y(),
                 -engineLeftFromOrigin.z());
    topRocketLeft.draw();
    bottomRocketLeft.draw();
    glPopMatrix();
    textureEngine.unbind();

    glPopMatrix();
	// Remove the last transformation matrix from the stack - you have drawn your last
	// object with a new transformation and now you go back to the previous one

    glPopMatrix();


    earthRotation += 0.20;
}
