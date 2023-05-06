/**************************************************
Obj is subclass class of Mesh
that loads an obj file.
*****************************************************/
#define GLM_FORCE_RADIANS

#ifndef __OBJ_H__
#define __OBJ_H__

#include <stdio.h>

#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "client/graphics/Mesh.h"

class Obj : public Mesh {
 public:
  Obj() { creategl(); }
  ~Obj() { cleargl(); }

  void init(const char* filename) {
    std::vector<glm::vec3> temp_vertices, vertices;
    std::vector<glm::vec3> temp_normals, normals;
    std::vector<glm::vec2> temp_uvs, uvs;
    std::vector<unsigned int> temp_vertexIndices, indices;
    std::vector<unsigned int> temp_normalIndices;
    std::vector<unsigned int> temp_textureIndices;

    bool hasUV = false;

    std::cout << "Obj: Importing " << filename << "..." << std::endl;
    // load obj file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
      perror("fopen() failed");
      std::cerr << "Cannot open file: " << filename << std::endl;
      return;
      // exit(-1);
    }
    std::cout << "\tLoading mesh data...";
    while (!feof(file)) {
      char lineHeader[128];
      // read the first word of the line
      int res = fscanf(file, "%s", lineHeader);
      if (res == EOF) break;  // EOF = End Of File. Quit the loop.

      // else : parse lineHeader
      if (strcmp(lineHeader, "o") == 0) {
        // separate object. assume all meshes are one object for now
      } else if (strcmp(lineHeader, "v") == 0) {
        glm::vec3 vertex;
        fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
        temp_vertices.push_back(vertex);
      } else if (strcmp(lineHeader, "vt") == 0) {
        hasUV = true;
        glm::vec2 uv;
        fscanf(file, "%f %f\n", &uv.x, &uv.y);
        temp_uvs.push_back(uv);
      } else if (strcmp(lineHeader, "vn") == 0) {
        glm::vec3 normal;
        fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
        temp_normals.push_back(normal);
      } else if (strcmp(lineHeader, "f") == 0) {
        // std::string vertex1, vertex2, vertex3;
        unsigned int vertexIndex[3], textureIndex[3], normalIndex[3];
        if (hasUV) {
          fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0],
                 &textureIndex[0], &normalIndex[0], &vertexIndex[1],
                 &textureIndex[1], &normalIndex[1], &vertexIndex[2],
                 &textureIndex[2], &normalIndex[2]);
        } else
          fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0],
                 &normalIndex[0], &vertexIndex[1], &normalIndex[1],
                 &vertexIndex[2], &normalIndex[2]);
        temp_vertexIndices.push_back(vertexIndex[0]);
        temp_vertexIndices.push_back(vertexIndex[1]);
        temp_vertexIndices.push_back(vertexIndex[2]);
        temp_normalIndices.push_back(normalIndex[0]);
        temp_normalIndices.push_back(normalIndex[1]);
        temp_normalIndices.push_back(normalIndex[2]);
        temp_textureIndices.push_back(textureIndex[0]);
        temp_textureIndices.push_back(textureIndex[1]);
        temp_textureIndices.push_back(textureIndex[2]);
      }
    }
    std::cout << "done." << std::endl;

    // post processing
    std::cout << "\tProcessing mesh data...";    // this is probably not needed
    unsigned int n = temp_vertexIndices.size();  // #(triangles)*3
    vertices.resize(n);
    uvs.resize(n);
    normals.resize(n);
    indices.resize(n);
    for (unsigned int i = 0; i < n; i++) {
      indices[i] = i;
      vertices[i] = temp_vertices[temp_vertexIndices[i] - 1];
      if (hasUV) uvs[i] = temp_uvs[temp_textureIndices[i] - 1];
      normals[i] = temp_normals[temp_normalIndices[i] - 1];
    }
    std::cout << "done." << std::endl;

    // setting up buffers
    std::cout << "\tSetting up mesh GL buffers...";

    if (hasUV)
      bindgl(vertices, normals, uvs, indices);
    else
      bindgl(vertices, normals, indices);

    std::cout << "done." << std::endl;

    std::cout << "Obj " << filename << " loaded successfully." << std::endl;
  }
};

#endif
