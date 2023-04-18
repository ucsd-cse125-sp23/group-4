/**************************************************
Obj is subclass class of Mesh
that loads an obj file.
*****************************************************/
#define GLM_FORCE_RADIANS

#ifndef __OBJ_H__
#define __OBJ_H__

#include <glm/glm.hpp>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Mesh.h"

class Obj : public Mesh {
public:

    glm::mat4 model;
    glm::vec3 color;

    ~Obj() {
        cleargl();
    }

    void init(const char* filename) {
        color = glm::vec3(0.7f, 0.25f, 0.1f);

        std::vector< glm::vec3 > temp_vertices, vertices;
        std::vector< glm::vec3 > temp_normals, normals;
        std::vector< unsigned int > temp_vertexIndices, indices;
        std::vector< unsigned int > temp_normalIndices;

        // load obj file
        FILE* file = fopen(filename, "r");
        if (file == NULL) {
            std::cerr << "Cannot open file: " << filename << std::endl;
            exit(-1);
        }
        std::cout << "Loading " << filename << "...";
        while (!feof(file)) {
            char lineHeader[128];
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.

            // else : parse lineHeader
            if (strcmp(lineHeader, "v") == 0) {
                glm::vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                temp_vertices.push_back(vertex);
            }
            else if (strcmp(lineHeader, "vn") == 0) {
                glm::vec3 normal;
                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
                temp_normals.push_back(normal);
            }
            else if (strcmp(lineHeader, "f") == 0) {
                //std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], normalIndex[3];
                fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
                temp_vertexIndices.push_back(vertexIndex[0]);
                temp_vertexIndices.push_back(vertexIndex[1]);
                temp_vertexIndices.push_back(vertexIndex[2]);
                temp_normalIndices.push_back(normalIndex[0]);
                temp_normalIndices.push_back(normalIndex[1]);
                temp_normalIndices.push_back(normalIndex[2]);
            }
        }
        std::cout << "done." << std::endl;

        // post processing
        std::cout << "Processing data...";
        unsigned int n = temp_vertexIndices.size(); // #(triangles)*3
        vertices.resize(n);
        normals.resize(n);
        indices.resize(n);
        for (unsigned int i = 0; i < n; i++) {
            indices[i] = i;
            vertices[i] = temp_vertices[temp_vertexIndices[i] - 1];
            normals[i] = temp_normals[temp_normalIndices[i] - 1];
        }
        std::cout << "done." << std::endl;

        // setting up buffers
        std::cout << "Setting up buffers...";

        bindgl(vertices, normals, indices);

        std::cout << "done." << std::endl;
    }

    // deprecated vvv, use Model.h instead
    void draw(const glm::mat4& viewProjMtx, GLuint shader)
    {
        // actiavte the shader program 
        glUseProgram(shader);

        // get the locations and send the uniforms to the shader
        // TODO: create a "setUniforms" method
        glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
        glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

        // Bind the VAO
        glBindVertexArray(vao);

        // draw the points using triangles, indexed with the EBO
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

        // Unbind the VAO and shader program
        glBindVertexArray(0);
        glUseProgram(0);
    }
};

#endif 
