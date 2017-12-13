#include "Base.h"
#include <math.h>
#include <QtOpenGL>
#include "Skybox.h"

Skybox::Skybox(const std::string &path)
{
    //Load the cube map, paint each side
    faces.push_back(PROJECT_FOLDER + "/skyboxes/"+ path +"/right.png");
    faces.push_back(PROJECT_FOLDER + "/skyboxes/"+ path +"/left.png");
    faces.push_back(PROJECT_FOLDER + "/skyboxes/"+ path +"/top.png");
    faces.push_back(PROJECT_FOLDER + "/skyboxes/"+ path +"/bottom.png");
    faces.push_back(PROJECT_FOLDER + "/skyboxes/"+ path +"/back.png");
    faces.push_back(PROJECT_FOLDER + "/skyboxes/"+ path +"/front.png");
}

void Skybox::init(){

        glGenTextures(1, &box);
        glBindTexture(GL_TEXTURE_CUBE_MAP, box);

        // we can not use the TExture class here
        // since we are creating a 3D cubic texture
        // Construct the 3D texture (equivalent to Texture::setTexture)
        for(int i = 0; i < faces.size(); i++)
        {
            QImageReader reader(faces[i].c_str());
            QImage img;
            const bool read = reader.read(&img);

            if(!read) {
                std::cout << "Failed to read: " << faces[i].c_str() << " with message:" << reader.errorString().toStdString().c_str() << "; " << std::endl;
                assert(read);
                return;
            }
            assert(img.width() > 0.0);

            img = QGLWidget::convertToGLFormat(img);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


        //Create unit cube to splat cube-map texture onto
        GLfloat D =1.0f;
        GLfloat vertices[] = {
           -D,  D, -D,
           -D, -D, -D,
            D, -D, -D,
            D, -D, -D,
            D,  D, -D,
           -D,  D, -D,

           -D, -D,  D,
           -D, -D, -D,
           -D,  D, -D,
           -D,  D, -D,
           -D,  D,  D,
           -D, -D,  D,

            D, -D, -D,
            D, -D,  D,
            D,  D,  D,
            D,  D,  D,
            D,  D, -D,
            D, -D, -D,

           -D, -D,  D,
           -D,  D,  D,
            D,  D,  D,
            D,  D,  D,
            D, -D,  D,
           -D, -D,  D,

           -D,  D, -D,
            D,  D, -D,
            D,  D,  D,
            D,  D,  D,
           -D,  D,  D,
           -D,  D, -D,

           -D, -D, -D,
           -D, -D,  D,
            D, -D, -D,
            D, -D, -D,
           -D, -D,  D,
            D, -D,  D
        };
        fvertices.insert(fvertices.begin(), vertices, vertices + (36*3));

        // create buffers
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, fvertices.size() * sizeof(GLfloat), &fvertices[0], GL_STATIC_DRAW);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        //bind the skybox vertexBuffer (sets up all vertex data)
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexPointer(
                    3,                  // size
                    GL_FLOAT,           // type
                    0,                  // stride
                    (void*)0            // array buffer offset
                );
        glBindVertexArray(0);
}

void Skybox::draw(){

        //don't write to Z buffer
//        glDepthMask(GL_FALSE);

        glEnableClientState(GL_VERTEX_ARRAY);

        //bind texture object to cube-map target on texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, box);

        //tell OpenGL to fill polygons
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //draw the sky box
        glDrawArrays(GL_TRIANGLES, 0, fvertices.size() / 3);

        glDisable(GL_TEXTURE_CUBE_MAP);
        //turn depth writing on again
//        glDepthMask(GL_TRUE);

        //unbind vertexBuffer
        glDisableClientState(GL_VERTEX_ARRAY);

//    //don't write to Z buffer
//    glDepthMask(GL_FALSE);

//    //bind the skybox VAO (sets up all vertex data)
//    glBindVertexArray(VAO);

//    //bind texture object to cube-map target on texture unit 0
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, box);

//    //tell OpenGL to fill polygons
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//    //draw the sky box
//    glDrawArrays(GL_TRIANGLES, 0, 36);

//    //turn depth writing on again
//    glDepthMask(GL_TRUE);

//    //unbind VAO
//    glBindVertexArray(0);

}
