#include "AssimpMesh.h"

AssimpMesh::AssimpMesh() : isLoaded(false) { }

void AssimpMesh::loadFromAssimp(aiMesh *mesh, const aiScene *scene) {
    // Is it possible to use vector.resize() & index?

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x,
                                    mesh->mVertices[i].y,
                                    mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x,
                                  mesh->mNormals[i].y,
                                  mesh->mNormals[i].z);
        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace f = mesh->mFaces[i];
        for(unsigned int j = 0; j < f.mNumIndices; j++) {
            indices.push_back(f.mIndices[j]);
        }
    }
}

void AssimpMesh::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    if(!isLoaded) {
        gl_load();
    }
    glm::mat4 m4(1.0f);
    glm::vec3 color(1.0f, 1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&m4);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void AssimpMesh::gl_load() {
    if(isLoaded) {
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);
    isLoaded = true;
}

void AssimpMesh::gl_delete() {
    if(!isLoaded) {
        return;
    }

    // TODO:
}