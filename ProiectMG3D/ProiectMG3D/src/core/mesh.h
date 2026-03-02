#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <assimp/scene.h>

#include "core/vertex_data.h"
#include "core/material.h"

#define INVALID 123456789

class MeshEntry
{
public:
	MeshEntry() {
		nrIndices = 0;
		baseVertex = 0;
		baseIndex = 0;
		materialIndex = INVALID;
	}

	unsigned int nrIndices;
	unsigned int baseVertex;
	unsigned int baseIndex;
	unsigned int materialIndex;
};

class Mesh
{
private:
	std::string meshID;
	bool isImported = false;

	std::vector<MeshEntry> meshEntries;
	std::vector<Material *> materials;

	bool InitScene(const aiScene* scene);
	void InitMaterials(const aiScene* scene);
	void InitMesh(const aiMesh* mesh);

	void RenderSimple();
	void RenderImported();

public:
	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;
	// For imported objects
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	GLuint meshVAO;
	GLuint meshVBO[3];
	unsigned int VBOcount;
	GLuint meshEBO;

	GLenum drawMode;

	std::string directory;
	std::string file;

	Mesh(std::string meshID);
	virtual ~Mesh();

	bool Init(std::vector<VertexData>& vertices, std::vector<unsigned int>& indices);
	bool InitImported();
	bool Load(std::string directory, std::string file);
	void Render();

	std::string GetMeshID();
	bool IsImported();
};