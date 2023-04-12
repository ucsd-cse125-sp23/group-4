#include "Skin.h"


bool Skin::Load(const char* file) {
	Tokenizer t;
	bool success = t.Open(file);

	if (!success) return false;

	t.FindToken("positions");
	int numverts = t.GetInt();
	positions.reserve(numverts);
	initpositions.reserve(numverts);
	t.FindToken("{");
	for (int i = 0; i < numverts; i++) {
		glm::vec3 p;
		p.x = t.GetFloat();
		p.y = t.GetFloat();
		p.z = t.GetFloat();

		positions.push_back(p);
		initpositions.push_back(p);
	}
	t.FindToken("}");

	t.FindToken("normals");
	numverts = t.GetInt();
	normals.reserve(numverts);
	initnormals.reserve(numverts);
	t.FindToken("{");
	for (int i = 0; i < numverts; i++) {
		glm::vec3 n;
		n.x = t.GetFloat();
		n.y = t.GetFloat();
		n.z = t.GetFloat();

		normals.push_back(n);
		initnormals.push_back(n);
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
			int j = t.GetInt();		// joint
			float w = t.GetFloat();	// weight

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

	positions.resize(numverts);
	normals.resize(numverts);
	indices.resize(numtris * 3);

	initpositions.resize(numverts);
	initnormals.resize(numverts);

	skinweights.resize(numverts);

	bindings.resize(numbinds);

	// Finish
	t.Close();

	return true;
}

void Skin::Update(Skeleton* skel)
{
	if (!skel) return;

	// Update positions based on joints, bindings, and weights
	for (int i = 0; i < positions.size(); i++) {
		mat4 sum = mat4(0.0f);

		for (int j = 0; j < skinweights[i].size(); j++) {
			int jointIndex = skinweights[i][j].first;
			mat4 W = skel->GetWorldMatrix(jointIndex);
			mat4 B = bindings[jointIndex];

			sum += skinweights[i][j].second * W * inverse(B);
		}
		
		positions[i] = vec3(sum * vec4(initpositions[i], 1));
	}
}

void Skin::Draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	// actiavte the shader program 
	glUseProgram(shader);

	// get the locations and send the uniforms to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&glm::mat4(1.0f));
	glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

	// Bind the VAO
	glBindVertexArray(VAO);

	//
	// Bind to the first VBO - We will use it to store the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO - We will use it to store the normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind the EBO to the bound VAO and send the data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//

	// draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}
