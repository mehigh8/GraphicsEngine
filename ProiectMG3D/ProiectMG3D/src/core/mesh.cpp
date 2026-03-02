#include "core/mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>

Mesh::Mesh(std::string meshID)
{
	this->meshID = std::move(meshID);
	drawMode = GL_TRIANGLES;
	meshVAO = 0;
	VBOcount = 0;
	isImported = false;
}

Mesh::~Mesh()
{
	vertices.clear();
	indices.clear();
	positions.clear();
	normals.clear();
	texCoords.clear();
	meshEntries.clear();
	materials.clear();

	glDeleteVertexArrays(1, &meshVAO);
	if (VBOcount > 0)
		glDeleteBuffers(VBOcount, meshVBO);
	glDeleteBuffers(1, &meshEBO);
}

bool Mesh::Init(std::vector<VertexData>& vertices, std::vector<unsigned int>& indices)
{
	this->vertices = vertices;
	this->indices = indices;

	glGenVertexArrays(1, &meshVAO);
	glGenBuffers(1, meshVBO);
	VBOcount = 1;
	glGenBuffers(1, &meshEBO);

	glBindVertexArray(meshVAO);

	glBindBuffer(GL_ARRAY_BUFFER, meshVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)(sizeof(glm::vec3)));
	// texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(glm::vec3) * 2));
	// color
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(glm::vec3) * 2 + sizeof(glm::vec2)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	
	return meshVAO != 0;
}

bool Mesh::Load(std::string directory, std::string file)
{
	isImported = true;
	this->directory = directory;
	this->file = file;
	std::string path = directory + "/" + file;

	Assimp::Importer importer;

	unsigned int flags = aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_Triangulate;

	const aiScene* scene = importer.ReadFile(path, flags);

	if (scene) {
		return InitScene(scene);
	}

	std::cout << "ERROR: Failed to load object from: " << path << std::endl;
	return false;
}

bool Mesh::InitScene(const aiScene* scene)
{
	unsigned int nrVertices = 0;
	unsigned int nrIndices = 0;

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		MeshEntry entry;
		entry.materialIndex = scene->mMeshes[i]->mMaterialIndex;
		entry.nrIndices = scene->mMeshes[i]->mNumFaces * 3;
		entry.baseVertex = nrVertices;
		entry.baseIndex = nrIndices;

		nrVertices += scene->mMeshes[i]->mNumVertices;
		nrIndices += entry.nrIndices;

		meshEntries.push_back(entry);
	}

	for (unsigned int i = 0; i < meshEntries.size(); i++)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		InitMesh(mesh);
	}

	InitMaterials(scene);

	InitImported();

	return true;
}

void Mesh::InitMesh(const aiMesh* mesh)
{
	const aiVector3D zero(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(mesh->mVertices[i]);
		const aiVector3D* pNormal = &(mesh->mNormals[i]);
		const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &zero;

		positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		texCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace& face = mesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
}

void Mesh::InitMaterials(const aiScene* scene)
{
	aiColor4D color;

	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* material = scene->mMaterials[i];
		Material* localMaterial = new Material();

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				localMaterial->texture = new Texture("tex" + i);
				localMaterial->texture->LoadTexture((directory + "/" + Path.data).c_str(), false);
			}
		}

		if (aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color) == AI_SUCCESS)
			localMaterial->ambient = glm::vec4(color.r, color.g, color.b, color.a);

		if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
			localMaterial->diffuse = glm::vec4(color.r, color.g, color.b, color.a);

		if (aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color) == AI_SUCCESS)
			localMaterial->specular = glm::vec4(color.r, color.g, color.b, color.a);

		if (aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &color) == AI_SUCCESS)
			localMaterial->emissive = glm::vec4(color.r, color.g, color.b, color.a);

		materials.push_back(localMaterial);
	}
}

bool Mesh::InitImported()
{
	glGenVertexArrays(1, &meshVAO);
	glGenBuffers(3, meshVBO);
	VBOcount = 3;
	glGenBuffers(1, &meshEBO);

	glBindVertexArray(meshVAO);

	// position
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// normal
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// texture coords
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	return meshVAO != 0;
}

void Mesh::Render()
{
	if (isImported)
		RenderImported();
	else
		RenderSimple();
}

void Mesh::RenderSimple()
{
	glBindVertexArray(meshVAO);
	glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::RenderImported()
{
	glBindVertexArray(meshVAO);
	
	for (int i = 0; i < meshEntries.size(); i++)
	{
		if (meshEntries[i].materialIndex != INVALID && materials[meshEntries[i].materialIndex]->texture)
		{
			materials[meshEntries[i].materialIndex]->texture->BindToActiveTexture(GL_TEXTURE0);
		}

		glDrawElementsBaseVertex(drawMode, meshEntries[i].nrIndices,
			GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * meshEntries[i].baseIndex),
			meshEntries[i].baseVertex);
	}

	glBindVertexArray(0);
}

std::string Mesh::GetMeshID()
{
	return meshID;
}

bool Mesh::IsImported()
{
	return isImported;
}
