/**************************************************
Mesh is an abstract class for a 3D rendered object.
*****************************************************/

#ifndef __MESH_H__
#define __MESH_H__

#include"shader.h"
#include <vector>
#include <glm/glm.hpp>

class Mesh {
public:
    GLenum mode = GL_TRIANGLES; // the cookboook for glDrawElements
    int count; // number of elements (indices) to draw
    GLenum type = GL_UNSIGNED_INT; // type of the index array

    GLuint vao; // vertex array object a.k.a. geometry spreadsheet
    std::vector<GLuint> buffers; // data storage

    virtual void init() {};
    virtual void init(const char* s) {};

    virtual void draw(void) {
        glBindVertexArray(vao);
        glDrawElements(mode, count, type, 0);   // uses indices
        glBindVertexArray(0);
    }

    void creategl() {
        glGenVertexArrays(1, &vao);
        buffers.resize(3);
        glGenBuffers(3, &buffers[0]);
    }

    void bindgl(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<unsigned int> indices) {
        unsigned int n = indices.size(); // #(triangles)*3

        count = n;

        glBindVertexArray(vao);

        // 0th attribute: position
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 1st attribute: normal
        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), &normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * n, &indices[0], GL_STATIC_DRAW);

        // unbind the buffers, vao
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // don't unbuffer me!
        glBindVertexArray(0);
    }

    void cleargl() {
        // Delete the VBOs and the VAO.
        glDeleteBuffers(1, &vao);
        glDeleteBuffers(1, &buffers[0]);
        glDeleteBuffers(1, &buffers[1]);
        glDeleteBuffers(1, &buffers[2]);
    }
};

#endif 