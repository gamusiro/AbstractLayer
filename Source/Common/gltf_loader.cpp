/*===================================================================================
*    Date : 2022/12/11(��)
*        Author		: Gakuto.S
*        File		: gltf_loader.cpp
*        Detail		:
===================================================================================*/
#define _CRT_SECURE_NO_WARNINGS
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION 
#include "../ThirdLib/tiny_gltf.h"

#include "gltf_loader.h"
using namespace std;


// �}�N����`
#define GLTF_ATTRIBUTE_POSITION			"POSITION"
#define GLTF_ATTRIBUTE_NORMAL			"NORMAL"
#define GLTF_ATTRIBUTE_TEXCOORD0		"TEXCOORD_0"
#define GLTF_MATERIAL_ALPAHMODE_BLEND	"BLEND"


// �ċA�֐�

// map<���b�V���̖��O, ���b�V�����\������f�[�^>
// ���b�V�������擾���܂�
void GetMeshInfomation(const tinygltf::Node& node, const tinygltf::Model& model, map<string, GLTF_MESH>& meshes, const string& parentMeshName)
{
	const tinygltf::Mesh& mesh = model.meshes[node.mesh];

	for (auto& primitive : mesh.primitives)
	{// �A�g���r���[�g���璸�_���Ɋ܂܂��f�[�^���擾����

		GLTF_PRIMITIVE gltfPrimitve{};

		for (auto& attribute : primitive.attributes)
		{// ���_�f�[�^�����o��
			auto& acs = model.accessors[attribute.second];
			auto& bfv = model.bufferViews[acs.bufferView];
			auto& buf = model.buffers[bfv.buffer];

			// �f�[�^���Ƃ肠�������o��
			vector<unsigned char>source(bfv.byteLength);
			copy(buf.data.begin() + bfv.byteOffset,
				buf.data.begin() + bfv.byteOffset + bfv.byteLength,
				source.begin()
			);

			if (attribute.first == GLTF_ATTRIBUTE_POSITION)
			{// �|�W�V�����̎擾
				gltfPrimitve.pos.resize(acs.count);
				memcpy(gltfPrimitve.pos.data(), source.data(), source.size());
			}
			else if (attribute.first == GLTF_ATTRIBUTE_NORMAL)
			{// �@���̎擾
				gltfPrimitve.nor.resize(acs.count);
				memcpy(gltfPrimitve.nor.data(), source.data(), source.size());
			}
			else if (attribute.first == GLTF_ATTRIBUTE_TEXCOORD0)
			{// UV�̎擾
				gltfPrimitve.tex0.resize(acs.count);
				memcpy(gltfPrimitve.tex0.data(), source.data(), source.size());
			}
		}

		// �f�[�^�����݂��Ȃ��ꍇ�́A���_�̃f�[�^���ɍ��킹��
		if (gltfPrimitve.nor.size() == 0) gltfPrimitve.nor.resize(gltfPrimitve.pos.size());
		if (gltfPrimitve.tex0.size() == 0) gltfPrimitve.tex0.resize(gltfPrimitve.pos.size());

		{// �C���f�b�N�X�̎擾
			auto& acs = model.accessors[primitive.indices];
			auto& bfv = model.bufferViews[acs.bufferView];
			auto& buf = model.buffers[bfv.buffer];

			gltfPrimitve.indices.resize(bfv.byteLength);
			copy(buf.data.begin() + bfv.byteOffset,
				buf.data.begin() + bfv.byteOffset + bfv.byteLength,
				gltfPrimitve.indices.begin()
			);

			// �C���f�b�N�X�f�[�^�̃o�C�g�T�C�Y���擾
			gltfPrimitve.perIndexSize = tinygltf::GetComponentSizeInBytes(acs.componentType);
		}

		if (primitive.material >= 0)
		{// �}�e���A���̎擾
			auto& material = model.materials[primitive.material];
			gltfPrimitve.material.emissive	= { float(material.emissiveFactor[0]), float(material.emissiveFactor[1]) , float(material.emissiveFactor[2]) };
			gltfPrimitve.material.color		= { float(material.pbrMetallicRoughness.baseColorFactor[0]), float(material.pbrMetallicRoughness.baseColorFactor[1]) , float(material.pbrMetallicRoughness.baseColorFactor[2]), float(material.pbrMetallicRoughness.baseColorFactor[3]) };
			gltfPrimitve.material.metallic	= float(material.pbrMetallicRoughness.metallicFactor);
			gltfPrimitve.material.roughness	= float(material.pbrMetallicRoughness.roughnessFactor);

			if (material.pbrMetallicRoughness.baseColorTexture.index >= 0)
			{// �x�[�X�J���[�̃e�N�X�`��������ꍇ
				auto& texture	= model.textures[material.pbrMetallicRoughness.baseColorTexture.index];
				auto& image		= model.images[texture.source];
				
				gltfPrimitve.material.baseColorTextureMeta = image.image;
			}
		}

		meshes[mesh.name].primitives.push_back(gltfPrimitve);
	}

	// ���[�J�����W�f�[�^�̎擾
	GLTF_LOCAL localPoint{};
	localPoint.translate = node.translation;
	localPoint.rotate = node.rotation;
	localPoint.scale = node.scale;

	if (node.translation.empty())	localPoint.translate.resize(3);
	if (node.rotation.empty())		localPoint.rotate.resize(4);
	if (node.scale.empty())			
	{
		localPoint.scale.resize(3);
		localPoint.scale[0] = localPoint.scale[1] = localPoint.scale[2] = 1.0f;
	}

	//localPoint.translate[1] *= -1;
	 
	// Y���W�𔽓]������(+YUp�ŏo�͂�������)
	localPoint.translate[2] *= -1;
	meshes[mesh.name].localeParam = localPoint;

	// �e�̖��O��o�^���Ă���
	meshes[mesh.name].parentMeshName = parentMeshName;
}

// �q�m�[�h�T��
void SearchChildNode(const tinygltf::Node& node, const tinygltf::Model& model, map<string, GLTF_MESH>& meshes, const string& parentMeshName)
{
	string currentlyMeshName = model.meshes[node.mesh].name;

	for (const int& childIndex : node.children)
	{
		tinygltf::Node child = model.nodes[childIndex];
		SearchChildNode(child, model, meshes, currentlyMeshName);
	}

	// ���b�V���f�[�^�̎擾
	GetMeshInfomation(node, model, meshes, parentMeshName);
	return;
}


GLTFLoader::GLTFLoader(const char* fileName, bool binary)
{
	tinygltf::TinyGLTF loader;
	tinygltf::Model model;
	string error, warning;

	// �ǂݍ���
	bool success = false;
	if (binary)
		success = loader.LoadBinaryFromFile(&model, &error, &warning, fileName);
	else
		success = loader.LoadASCIIFromFile(&model, &error, &warning, fileName);

	if (!success || !error.empty() || !warning.empty())
		throw "�ǂݍ��݃G���[";

	// �V�[���̎擾
	const tinygltf::Scene& scene = model.scenes[model.defaultScene];

	// �e�q�֌W�̃f�[�^��T��
	for (const int& parentIndex : scene.nodes)
	{
		tinygltf::Node parent = model.nodes[parentIndex];
		SearchChildNode(parent, model, m_meshes, string());
	}

	int a = 0;
}
