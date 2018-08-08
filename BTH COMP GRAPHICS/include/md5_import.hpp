#ifndef MD5_IMPORT_HPP
#define MD5_IMPORT_HPP
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <map>
#include <vector>
#include "mesh.hpp"

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

		mesh*		GetRootMesh() const { return (mesh*)rootMesh; }

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

		md5_mesh*		rootMesh;


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


	//ANIM

	/*
	MD5 Files are plain text, with each section in the file marked with tags.
	It's good practice to use defines to keep these tag strings, even though
	most of them will only actually be used once.
	*/
#define MD5_VERSION_TAG			"MD5Version"
#define MD5_COMMANDLINE_TAG		"commandline"
#define MD5_NUMJOINTS_TAG		"numJoints"
#define MD5_ANIM_NUMFRAMES		"numFrames"
#define MD5_ANIM_FRAMERATE		"frameRate"
#define MD5_ANIM_COMPONENTS		"numAnimatedComponents"
#define MD5_ANIM_HIERARCHY		"hierarchy"
#define MD5_ANIM_BOUNDS			"bounds"
#define MD5_ANIM_BASEFRAME		"baseframe"
#define MD5_ANIM_FRAME			"frame"


	/*
	Each MD5 anim frame has the differences between the baseframe and itself,
	stored as an array of floating points. So if the x axis position of a joint
	changes in a frame, the joints position.x is replaced with a float from the
	array. Whether a frame changes a joint's position is determined by a mask,
	which is ANDed against using the following defines, where a value of > 1
	means the component changes.
	*/
#define MD5_ANIM_XPOS			1
#define MD5_ANIM_YPOS			2
#define MD5_ANIM_ZPOS			4

#define MD5_ANIM_XQUAT			8
#define MD5_ANIM_YQUAT			16
#define MD5_ANIM_ZQUAT			32

	/*
	Every MD5Anim has a number of MD5AnimJoints. These are essentially the
	same as MD5Mesh joints, with an added bitmask, which determines which
	components of the joint, if any, are updated in this animation.
	*/
	struct MD5AnimJoint {
		std::string  name;	//Name of this joint
		int parent;			//Index of the parent of this joint
		int flags;			//bitmask used to determine which components update
		int frameIndex;		//First float in the changes array that effects joint
	};

	/*
	Every MD5Anim frame has an axis aligned bounding box, stretching from min
	to max. Useful for collision detection etc.
	TODO: These haven't actually been transformed into the correct axis yet!
	*/
	struct MD5Bounds {
		glm::vec3 min;
		glm::vec3 max;
	};

	/*
	Every MD5Anim has an MD5BaseFrame, consisting of the orientations and positions
	of every MD5AnimJoint
	*/
	struct MD5BaseFrame {
		glm::quat* orientations;	//Orientations for every base frame joint
		glm::vec3*	positions;		//Positions for every base frame joint

		MD5BaseFrame() {
			orientations = NULL;
			positions = NULL;
		}

		//Delete our heap memory
		~MD5BaseFrame() {
			delete[] orientations;
			delete[] positions;
		};
	};

	/*
	Every MD5Anim is made up of a number of MD5Frame structs - one for each
	frame of the animation. It consists of an array of floats, which equate
	to the orientation and position changes from the baseframe for the current
	animation frame. Which component equates to each frame 'delta' is determined
	by the flags variable of the MD5AnimJoint
	*/
	struct MD5Frame {
		float* components;

		MD5Frame() {
			components = NULL;
		}

		~MD5Frame() {
			delete[] components;
		};
	};

	//Tell the compiler that we need the MD5Skeleton structure compiled
	struct MD5Skeleton;

	/*
	Now for the class definition. Every MD5Anim has a number of joints
	(which should equal the number of joints of the MD5Mesh it is to be applied to)
	a number of frames of animation, an axis-aligned bounding box for every frame,
	and a baseFrame.
	*/
	class MD5Anim {
	public:
		//Constructor takes in a filename to load the MD5Anim data from
		MD5Anim(std::string filename);
		~MD5Anim(void);

		//Transforms the passed in skeleton to the correct positions and
		//orientations for the desired frame
		void	TransformSkeleton(MD5Skeleton &skel, unsigned int frame);

		//Returns the framerate
		unsigned int	GetFrameRate() { return frameRate; }
		//Returns the number of frames of animation
		unsigned int	GetNumFrames() { return numFrames; }

	protected:
		//Helper function used by the constructor to load in an MD5Anim from the 
		//relevent file
		void	LoadMD5Anim(std::string filename);

		//Helper function for LoadMD5Anim to load in the joints
		void	LoadMD5AnimHierarchy(std::ifstream &from, unsigned int &count);

		//Helper function for LoadMD5Anim to load in the bounding boxes
		void	LoadMD5AnimBounds(std::ifstream &from, unsigned int &count);

		//Helper function for LoadMD5Anim to load in the base frame
		void	LoadMD5AnimBaseFrame(std::ifstream &from);

		//Helper function for LoadMD5Anim to load in animation frames
		void	LoadMD5AnimFrame(std::ifstream &from, unsigned int &count);

		unsigned int	frameRate;		//Required framerate of this animation
		unsigned int	numJoints;		//Number of joints in this animation
		unsigned int	numFrames;		//Number of frames in this animation
		unsigned int	numAnimatedComponents; //Number of transform differences per frame

		MD5AnimJoint*	joints;			//Array of joints for this animation
		MD5Bounds*		bounds;			//Array of bounding boxes for this animation
		MD5Frame*		frames;			//Array of individual frames for this animation
		MD5BaseFrame	baseFrame;		//BaseFrame for this animation
	};



}

#endif