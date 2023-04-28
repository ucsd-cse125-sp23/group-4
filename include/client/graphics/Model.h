/**************************************************
A model is a mesh with material (and more?? animation skeleton???)
*****************************************************/
#include "client/graphics/Mesh.h"
#include "client/graphics/Material.h"

#ifndef __MODEL_H__
#define __MODEL_H__

struct Model {
    glm::mat4 transformMtx; // applied first!

    Mesh* mesh;
    Material* material;
    // TODO, consider adding skin stuff here

    void draw(const glm::mat4& viewProjMtx, const glm::mat4& modelMtx, const bool ignoreDepth = false)
    {
        if (!material || !mesh)
            return;

        GLuint shader = material->shader;

        // actiavte the shader program      ---
        glUseProgram(shader);

        material->setUniforms(viewProjMtx, modelMtx * transformMtx);

        if (ignoreDepth) glDisable(GL_DEPTH_TEST);

        mesh->draw();

        if (ignoreDepth) glEnable(GL_DEPTH_TEST);

        // deactivate the shader program    ---
        glUseProgram(0);
    }
};

#endif 
