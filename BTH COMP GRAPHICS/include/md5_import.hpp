#ifndef MD5_IMPORT_HPP
#define MD5_IMPORT_HPP
#include <string>
#include <glm/glm.hpp>

namespace md5
{
	struct MD5Tri
	{
		int triIndex;	//Which triangle of the mesh this is
		unsigned int a;	//Index to first vertex
		unsigned int b;	//Index to second vertex
		unsigned int c;	//Index to third vertex
	};
	struct MD5Joint
	{
		std::string* name;			//Pointer to the name of this joint
		int parent;			//Index into the joint array of parent
		int forceWorld;
		glm::vec3 position;		//Position relative to parent joint
		glm::quat orientation;	//Orientation relative to parent joint
		glm::mat4 transform;		//World transform of this joint

		glm::mat4 localTransform;
	};
	struct MD5Vert {
		int		vertIndex;		//Which vertex of the MD5SubMesh this is
		glm::vec2 texCoords;		//Texture coordinates of this vertex
		int		weightIndex;	//Array index of first weighting influence
		int		weightElements; //Number of MD5Weights that influence this vertex
	};
	struct MD5Weight {
		int		weightIndex;	//Which weight of the MD5SubMesh this is
		int		jointIndex;		//Which joint of the MD5Skeleton this weight is relative to
		float	weightValue;	//How much influence this MD5Weight has on the MD5Vert
		glm::vec3 position;		//Anchor position of this MD5Weight
	};
	struct MD5Skeleton
	{
		MD5Joint*joints;	//Pointer to our array of MD5Joints
		int numJoints;		//Number of joints in the skeleton

							//Always initialise pointers to null!
		MD5Skeleton() {
			joints = NULL;
			numJoints = 0;
		}

		//MD5Skeleton's have heap memory, so we must free it!
		~MD5Skeleton() {
			delete[] joints;
		}
	};
	struct MD5SubMesh {
		int numverts;			//How many vertices in this MD5SubMesh
		int numtris;			//How many triangles in this MD5SubMesh
		int numweights;			//How many weights in this MD5SubMesh
		unsigned int texIndex;	//OGL Name of the diffuse map (if any)

#ifdef	MD5_USE_TANGENTS_BUMPMAPS
		unsigned int bumpIndex;	//OGL Name of the bump map (if any)
#endif

		MD5Tri*		tris;		//Pointer to array of MD5Tris of this MD5SubMesh
		MD5Weight*	weights;	//Pointer to array of MD5Weights of this MD5SubMesh
		MD5Vert*	verts;		//Pointer to array of MD5Verts of this MD5SubMesh

		MD5SubMesh() {
			texIndex = 0;
#ifdef	MD5_USE_TANGENTS_BUMPMAPS
			bumpIndex = 0;
#endif
			tris = NULL;
			weights = NULL;
			verts = NULL;
		}

		/*
		We should probably delete our heap memory...
		*/
		~MD5SubMesh() {
			delete[] tris;
			delete[] weights;
			delete[] verts;
		}
	};
	class importer
	{

	};

	class file_data
	{

	};

	class animation
	{

	};
}

#endif