#ifndef MESH_FACTORY_HPP
#define MESH_FACTORY_HPP
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/Scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <gl/glew.h>
#include <map>
#include "texture.hpp"

static glm::mat4 ai_to_glm4(const aiMatrix4x4& mat)
{
	return glm::mat4(
		mat.a1, mat.a2, mat.a3, mat.a4,
		mat.b1, mat.b2, mat.b3, mat.b4,
		mat.c1, mat.c2, mat.c3, mat.c4,
		mat.d1, mat.d2, mat.d3, mat.d4);
}

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define INVALID_MATERIAL 0xFFFFFFFF
#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

enum VB_TYPES {
	INDEX_BUFFER,
	POS_VB,
	NORMAL_VB,
	TEXCOORD_VB,
	BONE_VB,
	NUM_VBs
};

struct MeshEntry
{
	MeshEntry()
	{
		NumIndices = 0;
		BaseVertex = 0;
		BaseIndex = 0;
		MaterialIndex = INVALID_MATERIAL;
	}

	unsigned int NumIndices;
	unsigned int BaseVertex;
	unsigned int BaseIndex;
	unsigned int MaterialIndex;
};

struct BoneInfo
{
	glm::mat4 BoneOffset;
	glm::mat4 FinalTransformation;

	BoneInfo()
	{
		BoneOffset = glm::mat4(1.f); // SET ZERO?
		FinalTransformation = glm::mat4(1.f);
	}
};

struct Edge
{
	Edge(unsigned int _a, unsigned int _b)
	{
		assert(_a != _b);

		if (_a < _b)
		{
			a = _a;
			b = _b;
		}
		else
		{
			a = _b;
			b = _a;
		}
	}

	void Print()
	{
		printf("Edge %d %d\n", a, b);
	}

	unsigned int a;
	unsigned int b;
};

struct Face
{
	unsigned int Indices[3];

	unsigned int GetOppositeIndex(const Edge& e) const
	{
		for (auto i = 0u; i < ARRAY_SIZE_IN_ELEMENTS(Indices); i++) {
			unsigned int Index = Indices[i];

			if (Index != e.a && Index != e.b) {
				return Index;
			}
		}

		assert(0);

		return 0;
	}
};

struct Neighbors
{
	unsigned int n1;
	unsigned int n2;

	Neighbors()
	{
		n1 = n2 = (unsigned int)-1;
	}

	void AddNeigbor(unsigned int n)
	{
		if (n1 == -1) {
			n1 = n;
		}
		else if (n2 == -1) {
			n2 = n;
		}
		else {
			assert(0);
		}
	}

	unsigned int GetOther(unsigned int me) const
	{
		if (n1 == me) {
			return n2;
		}
		else if (n2 == me) {
			return n1;
		}
		else {
			assert(0);
		}

		return 0;
	}
};

struct CompareEdges
{
	bool operator()(const Edge& Edge1, const Edge& Edge2)
	{
		if (Edge1.a < Edge2.a) {
			return true;
		}
		else if (Edge1.a == Edge2.a) {
			return (Edge1.b < Edge2.b);
		}
		else {
			return false;
		}
	}
};


struct CompareVectors
{
	bool operator()(const glm::vec3& a, const glm::vec3& b)
	{
		if (a.x < b.x) {
			return true;
		}
		else if (a.x == b.x) {
			if (a.y < b.y) {
				return true;
			}
			else if (a.y == b.y) {
				if (a.z < b.z) {
					return true;
				}
			}
		}

		return false;
	}
};

class mesh
{
private:
	static const unsigned int NUM_BONES_PER_VERTEX = 4;
public:
	struct VertexBoneData
	{
		unsigned int IDs[NUM_BONES_PER_VERTEX];
		float Weights[NUM_BONES_PER_VERTEX];
		void AddBoneData(unsigned int BoneID, float Weight)
		{
			for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++) {
				if (Weights[i] == 0.0) {
					IDs[i] = BoneID;
					Weights[i] = Weight;
					return;
				}
			}
			// should never get here - more bones than we have space for
			assert(0);
		}
	};

public:

	bool load_mesh(const std::string& Filename)
	{
		// Release the previously loaded mesh (if it exists)
		//Clear();

		// Create the VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Create the buffers for the vertices attributes
		glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(buffers), buffers);

		bool Ret = false;

		scene = importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
			aiProcess_FlipUVs);

		if (scene)
		{
			global_inverse_transform = ai_to_glm4(scene->mRootNode->mTransformation);

			glm::inverse(global_inverse_transform);
			Ret = init_from_scene(scene, Filename);
		}
		else {
			printf("Error parsing '%s': '%s'\n", Filename.c_str(), importer.GetErrorString());
		}

		// Make sure the VAO is not changed from the outside
		glBindVertexArray(0);

		return Ret;
	}

	void FindAdjacencies(const aiMesh* paiMesh, std::vector<unsigned int>& Indices)
	{
		// Step 1 - find the two triangles that share every edge
		for (auto i = 0u; i < paiMesh->mNumFaces; i++) {
			const aiFace& face = paiMesh->mFaces[i];

			Face Unique;

			// If a position vector is duplicated in the VB we fetch the 
			// index of the first occurrence.
			for (auto j = 0u; j < 3; j++) {
				unsigned int Index = face.mIndices[j];
				glm::vec3 v = glm::vec3(paiMesh->mVertices[Index].x, paiMesh->mVertices[Index].y, paiMesh->mVertices[Index].z);

				if (m_posMap.find(v) == m_posMap.end()) {
					m_posMap[v] = Index;
				}
				else {
					Index = m_posMap[v];
				}

				Unique.Indices[j] = Index;
			}

			m_uniqueFaces.push_back(Unique);

			Edge e1(Unique.Indices[0], Unique.Indices[1]);
			Edge e2(Unique.Indices[1], Unique.Indices[2]);
			Edge e3(Unique.Indices[2], Unique.Indices[0]);

			m_indexMap[e1].AddNeigbor(i);
			m_indexMap[e2].AddNeigbor(i);
			m_indexMap[e3].AddNeigbor(i);
		}

		// Step 2 - build the index buffer with the adjacency info
		for (auto i = 0u; i < paiMesh->mNumFaces; i++) {
			const Face& face = m_uniqueFaces[i];

			for (auto j = 0u; j < 3; j++) {
				Edge e(face.Indices[j], face.Indices[(j + 1) % 3]);
				assert(m_indexMap.find(e) != m_indexMap.end());
				Neighbors n = m_indexMap[e];
				unsigned int OtherTri = n.GetOther(i);

				assert(OtherTri != -1);

				const Face& OtherFace = m_uniqueFaces[OtherTri];
				unsigned int OppositeIndex = OtherFace.GetOppositeIndex(e);

				Indices.push_back(face.Indices[j]);
				Indices.push_back(OppositeIndex);
			}
		}
	}

	void LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
	{
		for (unsigned int i = 0; i < pMesh->mNumBones; i++) {
			unsigned int BoneIndex = 0;
			std::string BoneName(pMesh->mBones[i]->mName.data);

			if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
				// Allocate an index for a new bone
				BoneIndex = m_NumBones;
				m_NumBones++;
				BoneInfo bi;
				m_BoneInfo.push_back(bi);
				m_BoneInfo[BoneIndex].BoneOffset = ai_to_glm4(pMesh->mBones[i]->mOffsetMatrix);
				m_BoneMapping[BoneName] = BoneIndex;
			}
			else {
				BoneIndex = m_BoneMapping[BoneName];
			}

			for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
				unsigned int VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
				Bones[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}
	}

	void init_mesh(unsigned int MeshIndex,
		const aiMesh* paiMesh,
		std::vector<glm::vec3>& Positions,
		std::vector<glm::vec3>& Normals,
		std::vector<glm::vec2>& TexCoords,
		std::vector<VertexBoneData>& Bones,
		std::vector<unsigned int>& Indices)
	{
		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		// Populate the vertex attribute vectors
		for (auto i = 0u; i < paiMesh->mNumVertices; i++) {
			const aiVector3D* pPos = &(paiMesh->mVertices[i]);
			const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
			const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

			Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
			Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
			TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
		}

		LoadBones(MeshIndex, paiMesh, Bones);

		if (m_withAdjacencies) {
			FindAdjacencies(paiMesh, Indices);
		}
		else {
			// Populate the index buffer
			for (auto i = 0u; i < paiMesh->mNumFaces; i++) {
				const aiFace& Face = paiMesh->mFaces[i];
				assert(Face.mNumIndices == 3);
				Indices.push_back(Face.mIndices[0]);
				Indices.push_back(Face.mIndices[1]);
				Indices.push_back(Face.mIndices[2]);
			}
		}
	}


	bool InitMaterials(const aiScene* pScene, const std::string& Filename)
	{
		// Extract the directory part from the file name
		std::string::size_type SlashIndex = Filename.find_last_of("/");
		std::string Dir;

		if (SlashIndex == std::string::npos) {
			Dir = ".";
		}
		else if (SlashIndex == 0) {
			Dir = "/";
		}
		else {
			Dir = Filename.substr(0, SlashIndex);
		}

		bool Ret = true;

		// Initialize the materials
		for (auto i = 0u; i < pScene->mNumMaterials; i++) {
			const aiMaterial* pMaterial = pScene->mMaterials[i];

			m_Textures[i] = NULL;

			if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
				aiString Path;

				if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
					std::string p(Path.data);

					if (p.substr(0, 2) == ".\\") {
						p = p.substr(2, p.size() - 2);
					}

					std::string FullPath = Dir + "/" + p;

					m_Textures[i] = new texture(FullPath.c_str());
				}
			}
		}

		return Ret;
	}

	bool init_from_scene(const aiScene* pScene, const std::string& Filename)
	{
		m_Entries.resize(pScene->mNumMeshes);
		m_Textures.resize(pScene->mNumMaterials);

		std::vector<glm::vec3> Positions;
		std::vector<glm::vec3> Normals;
		std::vector<glm::vec2> TexCoords;
		std::vector<VertexBoneData> Bones;
		std::vector<unsigned int> Indices;

		unsigned int NumVertices = 0;
		unsigned int NumIndices = 0;

		unsigned int VerticesPerPrim = m_withAdjacencies ? 6 : 3;

		// Count the number of vertices and indices
		for (auto i = 0u; i < m_Entries.size(); i++) {
			m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
			m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * VerticesPerPrim;
			m_Entries[i].BaseVertex = NumVertices;
			m_Entries[i].BaseIndex = NumIndices;

			NumVertices += pScene->mMeshes[i]->mNumVertices;
			NumIndices += m_Entries[i].NumIndices;
		}

		// Reserve space in the vectors for the vertex attributes and indices
		Positions.reserve(NumVertices);
		Normals.reserve(NumVertices);
		TexCoords.reserve(NumVertices);
		Bones.resize(NumVertices);
		Indices.reserve(NumIndices);

		// Initialize the meshes in the scene one by one
		for (auto i = 0u; i < m_Entries.size(); i++) {
			const aiMesh* paiMesh = pScene->mMeshes[i];
			init_mesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
		}

		if (!InitMaterials(pScene, Filename)) {
			return false;
		}

		// Generate and populate the buffers with vertex attributes and the indices
		glBindBuffer(GL_ARRAY_BUFFER, buffers[POS_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(POSITION_LOCATION);
		glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[TEXCOORD_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(TEX_COORD_LOCATION);
		glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(NORMAL_LOCATION);
		glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[BONE_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(BONE_ID_LOCATION);
		glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
		glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
		glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

		return true;
	}

	void Render()
	{
		glBindVertexArray(vao);

		unsigned int Topology = m_withAdjacencies ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;

		for (unsigned int i = 0; i < m_Entries.size(); i++) {
			const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

			assert(MaterialIndex < m_Textures.size());

			if (m_Textures[MaterialIndex]) {
				m_Textures[MaterialIndex]->bind_to_buffer();
				//m_Textures[MaterialIndex]->Bind(COLOR_TEXTURE_UNIT);
			}

			glDrawElementsBaseVertex(Topology,
				m_Entries[i].NumIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
				m_Entries[i].BaseVertex);
		}

		// Make sure the VAO is not changed from the outside    
		glBindVertexArray(0);
	}
private:
	unsigned int vao;
	unsigned int buffers[NUM_VBs];

	std::vector<MeshEntry> m_Entries;
	std::vector<texture*> m_Textures;

	bool m_withAdjacencies;
	std::map<std::string, unsigned int> m_BoneMapping;
	std::map<Edge, Neighbors, CompareEdges> m_indexMap;
	std::map<glm::vec3, unsigned int, CompareVectors> m_posMap;
	std::vector<Face> m_uniqueFaces;
	unsigned int m_NumBones;
	std::vector<BoneInfo> m_BoneInfo;

	const aiScene* scene;
	Assimp::Importer importer;
	glm::mat4 global_inverse_transform;
};

class mesh_factory
{
public:

private:


};

#endif