#ifndef SKYBOX_H
#define SKYBOX_H

#include <QtOpenGL>
#include <string>
#include "Base.h"
#include "Point3.h"
#include "Point2.h"

using namespace std;

class Skybox
{
public:
    Skybox(const std::string &path, const std::string &fmt);

    void init();
    void draw();

private:
    vector<GLfloat> fvertices;
    vector<GLfloat> fuvs;
    vector<GLfloat> fnormals;

    vector<string> faces;

    GLuint vertexBuffer;
    GLuint uvBuffer;
    GLuint normalsBuffer;

    //cube map texture itself
    GLuint box;
    GLuint VAO;

};

#endif // SKYBOX_H
