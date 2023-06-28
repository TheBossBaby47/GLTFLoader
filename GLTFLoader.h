#pragma once
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <iostream>

#include "../NCLCoreClasses/Assets.h"
#include "../NCLCoreClasses/Matrix4.h"

namespace tinygltf {
	class Model;
}

namespace NCL {
	class Mesh;
	class MeshAnimation;

	namespace Rendering {
		class Texture;
	}

	namespace Assets {
		const std::string GLTFDIR(ASSETROOT + "GLTF/");
	}

	class GLTFLoader	{
	public:
		typedef std::function<NCL::Mesh* (void)>					MeshConstructionFunction;
		typedef std::function<NCL::Rendering::Texture* (std::string&)>	TextureConstructionFunction;

		struct GLTFMaterialLayer {
			Rendering::Texture* diffuse;
			Rendering::Texture* bump;
			Rendering::Texture* occlusion;
			Rendering::Texture* emission;
			Rendering::Texture* metallic;

			GLTFMaterialLayer() {
				diffuse		= nullptr;
				bump		= nullptr;
				occlusion	= nullptr;
				emission	= nullptr;
				metallic	= nullptr;
			}
		};

		struct GLTFMaterial {
			std::vector< GLTFMaterialLayer > allLayers;
		};

		GLTFLoader(MeshConstructionFunction meshConstructor, TextureConstructionFunction textureConstruction);
		~GLTFLoader();

		std::vector<Mesh*>				outMeshes;
		std::vector<Rendering::Texture*>	outTextures;
		std::vector<GLTFMaterial>				outMats;
		std::vector<MeshAnimation*>				outAnims;	
		
		void Load(const std::string& filename);
		
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
		void LoadSkinningData(tinygltf::Model& m, Mesh* geometry);
		void LoadAnimationData(tinygltf::Model& m, Mesh* mesh, GLTFSkin& skin);

		std::map<int, int>				parentChildNodeLookup;		
		std::vector<Maths::Matrix4>		localNodeMatrices;
		std::vector<Maths::Matrix4>		worldNodeMatrices;	
		std::vector<GLTFMaterialLayer>	fileMats;

		MeshConstructionFunction	meshConstructor;
		TextureConstructionFunction textureConstruction;
	};
}
