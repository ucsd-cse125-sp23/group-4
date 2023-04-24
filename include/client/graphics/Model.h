/**************************************************
A model is a mesh with material (and more?? animation skeleton???)
*****************************************************/
#include "Mesh.h"
#include "Material.h"

#ifndef __MODEL_H__
#define __MODEL_H__

struct Model {
    glm::mat4 transformMtx; // applied first!

    Mesh* mesh;
    Material* material;
    // TODO, consider adding skin stuff here

    virtual void draw(const glm::mat4& viewProjMtx, const glm::mat4& modelMtx)
    {
        if (!material || !mesh)
            return;

        GLuint shader = material->shader;

        // actiavte the shader program      ---
        glUseProgram(shader);

        material->setUniforms(viewProjMtx, modelMtx * transformMtx);

        mesh->draw();

        // deactivate the shader program    ---
        glUseProgram(0);
    }
};

#endif 
