#pragma once

#include "core.h"

#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

class AssimpMesh {
public:
    AssimpMesh();

    void loadFromAssimp(aiMesh *mesh, const aiScene *scene);
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void gl_load();
    void gl_delete();
private:
    unsigned int VAO, VBO, EBO;
    bool isLoaded;
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
};