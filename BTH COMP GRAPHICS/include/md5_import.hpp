#ifndef MD5_IMPORT_HPP
#define MD5_IMPORT_HPP
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <map>
#include <vector>

#define MD5_VERSION_TAG			"MD5Version"
#define MD5_COMMANDLINE_TAG		"commandline"
#define MD5_NUMJOINTS_TAG		"numJoints"
#define MD5_NUMMESHES_TAG		"numMeshes"
#define MD5_JOINTS_TAG			"joints"
#define MD5_MESH_TAG			"mesh"
#define MD5_SUBMESH_NUMVERTS	"numverts"
#define MD5_SUBMESH_NUMTRIS		"numtris"
#define MD5_SUBMESH_NUMWEIGHTS  "numweights"
#define MD5_SUBMESH_SHADER		"shader"
#define MD5_SUBMESH_VERT		"vert"
#define MD5_SUBMESH_TRI			"tri"
#define MD5_SUBMESH_WEIGHT		"weight"

#define MD5_WEIGHT_TEXNUM		10
#define MD5_TRANSFORM_TEXNUM	11

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

	class MD5Anim;

	class MD5FileData
	{
	public:
		friend class MD5Anim;
		friend class MD5Mesh;
		friend class MD5Node;

		MD5FileData(const std::string &filename);
		~MD5FileData(void);

		void		CloneSkeleton(MD5Skeleton &into) const;

		//My experimental hardware skinning uses some extra data, and a couple of
		//functions to upload that data to the graphics card...
#ifdef MD5_USE_HARDWARE_SKINNING 
		void		BindTextureBuffers() const;
		void		UpdateTransformTBO(const MD5Skeleton &skel) const;
#endif

		//Mesh*		GetRootMesh() const { return (Mesh*)rootMesh; }

		MD5Anim*	GetAnim(const std::string &name) const;

		/*
		Adds an MD5Anim to the MD5Mesh's map of animations. This should probably
		return a bool to detect file loading success.
		*/
		void		AddAnim(std::string filename);

		int			GetIndexForJointName(const std::string &name) const;

		/*
		idTech games (as well as Unreal engine games, and some others) don't use
		the standard axis' you'll be used to, pointing down negative Z and having
		Y as 'up'. Instead, they point down negative X, and have Z as up. This means
		all of the data in the MD5Mesh file is 'incorrectly' rotated for the
		standard OGL render space. We can transform everything to the 'correct' space
		using a simple transformation matrix. We only need one such matrix, and it
		never needs to change once created, so it is declared as static const.
		*/
		static const glm::mat4 conversionMatrix;

		static const glm::mat4 inverseConversionMatrix;


	protected:
		/*
		Helper function used by LoadMD5Mesh to load in the joints for this mesh
		from an MD5Mesh file.
		*/
		int		LoadMD5Joints(std::ifstream &from);

		/*
		Helper function used by LoadMD5Mesh to load in the submeshes that make
		up this mesh from an MD5Mesh file.
		*/
		void	LoadMD5SubMesh(std::ifstream &from, int &count);

		/*
		Once all of a MD5Mesh file's data has been loaded in, we can create the
		various Mesh class instances required to render our new MD5Mesh, including
		setting up all of the VBOs and VAOs
		*/
		void	CreateMeshes();

#ifdef MD5_USE_HARDWARE_SKINNING 
		void	CreateTBOs();
#endif

		/*
		In Doom3 and other idTech4 games, each MD5Mesh has a 'shader' attached
		to it, containing all of the uniforms that should be sent to the game's
		active shader, and which textures should be used for which texture sampler.
		As we don't have anything quite so extravagant in this tutorial series,
		instead we have a series of shader 'proxy' files, containing two strings -
		one for the diffuse map, and one for the bump map.
		*/
		void	LoadShaderProxy(std::string filename, MD5SubMesh &m);


		MD5Skeleton		bindPose;			//'Default' bindpose skeleton

		//MD5Mesh*		rootMesh;


		MD5SubMesh*		subMeshes;			//array of MD5SubMeshes
		unsigned int	numSubMeshes;		//How many submeshes this mesh has
		std::vector<std::string>	jointNames;			//Array of joint names for the skeleton

		std::map<std::string, MD5Anim*>	animations;	//map of anims for this mesh

													//These are extra buffers, and arrays of data, for use when using hardware
													//skinning of meshes. We have 2 extra buffers - VBOs for storing the
													//weights and skeleton of our mesh. We also have 2 extra textures, which will
													//really be OpenGL 'pointers' to the new VBOs, suitable for sampling inside
													//a shader. Finally, we have a couple of arrays, just to store where our VBOs
													//get their data from, for consistency with other VBO data arrays we make
#ifdef MD5_USE_HARDWARE_SKINNING 
		GLuint			weightBuffer;		//VBO where we keep weightings of this mesh
		GLuint			transformBuffer;	//VBO where we keep skeleton transforms

		GLuint			weightTexture;		//TBO Texture we use to access the VBO
		GLuint			transformTexture;	//TBO Texture we use to access the VBO

		glm::mat4*		transforms;			//Array of skeleton transforms
		glm::vec3*		weightings;			//Array of Vertex weightings
#endif


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