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

#include "client/graphics/Mesh.h"

class Obj : public Mesh {
public:
    Obj() {
        creategl();
    }
    ~Obj() {
        cleargl();
    }

    void init(const char* filename) {

        std::vector< glm::vec3 > temp_vertices, vertices;
        std::vector< glm::vec3 > temp_normals, normals;
        std::vector< unsigned int > temp_vertexIndices, indices;
        std::vector< unsigned int > temp_normalIndices;

        std::cout << "Obj: Importing " << filename << "..." << std::endl;
        // load obj file
        FILE* file = fopen(filename, "r");
        if (file == NULL) {
            perror("fopen() failed");
            std::cerr << "Cannot open file: " << filename << std::endl;
            return;
            //exit(-1);
        }
        std::cout << "\tLoading mesh data...";
        while (!feof(file)) {
            char lineHeader[128];
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.

            // else : parse lineHeader
            if (strcmp(lineHeader, "o") == 0) {
                // separate object. assume all meshes are one object for now
            }
            else if (strcmp(lineHeader, "v") == 0) {
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
        std::cout << "\tProcessing mesh data...";   // this is probably not needed
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
        std::cout << "\tSetting up mesh GL buffers...";

        bindgl(vertices, normals, indices);

        std::cout << "done." << std::endl;

        std::cout << "Obj " << filename << " loaded successfully." << std::endl;
    }
};

#endif 