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
  float fallBoundY = -100;

  std::map<std::string, std::vector<MapPointData>> groups;
};

class MapDataImporter {
 public:
  static MapData Import(std::string fn) { return Import(fn.c_str()); }
  static MapData Import(const char* filename) {
    MapData result = MapData();
    unsigned int count = 0;

    std::vector<vec3f> temp_vertices;

    std::cout << "MapDataImporter: Importing " << filename << "..."
              << std::endl;
    // load obj file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
      perror("fopen() failed");
      std::cerr << "Cannot open file: " << filename << std::endl;
      return result;
    }
    std::cout << "\tLoading object vertex data...";

    std::string currObjectName = "";
    std::string currPrefix = "";
    std::string currName = "obj";
    while (!feof(file)) {
      char lineHeader[128];
      // read the first word of the line
      int res = fscanf(file, "%s", lineHeader);

      // parse lineHeader
      if (res == EOF || strcmp(lineHeader, "o") == 0) {
        // moving on to next object now, save the current one:

        if (temp_vertices.size() > 0) {
          MapPointData c = MapPointData(temp_vertices);
          c.name = std::string(currName);

          // place in correct container...!
          if (currPrefix.length() == 0) {
            result.colliders.push_back(c);
          } else if (strcmp(currPrefix.c_str(), "plyr") == 0) {
            result.spawnPoints.push_back(c);
          } else if (strcmp(currPrefix.c_str(), "item") == 0) {
            result.itemPoints.push_back(c);
          } else if (strcmp(currPrefix.c_str(), "fall") == 0) {
            result.fallBoundY = c.point.y;
          } else {
            result.groups[currPrefix].push_back(c);
          }
          count++;

          temp_vertices.clear();
        }
        if (res == EOF) break;  // EOF = End Of File. Quit the loop.

        currObjectName = "";
        char temp_str[60];
        fscanf(file, "%s", temp_str);
        currObjectName = std::string(temp_str);

        // expected format: "_plyr_homepoint"
        currPrefix = "";
        currName = currObjectName;
        if (currObjectName.length() >= 3 &&
            currObjectName.at(0) == '_') {  // check first char
          size_t found = currObjectName.find('_', 1);
          if (found != std::string::npos) {
            currPrefix = currObjectName.substr(1, found - 1);
            currName = currObjectName.substr(found + 1);
            if (currName.length() == 0) currName = "unnamed";
          }
        }

      } else if (strcmp(lineHeader, "v") == 0) {
        // this line is a single vertex:

        vec3f vertex;
        fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
        temp_vertices.push_back(vertex);

      } else if (strcmp(lineHeader, "vn") == 0) {
        // ignore line
        vec3f normal;
        fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      } else if (strcmp(lineHeader, "f") == 0) {
        // ignore line
        unsigned int vertexIndex[3], normalIndex[3];
        fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0],
               &vertexIndex[1], &normalIndex[1], &vertexIndex[2],
               &normalIndex[2]);
      }
    }
    std::cout << "done." << std::endl;

    fclose(file);

    std::cout << "\tLoaded " << std::to_string(count) << " map objects total."
              << std::endl;
    std::cout << "Map Data " << filename << " loaded successfully."
              << std::endl;

    return result;
  }
};
