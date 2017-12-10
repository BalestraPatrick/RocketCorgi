#include "Base.h"
#include <math.h>
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

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, fvertices.size() * sizeof(GLfloat), &fvertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, fuvs.size() * sizeof(GLfloat), &fuvs[0], GL_STATIC_DRAW);

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

        //using explicit AA 2 to pass vetex info to vertex shader
        int AA_index=2;

        //Generate VBO
        GLuint VBO = createBuffer(GL_ARRAY_BUFFER, vertices, sizeof(GLfloat) * (36*3), GL_STATIC_DRAW); //buffer_tools.cpp

        //Generate VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(AA_index);
        attributeBind(VBO, AA_index, 3); //buffer_tools.cpp
        glBindVertexArray(0);
}

void Skybox::draw(){

        //don't write to Z buffer
        glDepthMask(GL_FALSE);

        //bind the skybox VAO (sets up all vertex data)
        glBindVertexArray(VAO);

        //bind texture object to cube-map target on texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, box);

        //tell OpenGL to fill polygons
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //draw the sky box
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //turn depth writing on again
        glDepthMask(GL_TRUE);

        //unbind VAO
        glBindVertexArray(0);

        //switch back to normal shaders
        glUseProgram(programID);
}
