/**************************************************
MapObj is subclass class of Model
that organizes several Meshes with several Materials.
*****************************************************/

#pragma once

#define GLM_FORCE_RADIANS

#include <stdio.h>

#include <algorithm>
#include <cstring>
#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "client/graphics/Model.h"

struct MapObjSubmesh {
  std::string name;
  Mesh mesh;

  Material* material = nullptr;

  void trySetMaterial(std::string tag,
                      std::map<std::string, Material*> resources) {
    if (tag.length() == 0) return;

    if (resources.count(tag) == 0) return;

    material = resources[tag];
  }
};

class MapObj : public Model {
 public:
  std::vector<MapObjSubmesh> meshes;

  ~MapObj() {
    for (auto mapmesh : meshes) {
      mapmesh.mesh.cleargl();
    }
  }

  void init(std::string str, std::map<std::string, Material*> mr) {
    init(str.c_str(), mr);
  }
  void init(const char* filename,
            std::map<std::string, Material*> materialResources) {
    unsigned int count = 0;

    std::vector<glm::vec3> temp_vertices, vertices;
    std::vector<glm::vec3> temp_normals, normals;
    std::vector<glm::vec2> temp_uvs, uvs;
    std::vector<unsigned int> temp_vertexIndices, indices;
    std::vector<unsigned int> temp_normalIndices;
    std::vector<unsigned int> temp_textureIndices;

    unsigned int maxIndV = 0;
    unsigned int maxIndN = 0;
    unsigned int maxIndT = 0;

    bool hasUV = false;

    std::cout << "MapObj: Importing " << filename << "..." << std::endl;
    // load obj file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
      perror("fopen() failed");
      std::cerr << "Cannot open file: " << filename << std::endl;
      return;
      // exit(-1);
    }
    std::cout << "\tLoading map obj visuals...";

    std::string currObjectName = "";
    std::string currMaterial = "None";
    while (!feof(file)) {
      char lineHeader[128];
      // read the first word of the line
      int res = fscanf(file, "%s", lineHeader);

      // else : parse lineHeader
      if (res == EOF || strcmp(lineHeader, "o") == 0) {
        // finished parsing object. treat as sub-mesh
        if (temp_vertices.size() > 0) {
          // post processing submesh
          std::cout << "\tProcessing submesh data:";
          unsigned int n = temp_vertexIndices.size();  // #(triangles)*3
          vertices.resize(n);
          uvs.resize(n);
          normals.resize(n);
          indices.resize(n);
          for (unsigned int i = 0; i < n; i++) {
            indices[i] = i;
            vertices[i] = temp_vertices[(temp_vertexIndices[i] - 1) - maxIndV];
            if (hasUV)
              uvs[i] = temp_uvs[(temp_textureIndices[i] - 1) - maxIndT];
            normals[i] = temp_normals[(temp_normalIndices[i] - 1) - maxIndN];
          }
          std::cout << "[" << currObjectName << "]... ";

          // setting up buffers
          Mesh submesh = Mesh();
          submesh.creategl();
          if (hasUV)
            submesh.bindgl(vertices, normals, uvs, indices);
          else
            submesh.bindgl(vertices, normals, indices);

          // place in mesh container
          MapObjSubmesh sm = MapObjSubmesh();
          sm.name = currObjectName;
          sm.mesh = submesh;
          sm.trySetMaterial(currMaterial, materialResources);

          meshes.push_back(sm);

          std::cout << "done." << std::endl;
          count++;

          maxIndV = *std::max_element(temp_vertexIndices.begin(),
                                      temp_vertexIndices.end());
          maxIndN = *std::max_element(temp_normalIndices.begin(),
                                      temp_normalIndices.end());
          maxIndT = *std::max_element(temp_textureIndices.begin(),
                                      temp_textureIndices.end());

          temp_vertices.clear();
          temp_normals.clear();
          temp_uvs.clear();
          temp_vertexIndices.clear();
          temp_normalIndices.clear();
          temp_textureIndices.clear();
        }
        if (res == EOF) break;  // EOF = End Of File. Quit the loop.

        currObjectName = "";
        char temp_str[60];
        fscanf(file, "%s", temp_str);
        currObjectName = std::string(temp_str);

      } else if (strcmp(lineHeader, "usemtl") == 0) {
        currMaterial = "";
        char temp_str[60];
        fscanf(file, "%s", temp_str);
        currMaterial = std::string(temp_str);
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
        } else {
          fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0],
                 &normalIndex[0], &vertexIndex[1], &normalIndex[1],
                 &vertexIndex[2], &normalIndex[2]);
        }

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

    std::cout << "\tLoaded " << std::to_string(count) << " submeshes total."
              << std::endl;
    std::cout << "MapObj " << filename << " loaded successfully." << std::endl;
  }

  void draw(const glm::mat4& viewProjMtx, const glm::mat4& viewMtx,
            const glm::mat4& transformMtx, const bool ignoreDepth = false) {
    if (!material) return;

    for (auto mapmesh : meshes) {
      Material currMat = *material;

      if (mapmesh.material) {
        currMat = *(mapmesh.material);
      }

      // actiavte the shader program      ---
      glUseProgram(currMat.shader);

      currMat.setUniforms(viewProjMtx, viewMtx, transformMtx * modelMtx);

      glDepthFunc(depthFunction);

      mapmesh.mesh.draw();

      glDepthFunc(GL_LESS);

      // deactivate the shader program    ---
      glUseProgram(0);
    }
  }
};
