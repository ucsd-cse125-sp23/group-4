/**************************************************
 * ColliderImporter.h
 * Static class
 * Processes collider data from an obj file.
 * Used for static map geometry.
 * Supports multiple separate CONVEX objects.
 *   usage:
 *   std::vector<Collider> mapColliders =
 *       ColliderImporter::ImportCollisionData("assets/models/file.obj");
 *****************************************************/

#pragma once

#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>

#include "client/graphics/ColliderData.h"
#include "core/math/vector.h"

class ColliderImporter {
 public:
  static std::vector<ColliderData> ImportCollisionData(const char* filename) {
    std::vector<ColliderData> result;

    std::vector<vec3f> temp_vertices, vertices;
    // std::vector< glm::vec3 > temp_normals, normals;
    // std::vector< unsigned int > temp_vertexIndices, indices;
    // std::vector< unsigned int > temp_normalIndices;

    std::cout << "ColliderImporter: Importing " << filename << "..."
              << std::endl;
    // load obj file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
      perror("fopen() failed");
      std::cerr << "Cannot open file: " << filename << std::endl;
      return result;
    }
    std::cout << "\tLoading collider vertex data...";

    std::string currColName = "";
    while (!feof(file)) {
      char lineHeader[128];
      // read the first word of the line
      int res = fscanf(file, "%s", lineHeader);
      if (res == EOF) break;  // EOF = End Of File. Quit the loop.

      // else : parse lineHeader
      if (strcmp(lineHeader, "o") == 0) {
        // separate object. for collision we need to separate these

        if (temp_vertices.size() > 0) {
          ColliderData c = ColliderData(temp_vertices);
          c.name = std::string(currColName);
          result.push_back(c);

          temp_vertices.clear();
        }

        currColName = "";
        char temp_str[60];
        fscanf(file, "%s", temp_str);
        currColName = std::string(temp_str);
      } else if (strcmp(lineHeader, "v") == 0) {
        vec3f vertex;
        fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
        temp_vertices.push_back(vertex);
      } else if (strcmp(lineHeader, "vn") == 0) {
        vec3f normal;
        fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
        // temp_normals.push_back(normal);
      } else if (strcmp(lineHeader, "f") == 0) {
        // std::string vertex1, vertex2, vertex3;
        unsigned int vertexIndex[3], normalIndex[3];
        fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0],
               &vertexIndex[1], &normalIndex[1], &vertexIndex[2],
               &normalIndex[2]);
        /*
        temp_vertexIndices.push_back(vertexIndex[0]);
        temp_vertexIndices.push_back(vertexIndex[1]);
        temp_vertexIndices.push_back(vertexIndex[2]);
        temp_normalIndices.push_back(normalIndex[0]);
        temp_normalIndices.push_back(normalIndex[1]);
        temp_normalIndices.push_back(normalIndex[2]);
        */
      }
    }
    std::cout << "done." << std::endl;

    fclose(file);

    std::cout << "Collider geometry " << filename << " loaded successfully."
              << std::endl;

    return result;
  }
};
