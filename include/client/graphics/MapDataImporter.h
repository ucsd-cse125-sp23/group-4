/**************************************************
 * MapDataImporter.h
 * Static class
 * (no GL code used)
 * Processes data from an obj file:
 *  - convex collider data
 *  - spawn points
 *  - item spawn points
 *  - kill plane
 *  - generic data for gameplay/visuals usage
 *    * lights
 *    * physics force hitbox
 *    * effects (sound, particles)
 * Intended for static map geometry and points.
 *
 * usage:
 *   MapData mapData =
 *       MapDataImporter::Import("assets/model/map-data.obj");
 *****************************************************/

#pragma once

#include <stdio.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "client/graphics/MapPointData.h"
#include "core/math/vector.h"

struct MapData {
  std::vector<MapPointData> colliders;
  std::vector<MapPointData> spawnPoints;
  std::vector<MapPointData> itemPoints;

  std::map<std::string, std::vector<MapPointData>> groups;
};

class MapDataImporter {
 public:
  static MapData Import(const char* filename) {
    std::vector<MapPointData> resultColliders;

    std::vector<vec3f> temp_vertices;

    std::cout << "MapDataImporter: Importing " << filename << "..."
              << std::endl;
    // load obj file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
      perror("fopen() failed");
      std::cerr << "Cannot open file: " << filename << std::endl;
      return resultColliders;
    }
    std::cout << "\tLoading object vertex data...";

    std::string currObjectName = "";
    while (!feof(file)) {
      char lineHeader[128];
      // read the first word of the line
      int res = fscanf(file, "%s", lineHeader);
      if (res == EOF) break;  // EOF = End Of File. Quit the loop.

      // else : parse lineHeader
      if (strcmp(lineHeader, "o") == 0) {
        // separate object. this line is the beginning of a new object

        if (temp_vertices.size() > 0) {
          MapPointData c = MapPointData(temp_vertices);
          c.name = std::string(currObjectName);
          resultColliders.push_back(c);

          temp_vertices.clear();
        }

        currObjectName = "";
        char temp_str[60];
        fscanf(file, "%s", temp_str);
        currObjectName = std::string(temp_str);
      } else if (strcmp(lineHeader, "v") == 0) {
        vec3f vertex;
        fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
        temp_vertices.push_back(vertex);
      } else if (strcmp(lineHeader, "vn") == 0) {
        vec3f normal;
        fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      } else if (strcmp(lineHeader, "f") == 0) {
        // std::string vertex1, vertex2, vertex3;
        unsigned int vertexIndex[3], normalIndex[3];
        fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0],
               &vertexIndex[1], &normalIndex[1], &vertexIndex[2],
               &normalIndex[2]);
      }
    }
    std::cout << "done." << std::endl;

    fclose(file);

    std::cout << "Map Data " << filename << " loaded successfully."
              << std::endl;

    return result;
  }
};
