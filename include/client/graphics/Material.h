/**************************************************
 * Material.h
 * Contains info about how to render the surface of a
 * given mesh
 *****************************************************/
#define GLM_FORCE_RADIANS
#include "client/graphics/Texture.h"
#include "client/graphics/core.h"

#ifndef __MATERIAL_H__
#define __MATERIAL_H__

struct Material {
  GLuint shader;  // points to the shader program we want to use

  Texture* texture = nullptr;  // optional

  glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  glm::vec4 diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);  // <- only one rn
  glm::vec4 specular = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  glm::vec4 emision = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  float shininess = 10.0f;

  void setUniforms(const glm::mat4& viewProjMtx, const glm::mat4& model) {
    // TODO: optimize this glGetLocation call out to an init func?
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false,
                       (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE,
                       (float*)&model);

    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &diffuse[0]);

    if (texture) {
      texture->bindgl();
      glUniform1i(glGetUniformLocation(shader, "gSampler"), 0);
    }

    // TODO: implement other shader cases + their uniform vars here! vvv

    /*
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(modelview_loc, 1, GL_FALSE, &modelview[0][0]);
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &projection[0][0]);
    glUniform4fv( ambient_loc  , 1, &(material -> ambient[0])  );
    glUniform4fv( diffuse_loc  , 1, &(material -> diffuse[0])  );
    glUniform4fv( specular_loc , 1, &(material -> specular[0]) );
    glUniform4fv( emision_loc  , 1, &(material -> emision[0])  );
    glUniform1fv( shininess_loc, 1, &(material -> shininess)   );
    glUniform1i( enablelighting_loc, enablelighting );
    glUniform1i( nlights_loc, nlights );
    glUniform4fv( lightpositions_loc, GLsizei(nlights), &lightpositions[0][0] );
    glUniform4fv( lightcolors_loc, GLsizei(nlights), &lightcolors[0][0] );
    */
  }
};

#endif
