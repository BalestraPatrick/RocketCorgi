#include "CCanvas.h"
#include "Base.h"
#include "Sphere.h"
#include "terrain.h"

using namespace std;

//-----------------------------------------------------------------------------

void CCanvas::initializeGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);			   // black background
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
	GLfloat lightpos[] = {0.0, 0.0, 10.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	GLfloat lightAmb[]  = {0.3, 0.3, 0.3};
	GLfloat lightDiff[] = {0.4, 0.4, 0.4};
	GLfloat lightSpec[] = {0.5, 0.5, 0.5};

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

	corgiFront.init();
	corgiBack.init();
	goggles.init();
	harness.init();
	topRocketRight.init();
	bottomRocketRight.init();
	topRocketLeft.init();
	bottomRocketLeft.init();

    Terrain::generateTerrain(600);
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

double t = 90;
float engineRotation = 0;
float corgiElevation = 1;
void CCanvas::paintGL()
{
	// clear screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set model-view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    t+=0.01;
//    lookAt(0,0,10-corgiElevation, //position of cam
//    0, -corgiElevation, 0,
//    0,1,0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Setup the current view
    setView(View::Perspective);

	// You can always change the light position here if you want
    GLfloat lightpos[] = {0.0f, 100.0f, 100.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

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
    Terrain::drawTerrain();

	/**** Setup and draw your objects ****/

	// You can freely enable/disable some of the lights in the scene as you wish
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	// Before drawing an object, you can set its material properties

//    glColor3f(0.5f, 0.5f, 0.5f);
//    GLfloat amb[]  = {0.1f, 0.1f, 0.1f};
//    GLfloat diff[] = {0.7f, 0.7f, 0.7f};
//    GLfloat spec[] = {0.1f, 0.1f, 0.1f};
//    GLfloat shin = 0.0001;
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shin);


	// You can stack new transformation matrix if you don't want
	// the previous transformations to apply on this object
    //glPushMatrix();
	/*
	 * Obtaining the values of the current modelview matrix
	 *  GLfloat matrix[16];
	 *  glGetFloatv (GL_MODELVIEW_MATRIX, matrix);
	*/
	GLfloat matrix[16];
	glGetFloatv (GL_MODELVIEW_MATRIX, matrix);

	// Look at the ObjModel class to see how the drawing is done
    glScalef(0.05f, 0.05f, 0.05f);
	glPushMatrix();


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

    glPopMatrix();

	// Remove the last transformation matrix from the stack - you have drawn your last
	// object with a new transformation and now you go back to the previous one
    glPopMatrix();

    if(engineRotation < 90)
        engineRotation += 1;
    else if(corgiElevation < 10000)
        corgiElevation = corgiElevation*1.06;


}
