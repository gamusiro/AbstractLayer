/*===================================================================================
*    Date : 2023/01/14(�y)
*        Author		: Gakuto.S
*        File		: Object_Pin_Ans.cpp
*        Detail		:
===================================================================================*/
#include "Object_Pin_Ans.h"

#include "Application.h"
#include "Common/GLTF_Loader.h"

using namespace DirectX;
using namespace graphics;
using namespace std;

void ObjectPinAns::Init()
{
	GLTFLoader loader("Material\\Model\\ansPin.glb", true);

	IGraphics* graphics = Application::Get()->Graphics();
	
	// �v���~�e�B�u�̐��������_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̍쐬
	for (auto& mesh : loader.GetMeshes())
	{
		// �萔�o�b�t�@�̍쐬
		m_objects[mesh.first].localBuffer = graphics->CreateShaderReource(sizeof(graphics::MATRICES), 0);

		for (auto& primitive : mesh.second.primitives)
		{
			// ���_�̐���
			vector<VERTEX_3D> vertices(primitive.pos.size());
			for (size_t i = 0; i < vertices.size(); ++i)
			{
				array<float, 3> pos = primitive.pos[i];
				vertices[i].position = { pos[0], pos[1], pos[2] };

				array<float, 3> nor = primitive.nor[i];
				vertices[i].normal = { nor[0], nor[1], nor[2] };

				array<float, 2> tex0 = primitive.tex0[i];
				vertices[i].texcoord = { tex0[0], tex0[1] };
			}

			PRIMITIVE_OBJECT object{};
			object.vbo = graphics->CreateVertexBuffer(sizeof(VERTEX_3D) * vertices.size(), sizeof(VERTEX_3D));
			object.vbo->Map(vertices.data(), sizeof(VERTEX_3D) * vertices.size());

			// �C���f�b�N�X�o�b�t�@�̍쐬
			INDEX_FORMAT format = INDEX_FORMAT::UINT32;
			if (primitive.perIndexSize == sizeof(unsigned short))
				format = INDEX_FORMAT::UINT16;

			object.ebo = graphics->CreateIndexBuffer(primitive.indices.size(), unsigned int (primitive.indices.size() / primitive.perIndexSize), format);
			object.ebo->Map(primitive.indices.data(), primitive.indices.size());

			// �}�e���A���̍쐬
			object.materialBuffer = graphics->CreateShaderReource(sizeof(MATERIAL), 1);

			{// �p�C�v���C���̍쐬����

				// ���_���C�A�E�g�̐ݒ�
				vector<pipeline::vertexLayout::VERTEX_LAYOUT_DESC> layouts
				{
					{"POSITION", 0, pipeline::vertexLayout::FORMAT::R32G32B32_FLOAT,	 0},
					{"NORMAL",	 0, pipeline::vertexLayout::FORMAT::R32G32B32_FLOAT,	12},
					{"TEXCOORD", 0, pipeline::vertexLayout::FORMAT::R32G32_FLOAT,		24},
				};

				pipeline::blend::BLEND_DESC blendDesc{};
				{// �u�����h�X�e�[�g�̐ݒ�
					blendDesc.blendEnable		= true;
					blendDesc.colorWriteMask	= 0xf;
					blendDesc.srcColorBlend		= pipeline::blend::BLEND::ONE;
					blendDesc.dstColorBlend		= pipeline::blend::BLEND::ZERO;
					blendDesc.srcAlphaBlend		= pipeline::blend::BLEND::ONE;
					blendDesc.dstAlphaBlend		= pipeline::blend::BLEND::ZERO;
					blendDesc.colorBlendOp		= pipeline::blend::BLEND_OP::ADD;
					blendDesc.alphaBlendOp		= pipeline::blend::BLEND_OP::ADD;
				}

				{// �v���~�e�B�u�̐ݒ�
				}

				pipeline::rasterizer::RASTERIZER_DESC rasterizerDesc{};
				{// ���X�^���C�U�̐ݒ�
					rasterizerDesc.cMode = pipeline::rasterizer::CULL_MODE::NONE;
					rasterizerDesc.fMode = pipeline::rasterizer::FILL_MODE::SOLID;
				}

				{// �T���v���̐ݒ�

				}

				{// �[�x�X�e�[�g�̐ݒ�

				}

				pipeline::shader::SHADER_DESC shaderDesc{};
				{// �V�F�[�_���̎w��
					shaderDesc.vsName		= "pinVS";
					shaderDesc.vsEntryName	= "main";
					shaderDesc.psName		= "pinPS";
					shaderDesc.psEntryName	= "main";
				}

				// �p�C�v���C���̍쐬
				object.pipeline = graphics->CreatePipeline(
					sizeof(VERTEX_3D),
					layouts,
					blendDesc,
					rasterizerDesc,
					pipeline::primitive::TRIANGLE_LIST,
					shaderDesc,
					{ m_objects[mesh.first].localBuffer, object.materialBuffer },
					{},
					{}
				);

				m_objects[mesh.first].primitives.push_back(object);
			}
		}

		// ���[�J�����W���擾
		// ����̓��b�V���̐���1�Ȃ̂ł��̂܂܃��[���h���W�Ƃ��Ďg��
		auto localParam = mesh.second.localeParam;
		m_position		= { float(localParam.translate[0]), float(localParam.translate[1]), float(localParam.translate[2]) };
		m_rotate		= { float(localParam.rotate[0]), float(localParam.rotate[1]), float(localParam.rotate[2]) };
		m_scale			= { float(localParam.scale[0]), float(localParam.scale[1]), float(localParam.scale[2]) };
	}
}

void ObjectPinAns::Draw()
{
	// �v���~�e�B�u�̐��������_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̍쐬
	for (auto& object : m_objects)
	{
		for (auto& primitve : object.second.primitives)
		{
			// �}�e���A���̍쐬
			graphics::MATERIAL material{};
			material.albedo = this->getColor();
			primitve.materialBuffer->Map(&material, sizeof(MATERIAL));
		}
	}

	// �`�揈��
	this->DrawMeshes(XMMatrixIdentity());
}

DirectX::XMFLOAT4 ObjectPinAns::getColor()
{
	XMFLOAT4 retColor(1, 1, 1, 1);
	switch (m_type)
	{
	case ObjectPinAns::ANSWER::HIT:
		retColor = XMFLOAT4(0.72f, 0.48f, 0.34f, 1.0f);
		break;
	case ObjectPinAns::ANSWER::BROW:
		retColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	}

	return retColor;
}
