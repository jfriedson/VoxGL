#include "model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/config.h>

#include <iostream>


Model::Model(const char* modelFile) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFile,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

	if (!scene) {
		std::cout << "Model " << modelFile << " failed to load: " << importer.GetErrorString() << std::endl;
		exit(1);
	}

	glm::vec3 vmin(FLT_MAX);
	glm::vec3 vmax(-FLT_MAX);

	unsigned int vertexIdx = 0;
	for (unsigned int mesh = 0; mesh < scene->mNumMeshes; mesh++) {
		const aiMesh* model = scene->mMeshes[mesh];

		const aiVector3D aiZeroVector(0.f, 0.f, 0.f);
		for (unsigned int vertex = 0; vertex < model->mNumVertices; vertex++) {
			const aiVector3D& pPos = model->mVertices[vertex];
			glm::vec3 pos = glm::vec3(pPos.x, pPos.y, pPos.z);

			vmin = glm::min(vmin, pos);
			vmax = glm::max(vmax, pos);

			const aiVector3D& pTexCoord = model->HasTextureCoords(0) ? model->mTextureCoords[0][vertex] : aiZeroVector;
			//const aiVector3D& pNormal = model->mNormals[vertex];
			//const aiVector3D* pTangent = &(model->mTangents[vertex]);

			vertices.emplace_back(pos, glm::vec2(pTexCoord.x, -pTexCoord.y));
			//glm::vec3(pNormal.x, pNormal.y, pNormal.z),
			//glm::vec3(pTangent->x, pTangent->y, pTangent->z));
		}

		for (unsigned int i = 0; i < model->mNumFaces; i++) {
			const aiFace& face = model->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(vertexIdx + face.mIndices[0]);
			indices.push_back(vertexIdx + face.mIndices[1]);
			indices.push_back(vertexIdx + face.mIndices[2]);
		}

		vertexIdx = vertices.size();
	}


	glm::vec3 vdiff = 1.f/(vmax - vmin);

	float vdiffmax = fmin(fmin(vdiff.x, vdiff.y), vdiff.z);

	for (unsigned int vertex = 0; vertex < vertices.size(); vertex++)
		vertices[vertex].position = (vertices[vertex].position - vmin) * vdiffmax;


	VAO = 0;
	VBO = 0;
	IBO = 0;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3) * 2 + sizeof(glm::vec2)));
	//glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	std::cout << "loaded model " << modelFile << std::endl;
}

Model::~Model() {
	glBindVertexArray(0);
	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Model::render() {
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}
