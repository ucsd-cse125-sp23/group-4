#include "SkinnedMesh.h"

using namespace glm;

void SkinnedMesh::init(const char* filename) {
  // check filetype. for now just pass to LoadCustom()
  // TODO: use ASSIMP here if .fbx extension detected
  LoadCustom(filename);
}

void SkinnedMesh::updateSkin(Skeleton* skel) {
  if (!skel) return;

  // Update positions based on joints, bindings, and weights
  for (int i = 0; i < vertices_curr.size(); i++) {
    mat4 sum = mat4(0.0f);

    for (int j = 0; j < skinweights[i].size(); j++) {
      int jointIndex = skinweights[i][j].first;
      mat4 W = skel->GetWorldMatrix(jointIndex);
      mat4 B = bindings[jointIndex];

      sum += skinweights[i][j].second * W * inverse(B);
    }

    vertices_curr[i] = vec3(sum * vec4(vertices_bind[i], 1));
  }
}

bool SkinnedMesh::LoadCustom(const char* file) {
  // uses custom filetype
  Tokenizer t;
  bool success = t.Open(file);

  if (!success) {
    std::cerr << "Cannot open file: " << file << std::endl;
    return false;
  }

  t.FindToken("positions");
  int numverts = t.GetInt();
  vertices_bind.reserve(numverts);
  vertices_curr.reserve(numverts);
  t.FindToken("{");
  for (int i = 0; i < numverts; i++) {
    glm::vec3 p;
    p.x = t.GetFloat();
    p.y = t.GetFloat();
    p.z = t.GetFloat();

    vertices_bind.push_back(p);
    vertices_curr.push_back(p);
  }
  t.FindToken("}");

  t.FindToken("normals");
  numverts = t.GetInt();
  normals.reserve(numverts);
  t.FindToken("{");
  for (int i = 0; i < numverts; i++) {
    glm::vec3 n;
    n.x = t.GetFloat();
    n.y = t.GetFloat();
    n.z = t.GetFloat();

    normals.push_back(n);
  }
  t.FindToken("}");

  t.FindToken("skinweights");
  numverts = t.GetInt();
  skinweights.reserve(numverts);
  t.FindToken("{");
  for (int i = 0; i < numverts; i++) {
    int numweights = t.GetInt();
    std::vector<std::pair<int, float>> vweights;
    for (int k = 0; k < numweights; k++) {
      int j = t.GetInt();      // joint
      float w = t.GetFloat();  // weight

      vweights.push_back(std::make_pair(j, w));
    }
    skinweights.push_back(vweights);
  }
  t.FindToken("}");

  t.FindToken("triangles");
  int numtris = t.GetInt();
  indices.reserve(numtris * 3);
  t.FindToken("{");
  for (int i = 0; i < numtris; i++) {
    int i0 = t.GetInt();
    int i1 = t.GetInt();
    int i2 = t.GetInt();

    indices.push_back(i0);
    indices.push_back(i1);
    indices.push_back(i2);
  }
  t.FindToken("}");

  t.FindToken("bindings");
  int numbinds = t.GetInt();
  bindings.reserve(numbinds);
  t.FindToken("{");
  for (int i = 0; i < numbinds; i++) {
    t.FindToken("matrix");
    t.FindToken("{");
    float x0 = t.GetFloat();
    float x1 = t.GetFloat();
    float x2 = t.GetFloat();
    float y0 = t.GetFloat();
    float y1 = t.GetFloat();
    float y2 = t.GetFloat();
    float z0 = t.GetFloat();
    float z1 = t.GetFloat();
    float z2 = t.GetFloat();
    float w0 = t.GetFloat();
    float w1 = t.GetFloat();
    float w2 = t.GetFloat();

    glm::mat4 b(1.0f);
    // columns
    b[0] = glm::vec4(x0, x1, x2, 0);
    b[1] = glm::vec4(y0, y1, y2, 0);
    b[2] = glm::vec4(z0, z1, z2, 0);
    b[3] = glm::vec4(w0, w1, w2, 1);

    bindings.push_back(b);
    t.FindToken("}");
  }
  t.FindToken("}");

  vertices_bind.resize(numverts);
  normals.resize(numverts);
  indices.resize(numtris * 3);

  vertices_curr.resize(numverts);

  skinweights.resize(numverts);

  bindings.resize(numbinds);

  // Finish
  t.Close();

  std::cerr << "Success Tokenizing..." << file << std::endl;

  return true;
}
