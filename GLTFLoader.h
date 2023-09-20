#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <iostream>

#include "../NCLCoreClasses/Assets.h"
#include "../NCLCoreClasses/Mesh.h"
#include "../NCLCoreClasses/MeshAnimation.h"
#include "../NCLCoreClasses/Texture.h"
#include "../NCLCoreClasses/Matrix4.h"

namespace tinygltf {
	class Model;
}

namespace NCL::Assets {
	const std::string GLTFDIR(ASSETROOT + "GLTF/");
}

namespace NCL::Rendering {
	class MeshAnimation;
	class Texture;



	class GLTFLoader	{
	public:
		typedef std::function<NCL::Rendering::Mesh* (void)>				MeshConstructionFunction;
		typedef std::function<NCL::Rendering::Texture* (std::string&)>	TextureConstructionFunction;

		struct GLTFMaterialLayer {
			SharedTexture diffuse;
			SharedTexture bump;
			SharedTexture occlusion;
			SharedTexture emission;
			SharedTexture metallic;

			GLTFMaterialLayer() {
			}
		};

		struct GLTFMaterial {
			std::vector< GLTFMaterialLayer > allLayers;
		};

		GLTFLoader();
		~GLTFLoader();

		std::vector<SharedMesh>			outMeshes;
		std::vector<SharedTexture>		outTextures;
		std::vector<GLTFMaterial>		outMats;
		std::vector<SharedMeshAnim>		outAnims;
		
		void Load(const std::string& filename, MeshConstructionFunction meshConstructor, TextureConstructionFunction textureConstruction);
		
	protected:
		struct GLTFSkin {
			std::vector<int>			nodesUsed;
			std::vector<Maths::Matrix4>	worldBindPose;
			std::vector<Maths::Matrix4> worldInverseBindPose;
			Maths::Matrix4				globalTransformInverse;
		};

		void LoadImages(tinygltf::Model& m, const std::string& rootFile, TextureConstructionFunction texFunc);
		void LoadMaterials(tinygltf::Model& m);
		void LoadSceneNodeData(tinygltf::Model& m);

		void LoadVertexData(tinygltf::Model& m, GLTFLoader::MeshConstructionFunction meshConstructor);

		void LoadSkinningData(tinygltf::Model& m, Mesh& geometry);
		void LoadAnimationData(tinygltf::Model& m, Mesh& mesh, GLTFSkin& skin);

		std::map<int, int>				parentChildNodeLookup;		
		std::vector<Maths::Matrix4>		localNodeMatrices;
		std::vector<Maths::Matrix4>		worldNodeMatrices;	
		std::vector<GLTFMaterialLayer>	fileMats;

		MeshConstructionFunction	meshConstructor;
		TextureConstructionFunction textureConstruction;
	};
}
