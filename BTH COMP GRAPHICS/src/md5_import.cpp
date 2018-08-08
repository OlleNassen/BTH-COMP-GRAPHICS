#include "md5_import.hpp"
#include <fstream>
#include <iostream>

namespace md5
{

	/*
	This is the actual matrix data that will be used for the space
	conversion matrix. As you can see, it's like an identity matrix,
	but swaps the Y and Z axis, and negates the X axis.
	*/
	static float	matrixElements[16] = {
		-1,  0, 0, 0,
		0,  0, 1, 0,
		0,  1, 0, 0,
		0,  0, 0, 1
	};

	/*
	static class variables must still be instantiated somewhere!!!
	*/
	const glm::mat4 MD5FileData::conversionMatrix = Matrix4(matrixElements);

	MD5FileData::MD5FileData(const std::string &filename) {
		std::ifstream f(filename, std::ios::in);	//MD5 files are text based, so don't make it an ios::binary ifstream...

		if (!f) {
			return; //Oh dear!
		}

		//We have our MD5 file handle!
		int numExpectedJoints = 0;
		int numExpectedMeshes = 0;
		int md5Version = 0;
		int numLoadedMeshes = 0;
		int numLoadedJoints = 0;

		/*
		Now we simply load in the data, while there's still data to load
		*/
		while (!f.eof()) {
			std::string currentLine;	//A temporary string to keep the current line in
			f >> currentLine;			//stream the next line of the file in

			if (currentLine.empty()) {	//Actually, there's nothing in this line...usually
				continue;				//because we've hit the end of the file.
			}
			/*
			String::Find returns a value, equating to the position in the string the search
			string is - or the special value 'npos' if the searchstring is not found at all
			*/

			else if (currentLine.find(MD5_VERSION_TAG) != std::string::npos) {
				//We've found the MD5 version string!
				//ifstream allows us to stream ints,floats etc into variables
				f >> md5Version;
				std::cout << "MD5 File version is: " << md5Version << std::endl;
			}
			else if (currentLine.find(MD5_COMMANDLINE_TAG) != std::string::npos) {
				/*
				MD5Mesh files sometimes have a 'command line' value, used by the game
				toolchain to generate some data. We don't care about it!
				*/
				std::cout << "Ignoring commandline value" << std::endl;
				getline(f, currentLine);
			}
			else if (currentLine.find(MD5_NUMJOINTS_TAG) != std::string::npos) {
				f >> numExpectedJoints; //Load in the number of joints held in this MD5Mesh file
				std::cout << "Expecting file to have " << numExpectedJoints << " joints" << std::endl;
				//grab enough space for this number of joints
				bindPose.joints = new MD5Joint[numExpectedJoints];
			}
			else if (currentLine.find(MD5_NUMMESHES_TAG) != std::string::npos) {
				f >> numExpectedMeshes; //load in the number of submeshes held in this md5mesh
				std::cout << "Expecting file to have " << numExpectedMeshes << " meshes" << std::endl;

				subMeshes = new MD5SubMesh[numExpectedMeshes];
			}
			else if (currentLine.find(MD5_JOINTS_TAG) != std::string::npos) {
				numLoadedJoints += LoadMD5Joints(f); //Load in as many joints as possible
			}
			else if (currentLine.find(MD5_MESH_TAG) != std::string::npos) {
				LoadMD5SubMesh(f, numLoadedMeshes);	//Load in a submesh
				++numLoadedMeshes;
			}
		}

		bindPose.numJoints = numLoadedJoints;
		numSubMeshes = numLoadedMeshes;

		//If we get to here, we've loaded in everything from the file, so we can close it
		f.close();

		//If what we've loaded in does not equal what we /should/ have loaded in, we'll output an error
		//
		if (numLoadedJoints != numExpectedJoints) {
			std::cout << "Expected " << numExpectedJoints << " joints, but loaded " << numLoadedJoints << std::endl;
			return;
		}

		if (numLoadedMeshes != numExpectedMeshes) {
			std::cout << "Expected " << numExpectedMeshes << " meshes, but loaded " << numLoadedMeshes << std::endl;
			return;
		}

		//Everything is OK! let's create our submeshes :)
		CreateMeshes();


#ifdef MD5_USE_HARDWARE_SKINNING 
		//Create the Texture Buffer Objects for this mesh
		CreateTBOs();
#endif
	}

	MD5FileData::~MD5FileData(void) {
		//Delete any MD5 animations we have loaded for this mesh
		for (std::map<std::string, MD5Anim*>::iterator i = animations.begin(); i != animations.end(); ++i) {
			delete i->second;
		}
		//Delete the additional data used when doing hardware skinning.
#ifdef MD5_USE_HARDWARE_SKINNING 
		glDeleteBuffers(1, &weightBuffer);
		glDeleteBuffers(1, &transformBuffer);

		glDeleteTextures(1, &weightTexture);
		glDeleteTextures(1, &transformTexture);

		delete[] transforms;
		delete[] weightings;
#endif
	}

	/*
	Each MD5Node in our applications must have its own skeleton, so that each
	can have a unique transformation. To get these skeletons, we define a
	'clone function', that will fill an MD5Nodes skeleton with the correct
	number of joints for the file it will be drawn from. This requires a memcpy,
	so we can't just do a 'blind' assignment - all node skeletons would end
	up pointing to the same data!

	*/
	void	MD5FileData::CloneSkeleton(MD5Skeleton &into) const {
		into.numJoints = bindPose.numJoints;
		into.joints = new MD5Joint[bindPose.numJoints];

		memcpy((void*)into.joints, (void*)bindPose.joints, sizeof(MD5Joint)*bindPose.numJoints);
	}


	/*
	As explained elsewhere, I use a special OpenGL construct called a 'Texture
	Buffer Object' to keep the weighting and skeleton data for each md5 on the
	graphics card. This mechanism allows a VBO to be accessed like a texture
	inside a shader. By storing all of the weighting and skeleton data inside
	these TBOs, the vertex shader has everything necessary in order to perform
	the skinning process - graphics cards are much better at the vector maths
	used in skinning that CPUs are, so this can end up many times faster than
	relying on even a fast CPU.

	*/
#ifdef MD5_USE_HARDWARE_SKINNING 
	void	MD5FileData::CreateTBOs() {
		//Generate two VBOs, just like we do inside the Mesh class
		glGenBuffers(1, &weightBuffer);		//contains weighting information
		glGenBuffers(1, &transformBuffer);	//contains transform information

											//And make a couple of textures, which should also be familiar to you!
		glGenTextures(1, &weightTexture);
		glGenTextures(1, &transformTexture);

		//We need to determine how much memory to allocate in order to upload
		//the mesh transform data to the graphics card. 
		int transformCount = bindPose.numJoints * 2; //storing inverse bindpose matrix, too!
		int weightCount = 0;

		for (unsigned int i = 0; i < numSubMeshes; ++i) {
			weightCount += subMeshes[i].numweights;
		}

		//Now we know how much space to allocate!
		transforms = new Matrix4[transformCount];
		weightings = new Vector3[weightCount * 2];

		unsigned int currentWeight = 0;

		//We store each of the weightings inside two vector3s - The first
		//contains the index, joint, and weighting value, while the second
		//contains the transformation position for this anchoring weight.

		for (unsigned int i = 0; i < numSubMeshes; ++i) {
			for (int j = 0; j < subMeshes[i].numweights; ++j) {
				weightings[(currentWeight * 2) + 0] = Vector3(
					(float)subMeshes[i].weights[j].weightIndex,
					(float)subMeshes[i].weights[j].jointIndex,
					(float)subMeshes[i].weights[j].weightValue);

				weightings[(currentWeight * 2) + 1] = subMeshes[i].weights[j].position;

				currentWeight++;
			}
		}

		//We aren't keeping these wrapped up inside a VAO, so to fill our graphics card with VBO
		//data, all we need to do is bind the appropriate buffer, then call glBufferData...

		glBindBuffer(GL_TEXTURE_BUFFER, weightBuffer);
		glBufferData(GL_TEXTURE_BUFFER, weightCount * sizeof(Vector3) * 2, &weightings[0], GL_STATIC_DRAW);
		glBindBuffer(GL_TEXTURE_BUFFER, 0);

		//Although we're going to allocate the memory for our skeleton, we aren't going to send any
		//skeleton data to it just yet - this will be done on a per-node basis as necessary.
		glBindBuffer(GL_TEXTURE_BUFFER, transformBuffer);
		glBufferData(GL_TEXTURE_BUFFER, transformCount * sizeof(Matrix4), &transforms[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_TEXTURE_BUFFER, 0);

		//By the end of this function, we don't really need subMeshes anymore!
		//All relevent data is in our new buffers.
	}
#endif

	/*
	Adds an MD5Anim to this MD5Mesh, so we can animate it. We store each
	MD5Anim in a map, using its filename as a key.
	*/
	void	MD5FileData::AddAnim(std::string filename)
	{
		animations.insert(std::make_pair(filename, new MD5Anim(filename)));
	}


	/*
	This function loads in the texture filenames for the current MD5Mesh.

	Each MD5Mesh submesh has a 'shader' applied to it, which in idTech4 games,
	will contain all of the texture filenames, and uniform data etc required
	for whatever effect has been applied to the mesh. For this tutorial series,
	we don't really care about all that, we just want the filenames. So instead
	of trying to parse the Doom3 shader files, instead there's some 'proxy' shaders
	in the Meshes folder, containing a couple of filenames. Each proxy has the same
	filename as a shader reference in the MD5Mesh file, so the MD5Mesh doesn't have
	to be modified at all. If you want to add more MD5Meshes to your projects, you
	are probably going to have to look in the MD5Mesh file and see what 'shaders'
	it references, and write proxy shaders for them.
	*/
	void	MD5FileData::LoadShaderProxy(std::string filename, MD5SubMesh &m) {
		//The filename passed to this function might be a relative path, or maybe even
		//an absolute path. So what what we do is cut the folder structure from the 
		//filename string entirely, leaving only the filename we want. We also remove
		//the file extension, and replace it with .proxy, the file extension I chose
		//to keep our 'pretend' texture filenames in.
		filename = filename.substr(1, filename.size() - 2);
		int at = filename.find_last_of('/');

		std::ifstream f("../../Meshes/" + filename.substr(at + 1) + ".proxy", std::ios::in);

		if (!f) {	//Oh dear.
			return;
		}

		/*
		Proxy files have a couple of strings, referencing the relative path of a diffuse map
		and a bump map. If you have a gloss map for an MD5Mesh, it should be pretty trivial
		to add support to load them from a proxy file, too.
		*/

		std::string  diffuseMap;
		f >> diffuseMap;
#ifdef MD5_USE_TANGENTS_BUMPMAPS
		string	bumpMap;
		f >> bumpMap;
#endif

		f.close();	//That's all that's in the file, so we can close it.


					//Load in the textures using SOIL. As long as we actually delete the MD5Mesh, these
					//textures will eventually be deleted from the OGL context, as they'll end up applied
					//to the Mesh texture values.
		m.texIndex = SOIL_load_OGL_texture(
			diffuseMap.c_str(),
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS// | SOIL_FLAG_COMPRESS_TO_DXT
		);

#ifdef MD5_USE_TANGENTS_BUMPMAPS
		m.bumpIndex = SOIL_load_OGL_texture(
			bumpMap.c_str(),
			SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | 0
		);
#endif
	}

	/*
	Loads in the MD5Mesh joint hierarchy. Uses the same ifstream as LoadMD5Mesh, passed
	by reference. This function will return how many joints were loaded.
	*/
	int MD5FileData::LoadMD5Joints(std::ifstream &from) {
		/*
		The joints section of the file should look something like this...

		joints {
		"name"	parent ( pos.x pos.y pos.z ) ( quat.x quat.y quat.z )	//
		...more things
		}

		"joints" is parse by LoadMD5Mesh, so the first thing this function should see
		is a brace
		*/

		int loaded = 0;
		char skipChar;

		std::string tempLine; //Another temporary line to stream things into...

		do {
			from >> tempLine;	//Stream a line in

			if (tempLine == "{") {//In a well-behaved MD5 file, the first line will be '{'
			}
			else if (tempLine[0] == '"') {//Name of joint begins with a "
										  //Substring cuts the name out from between the speech marks!
										  //We keep the actual strings separate, and store a pointer to the string
										  //in the joint (Avoids nasty string problems when using memcpy)
				jointNames.push_back(tempLine.substr(1, tempLine.find('"', 1) - 1));
				bindPose.joints[loaded].name = &jointNames.back();

				from >> bindPose.joints[loaded].parent;
				from >> skipChar; //first vec beginning bracket
				from >> bindPose.joints[loaded].position.x;
				from >> bindPose.joints[loaded].position.y;
				from >> bindPose.joints[loaded].position.z;
				from >> skipChar; //first vec end bracket
				from >> skipChar; //second vec beginning bracket
				from >> bindPose.joints[loaded].orientation.x;
				from >> bindPose.joints[loaded].orientation.y;
				from >> bindPose.joints[loaded].orientation.z;
				from >> skipChar; //second vec beginning bracket

				/*
				To save a tiny bit of space, the 4th component of the orientation
				quaternion is left out of the files. As we're dealing with unit length
				quaternions (i.e they have a length of 1), the 4th component will be
				sqrt of (1.0 - length of the other 3 components)
				*/

				bindPose.joints[loaded].orientation.GenerateW();
				bindPose.joints[loaded].orientation.Normalise();

				//Now we have the orientation and position, we can form the transformation matrix
				//for this joint. We need to further transform this matrix by the conversionmatrix
				//so that the rotation is in OpenGL space.

				bindPose.joints[loaded].transform = bindPose.joints[loaded].orientation.ToMatrix();
				bindPose.joints[loaded].transform.SetPositionVector(bindPose.joints[loaded].position);

				bindPose.joints[loaded].transform = conversionMatrix * bindPose.joints[loaded].transform;

				bindPose.joints[loaded].localTransform = bindPose.joints[loaded].transform;

				++loaded;	//...Just assume it worked ;)
			}
		} while (tempLine != "}");

		return loaded; //Return how many joints we loaded in
	}

	/*

	*/
	void MD5FileData::LoadMD5SubMesh(std::ifstream &from, int &count) {
		/*
		The submesh section of the file should look like this:

		mesh {
		shader shadername

		numverts numverts

		vert num ( tex.x tex.y ) weightindex weightcount
		...more verts

		numtris numtris
		tri trinum vertA vertB vertC
		...more tris

		numweights numweights
		weight weightnum jointnum weightval ( pos.x pos.y pos.z )
		}
		*/

		char skipChar;			//We skip the brackets by streaming them into this
		std::string tempLine;	//Another temporary line to stream things into...


		int vertsLoaded = 0;				//Number of vertices actually loaded
		int trisLoaded = 0;				//Number of tris actually loaded
		int weightsLoaded = 0;				//number of weights actually loaded

											//We created the memory for the submeshes earlier, so we can just grab a reference
											//to the next available submesh...this'll go wrong if the MD5Mesh is invalid!
		MD5SubMesh& m = subMeshes[count];

		do {
			from >> tempLine;

			if (tempLine == MD5_SUBMESH_SHADER) {
				//If the line is a shader, we let the LoadShaderProxy function handle it
				std::string shaderName;
				from >> shaderName;
				LoadShaderProxy(shaderName, m);
			}
			else if (tempLine == MD5_SUBMESH_NUMVERTS) {
				//if the line tells us how many vertices to expect, initialise the memory for them
				from >> m.numverts;
				m.verts = new MD5Vert[m.numverts];
			}
			else if (tempLine == MD5_SUBMESH_NUMTRIS) {
				//if the line tells us how many tris to expect, initialise the memory for them
				from >> m.numtris;
				m.tris = new MD5Tri[m.numtris];
			}
			else if (tempLine == MD5_SUBMESH_NUMWEIGHTS) {
				//if the line tells us how many weights to expect, initialise the memory for them
				from >> m.numweights;
				m.weights = new MD5Weight[m.numweights];
			}
			else if (tempLine == MD5_SUBMESH_VERT) {
				//if the line is a vertex, load it in
				from >> m.verts[vertsLoaded].vertIndex;

				from >> skipChar; //vec beginning bracket
				from >> m.verts[vertsLoaded].texCoords.x;
				from >> m.verts[vertsLoaded].texCoords.y;
				from >> skipChar; //vec end bracket

				from >> m.verts[vertsLoaded].weightIndex;
				from >> m.verts[vertsLoaded].weightElements;

				static int testweightcount = 0;

				if (m.verts[vertsLoaded].weightElements > testweightcount) {
					testweightcount = m.verts[vertsLoaded].weightElements;

					std::cout << testweightcount << std::endl;
				}

				vertsLoaded++;
			}
			else if (tempLine == MD5_SUBMESH_WEIGHT) {
				//if the line is a weight, load it in
				from >> m.weights[weightsLoaded].weightIndex;
				from >> m.weights[weightsLoaded].jointIndex;
				from >> m.weights[weightsLoaded].weightValue;

				from >> skipChar; //vec beginning bracket
				from >> m.weights[weightsLoaded].position.x;
				from >> m.weights[weightsLoaded].position.y;
				from >> m.weights[weightsLoaded].position.z;
				from >> skipChar; //vec end bracket

				weightsLoaded++;
			}
			else if (tempLine == MD5_SUBMESH_TRI) {
				//if the line is a triangle, load it in
				from >> m.tris[trisLoaded].triIndex;
				from >> m.tris[trisLoaded].a;
				from >> m.tris[trisLoaded].b;
				from >> m.tris[trisLoaded].c;

				trisLoaded++;
			}
			else {
				//Perhaps different MD5 files have other data? Or maybe the file is screwed...
				if (tempLine != "}" && tempLine != "{") {
					std::cout << "Unknown MD5 file tag: " << tempLine << std::endl;
				}
			}
		} while (tempLine != "}");
	}

	/*
	Create the child Mesh class instances from the loaded in MD5SubMeshes.
	*/
	void MD5FileData::CreateMeshes() {
		rootMesh = new MD5Mesh(*this); //This has to be an md5mesh, so it has a child interface... :(

		int indexStart = 0;

		for (unsigned int i = 0; i < numSubMeshes; ++i) {
			MD5SubMesh& subMesh = subMeshes[i]; //Reference to the current submesh

			MD5Mesh*target = new MD5Mesh(*this);

			rootMesh->AddChild(target);

			target->texture = subMesh.texIndex;				//Assign the diffuse map
			target->vertices = new glm::vec3[subMesh.numverts];	//Make vertex	mem
			target->textureCoords = new glm::vec2[subMesh.numverts];	//Make texCoord mem
#ifdef MD5_USE_HARDWARE_SKINNING
			target->weights = new Vector2[subMesh.numverts];	//Make weight mem
#endif

#ifdef MD5_USE_NORMALS
																//Create space for normals!
			target->normals = new Vector3[subMesh.numverts];
#endif 

#ifdef MD5_USE_TANGENTS_BUMPMAPS
			//Create space for tangents, and assign the bump texture
			//		target->bumpTexture	  = subMesh.bumpIndex;	
			//		target->tangents	  = new Vector3[subMesh.numverts];
#endif

			target->numIndices = subMesh.numtris * 3; //Each tri has 3 points....
			target->numVertices = subMesh.numverts;

			target->indices = new unsigned int[target->numIndices]; //Make mem for indices

																	/*
																	Here we go through each tri, and put its indices in the Mesh index buffer. You'll see
																	we have a weird ordering here, doing cba, rather than abc. MD5 triangles have a
																	/clockwise/ winding, whereas OGL expects anticlockwise to be 'forward facing' for
																	a triangle. So, we simply reverse the order of indices to make the tri anticlockwise
																	*/
			for (int j = 0; j < subMesh.numtris; ++j) {
				target->indices[(j * 3)] = subMesh.tris[j].c;
				target->indices[(j * 3) + 1] = subMesh.tris[j].b;
				target->indices[(j * 3) + 2] = subMesh.tris[j].a;

				//now to fill up its weighting information		
			}

#ifdef MD5_USE_HARDWARE_SKINNING
			for (int j = 0; j < subMesh.numverts; ++j) {
				target->weights[j].x = (float)(subMesh.verts[j].weightElements);
				target->weights[j].y = (float)(subMesh.verts[j].weightIndex + indexStart);
			}
			//Also Buffer this submeshes 'extra' data (its weightings)
			target->BufferExtraData();
#endif


			//Make the VAO and VBOs for this submesh!
			target->BufferData();

			//target->RebufferData();

			indexStart += subMesh.numweights;
		}

		rootMesh->SkinVertices(bindPose);
	}

#ifdef MD5_USE_HARDWARE_SKINNING 
	//This function will 'bind' our new VBOs to our new textures. 
	//We start off by binding a texture, just as if we were to
	//modify a texture parameter, except instead, we call the
	//glTexBuffer function, which tells the texture to get its
	//data from a VBO. We use the GL_RGB32F and GL_RGBA32F 
	//symbolic constants to define how many 'channels' our 
	//texture has, which would be 3 if we were using a 
	//vec3 VBO, and so on.
	void	MD5FileData::BindTextureBuffers() const {
		glActiveTexture(GL_TEXTURE0 + MD5_WEIGHT_TEXNUM);
		glBindTexture(GL_TEXTURE_BUFFER, weightTexture);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, weightBuffer);

		glActiveTexture(GL_TEXTURE0 + MD5_TRANSFORM_TEXNUM);
		glBindTexture(GL_TEXTURE_BUFFER, transformTexture);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, transformBuffer);
	}

	//Here's where we actually fill in the 'transforms' memory we created
	//earlier, and send it to the graphics card - an mD5Node will call this
	//function before its draw call, so that the vertex shader has the 
	//correct data
	void	MD5FileData::UpdateTransformTBO(const MD5Skeleton &skel) const {
		for (int i = 0; i < skel.numJoints; ++i) {
			//Oh dear...If we'd stored our MD5Skeleton in a data-driven way,
			//rather than an object oriented way, this'd be a simple memcpy
			//and for pure GPU skinning, we could just have given the pointer to
			//the skeletons transform array to glBufferSubData. So much for OO...

			//The reason we store /two/ transforms is due to needing to transform
			//the mesh's normals into the correct orientation for the current frame
			//of animation. We do this by first generating the normals and tangents
			//for the 'bind pose' of the mesh - the 'default' position of the mesh.
			//we then calculate the /inverse/ of the rotation for each of the bind
			//pose's joints - remember that that the inverse of a matrix moves us
			//'back a space', so this inverse matrix takes us from global joint
			//space in the bind pose, to the local joint space. Also remember we can
			//take the transpose of a matrix to get its inverse if we know it has 
			//no scaling information in it. We can use this inverse matrix to transform
			//a bind pose normal to joint space - which we can then transform to the
			//correct orientation for the current frame of animation. This is quite
			//tricky to get your head around, but think of it like the local and
			//world space of your vertices - we can go from world to local space
			//using the inverse of the model matrix, and so we can go from a world
			//normal to a local normal using an inverse matrix, too - and since
			//our normal's orientaiton is defined by a joint, it is the inverse
			//of that joints world transform that will take the normal to the local
			//space of that joint. 
			transforms[(i * 2) + 0] = skel.joints[i].transform;

			transforms[(i * 2) + 1] = bindPose.joints[i].transform.GetTransposedRotation();
		}

		glBindBuffer(GL_TEXTURE_BUFFER, transformBuffer);
		glBufferSubData(GL_TEXTURE_BUFFER, 0, bindPose.numJoints * 2 * sizeof(Matrix4), (void*)&transforms[0]);
		glBindBuffer(GL_TEXTURE_BUFFER, 0);

		GL_BREAKPOINT;
	}
#endif
	/*
	We keep all of the animations for a particular mesh inside a map, sorted
	by its name - we can search and get an animation using this function,
	bearing in mind it will return NULL if the specified anim cannot be found.
	*/
	MD5Anim*	MD5FileData::GetAnim(const std::string &name) const {
		std::map<std::string, MD5Anim*>::const_iterator i = animations.find(name);

		if (i != animations.end()) {
			return i->second;
		}
		return NULL;
	}

	int			MD5FileData::GetIndexForJointName(const std::string &name) const {
		for (unsigned int i = 0; i < jointNames.size(); ++i) {
			if (jointNames[i] == name) {
				return i;
			}
		}
		return -1;
	}

	MD5Anim::MD5Anim(std::string filename) {
		numAnimatedComponents = 0;
		frameRate = 0;
		numJoints = 0;
		numFrames = 0;
		joints = NULL;
		bounds = NULL;
		frames = NULL;

		LoadMD5Anim(filename);
	}

	/*
	Destructor simply deletes our heap memory
	*/
	MD5Anim::~MD5Anim(void) {
		delete[] joints;
		delete[] bounds;
		delete[] frames;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	void MD5Anim::LoadMD5Anim(std::string filename) {
		//The MD5Anim is human readable, and stores its data in an easily
		//traversable way, so we can simply stream data from the file
		std::ifstream f(filename, std::ios::in);

		if (!f) {	//Opening the file has failed :(
			return;
		}

		//We have our MD5 file handle!
		unsigned int md5Version = 0;
		unsigned int numLoadedFrames = 0;
		unsigned int numLoadedJoints = 0;
		unsigned int numLoadedBounds = 0;

		/*
		Now we simply load in the data, while there's still data to load
		*/
		while (!f.eof()) {
			std::string currentLine;	//A temporary string to keep the current line in
			f >> currentLine;			//stream the next line of the file in

			if (currentLine.empty()) {	//Actually, there's nothing in this line...usually
				continue;				//because we've hit the end of the file.
			}
			/*
			String::Find returns a value, equating to the position in the string the search
			string is - or the special value 'npos' if the searchstring is not found at all
			*/
			else if (currentLine.find(MD5_VERSION_TAG) != std::string::npos) {
				//We've found the MD5 version string!

				//ifstream allows us to stream ints,floats etc into variables
				f >> md5Version;
				std::cout << "MD5 File version is: " << md5Version << std::endl;
			}
			else if (currentLine.find(MD5_COMMANDLINE_TAG) != std::string::npos) {
				/*
				MD5Anim files sometimes have a 'command line' value, used by the game
				toolchain to generate some data. We don't care about it!
				*/
				std::cout << "Ignoring commandline value" << std::endl;
			}
			else if (currentLine.find(MD5_ANIM_NUMFRAMES) != std::string::npos) {
				f >> numFrames;	//Loading in the number of frames held in this MD5Anim file
				std::cout << "Expecting file to have " << numFrames << " frames" << std::endl;

				//If we have an incorrectly generated MD5Anim file, this might go wrong, as
				//there might be more frames than we've generated space for...
				bounds = new MD5Bounds[numFrames];
				frames = new MD5Frame[numFrames];
			}
			else if (currentLine.find(MD5_NUMJOINTS_TAG) != std::string::npos) {
				f >> numJoints;	//Loading in the number of joints in this MD5Anim file
				std::cout << "Expecting file to have " << numJoints << " joints" << std::endl;

				joints = new MD5AnimJoint[numJoints];
			}
			else if (currentLine.find(MD5_ANIM_FRAMERATE) != std::string::npos) {
				f >> frameRate; //Loading in the framerate of this anim
			}
			else if (currentLine.find(MD5_ANIM_COMPONENTS) != std::string::npos) {
				f >> numAnimatedComponents;	//Loading in the number of delta values in this anim
			}
			else if (currentLine.find(MD5_ANIM_HIERARCHY) != std::string::npos) {
				LoadMD5AnimHierarchy(f, numLoadedJoints);	//Let's load in the joint hierarchy!
			}
			else if (currentLine.find(MD5_ANIM_BOUNDS) != std::string::npos) {
				LoadMD5AnimBounds(f, numLoadedBounds);		//Let's load in the AABBs!
			}
			else if (currentLine.find(MD5_ANIM_BASEFRAME) != std::string::npos) {
				LoadMD5AnimBaseFrame(f);					//Let's load in the base frame!
			}
			else if (currentLine.find(MD5_ANIM_FRAME) != std::string::npos) {
				LoadMD5AnimFrame(f, numLoadedFrames);		//Let's load in an animation frame!
			}
		}

		//If we get to here, we've loaded in everything from the file, so we can close it
		f.close();

		//If what we've loaded in does not equal what we /should/ have loaded in, we'll output an error
		//
		if (numLoadedFrames != numFrames || numLoadedJoints != numJoints || numLoadedBounds != numFrames) {
			std::cout << "MD5Anim file has incorrect data..." << std::endl;
		}
	}

	/*
	Loads in the MD5Anim joint hierarchy. Uses the same ifstream as LoadMD5Anim, passed
	by reference. We also pass a count variable by reference, which this function will
	increment for every joint we load in.
	*/
	void MD5Anim::LoadMD5AnimHierarchy(std::ifstream &from, unsigned int &count) {
		/*
		The hierarchy section of the file should look something like this...

		hierarchy {
		"name"	parent flags frameIndex	//
		...more things
		}

		"hierarchy" is loaded in by LoadMD5Anim, so the first thing this function should see
		is a brace
		*/
		std::string tempLine; //Another temporary line to stream things into...

		do {
			from >> tempLine;	//Stream a line in

			if (tempLine == "{") {			//In a well-behaved MD5 file, the first line will be '{'
			}
			else if (tempLine[0] == '"') {	//It's a joint!
											//substr cuts out a section of a string, exclusive of the first and second parameter
											//positions (which should both be '"'
				joints[count].name = tempLine.substr(1, tempLine.find('"', 1) - 1);

				from >> joints[count].parent;
				from >> joints[count].flags;
				from >> joints[count].frameIndex;

				++count;
			}
		} while (tempLine != "}");	//Hit an end bracket...
	}

	/*
	Loads in the MD5Anim anim frames AABBs. Uses the same ifstream as LoadMD5Anim, passed
	by reference. We also pass a count variable by reference, which this function will
	increment for every joint we load in.
	*/
	void MD5Anim::LoadMD5AnimBounds(std::ifstream &from, unsigned int &count) {
		/*
		The bounds section of the file should look like this:
		bounds {
		( min.x min.y min.z ) ( max.x max.y max.z )
		...more things
		}

		"bounds" is loaded in by LoadMD5Anim, so the first thing this function should see
		is a brace
		*/

		char skipChar;			//We skip the brackets by streaming them into this
		std::string tempLine;	//Another temporary line to stream things into...

		do {
			from >> tempLine;

			if (tempLine == "{") {				//In a well-behaved MD5 file, the first line will be '{'
			}
			else if (tempLine[0] == '(') {		//It's a bounding box!
				from >> bounds[count].min.x;
				from >> bounds[count].min.y;
				from >> bounds[count].min.z;

				from >> skipChar;				//Skip the ')'
				from >> skipChar;				//skip the '('

				from >> bounds[count].max.x;
				from >> bounds[count].max.y;
				from >> bounds[count].max.z;

				from >> skipChar;				//Skip the ')'

				++count;
			}
		} while (tempLine != "}");		//Hit an end bracket...
	}

	/*
	Loads in the MD5Anim base frame, which consists of the default
	positions and orientations of every joint in the mesh.
	Uses the same ifstream as LoadMD5Anim, passed by reference.
	*/
	void MD5Anim::LoadMD5AnimBaseFrame(std::ifstream &from) {
		/*
		The baseframe section of the file should look like this:
		baseframe {
		( pos.x pos.y pos.z ) ( quat.x quat.y quat.z )
		...more things
		}

		"baseframe" is loaded in by LoadMD5Anim, so the first thing this function
		should see is a brace
		*/

		char skipChar;			//We skip the brackets by streaming them into this
		std::string tempLine;	//Another temporary line to stream things into...

								/*
								We need to initialise enough space on the heap for every joint transform
								*/
		baseFrame.orientations = new Quaternion[numJoints];
		baseFrame.positions = new Vector3[numJoints];

		int current = 0;

		do {
			from >> tempLine;

			if (tempLine == "{") {			//In a well-behaved MD5 file, the first line will be '{'
			}
			else if (tempLine[0] == '(') {	//It's a base frame (probably)!
				from >> baseFrame.positions[current].x;
				from >> baseFrame.positions[current].y;
				from >> baseFrame.positions[current].z;

				from >> skipChar;	//End Bracket
				from >> skipChar;	//Begin Bracket

				from >> baseFrame.orientations[current].x;
				from >> baseFrame.orientations[current].y;
				from >> baseFrame.orientations[current].z;

				/*
				To save a tiny bit of space, the 4th component of the orientation
				quaternion is left out of the files. As we're dealing with unit length
				quaternions (i.e they have a length of 1), the 4th component will be
				sqrt of (1.0 - length of the other 3 components)
				*/

				baseFrame.orientations[current].GenerateW();
				baseFrame.orientations[current].Normalise();

				from >> skipChar;	//End Bracket
				++current;
			}
		} while (tempLine != "}");
	}

	/*
	Loads in an MD5Frane, which consists of the differences from the baseframe
	of the positions and orientations of every joint in the mesh.
	Uses the same ifstream as LoadMD5Anim, passed by reference.
	We also pass a count variable by reference, which this function will
	increment for every joint we load in.
	*/
	void MD5Anim::LoadMD5AnimFrame(std::ifstream &from, unsigned int &count) {
		/*
		Each animframe section of the file should look like this:
		frame framenum {
		linear array of floating point values
		...
		}

		"frame" is loaded in by LoadMD5Anim, so the first thing this function
		should see is the framenum
		*/

		std::string tempLine;	//Another temporary line to stream things into...

		int frameNum;
		from >> frameNum;	//Stream in the current frame number

							/*
							Every frame has the same number of 'delta' floats - so even if a joint
							is only modified in a single frame, it will have a delta value in every
							frame.
							*/
		frames[frameNum].components = new float[numAnimatedComponents];

		from >> tempLine;	//Load in the next line, which /should/ be "{"

		if (tempLine == "{") {//In a well-behaved MD5 file, the first line will be '{'
			for (unsigned int i = 0; i < numAnimatedComponents; ++i) {
				//stream in the delta values of the current animation frame
				from >> frames[frameNum].components[i];
			}
			from >> tempLine;	// Should be '}'
			++count;
		}
	}


	//Transforms the passed in skeleton to the correct positions and
	//orientations for the desired frame
	void MD5Anim::TransformSkeleton(MD5Skeleton &skel, unsigned int frameNum) {
		/*
		Here's the most important function of the MD5Anim class. This transforms an input
		skeleton's joints (generally this will be the 'working' skeleton from an MD5Mesh instance)
		to the required transforms to represent the desired frame of animation
		*/

		if (frameNum > numFrames) {	//This probably shouldn't ever happen!
			return;
		}

		//Grab a reference to the frame data for the relevant frame
		MD5Frame&frame = frames[frameNum];

		//For each joint in the animation
		for (unsigned int i = 0; i < numJoints; ++i) {
			//Grab COPIES of the position and orientation of the baseframe joint
			glm::vec3		animPos = baseFrame.positions[i];
			glm::quat  animQuat = baseFrame.orientations[i];

			/*
			Each frame has a number of 'delta' components, and each joint
			uses a number of these components to update its position and
			orientation. Whether or not each value is updated or not is
			determined by the joints flags variable. The starting
			component for each joint is determined by the frameIndex value
			of the joint.

			For each value of the joint (ie its 3 position values and its 3
			orientation components (we don't bother with the 4th as we can
			reconstruct it) we check the flags to see if it should be updated,
			update it if necessary, and increment a counter so we access the
			next component.

			*/

			int j = 0;

			if (joints[i].flags & MD5_ANIM_XPOS) {//X component of Position
				animPos.x = frame.components[joints[i].frameIndex + j];
				++j;
			}

			if (joints[i].flags & MD5_ANIM_YPOS) {//Y component of Position
				animPos.y = frame.components[joints[i].frameIndex + j];
				++j;
			}

			if (joints[i].flags & MD5_ANIM_ZPOS) {//Z component of Position
				animPos.z = frame.components[joints[i].frameIndex + j];
				++j;
			}

			if (joints[i].flags & MD5_ANIM_XQUAT) {//X component of Orientation
				animQuat.x = frame.components[joints[i].frameIndex + j];
				++j;
			}

			if (joints[i].flags & MD5_ANIM_YQUAT) {//Y component of Orientation
				animQuat.y = frame.components[joints[i].frameIndex + j];
				++j;
			}

			if (joints[i].flags & MD5_ANIM_ZQUAT) {//Z component of Orientation
				animQuat.z = frame.components[joints[i].frameIndex + j];
				++j;
			}

			animQuat.GenerateW(); //We only get updated x,y,z so must generate W again...
			animQuat.Normalise(); //And we should probably normalise it, too, to keep to unit length

								  //now we have a copy of the baseframe joint transformed to the animation pose, we can start
								  //applying it to the input skeleton.

								  //First, let's get a reference to the skeleton joint equating to the current baseframe joint
			MD5Joint &skelJoint = skel.joints[i];

			//I'm fairly sure this doesn't ever actually change...
			skelJoint.parent = joints[i].parent;
			skelJoint.forceWorld = false;

			//We'll set its position and orientation to the transformed baseframe variables

			skelJoint.position = animPos;
			skelJoint.orientation = animQuat;

			//Now to set the local transform of the current joint. We start by turning the orientation
			//quaternion into a Matrix4, then we set the resulting matrix translation to the
			//transformed baseframe position

			skelJoint.localTransform = animQuat.ToMatrix();
			skelJoint.localTransform.SetPositionVector(animPos);

			//If the joint has no parent (determined by a negative parent variable) we need to 
			//transform the joint's transform to the correct rotation, using the conversion matrix
			if (skelJoint.parent < 0) {	//Base Joint, so we're done
				skelJoint.transform = MD5FileData::conversionMatrix * skelJoint.localTransform;
			}
			else {
				//If this joint /does/ have a parent, we transform the joint's transform by its
				//parent transform. Note that we don't have to transform it by the conversion matrix
				//again, as the parent node will already contain it, due to being propagated from 
				//the root node. Matrices are fun!
				MD5Joint &parent = skel.joints[skelJoint.parent];
				skelJoint.transform = parent.transform * skelJoint.localTransform;
			}
		}
	}


}