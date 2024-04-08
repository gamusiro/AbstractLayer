/*===================================================================================
*    Date : 2022/12/02(��)
*        Author		: Gakuto.S
*        File		: Object_TexCube.cpp
*        Detail		:
===================================================================================*/
#include "Object_Plane.h"

#include "Application.h"
using namespace DirectX;
using namespace graphics;
using namespace std;

static VERTEX_3D vertices[]
{
    // bottom
	{ XMFLOAT3(-0.5f, 0.0f, 0.5f), {}, {0, 0} },
	{ XMFLOAT3(-0.5f, 0.0f,-0.5f), {}, {0, 1} },
	{ XMFLOAT3( 0.5f, 0.0f, 0.5f), {}, {1, 0} },
	{ XMFLOAT3( 0.5f, 0.0f,-0.5f), {}, {1, 1} },
};

static uint32_t indices[] = {
   0, 1, 2, 
   1, 2, 3,
};

void ObjectPlane::Init()
{
	IGraphics* graphics = Application::Get()->Graphics();

	// ���_�o�b�t�@�̐���
	m_vbo = graphics->CreateVertexBuffer(sizeof(vertices), sizeof(VERTEX_3D));
	m_vbo->Map(vertices, sizeof(vertices));

	// �C���f�b�N�X�o�b�t�@�̐���
	m_ebo = graphics->CreateIndexBuffer(sizeof(indices), _countof(indices), INDEX_FORMAT::UINT32);
	m_ebo->Map(indices, sizeof(indices));

	// �萔�o�b�t�@�̍쐬
	m_constantBuffer = graphics->CreateShaderReource(sizeof(graphics::MATRICES), 0);

	// �e�N�X�`���̍쐬
	m_texture = graphics->CreateTextureFromFile((std::string("Material\\Texture\\") + m_fileName).c_str(), 0);

	// �T���v���[�̍쐬
	sampler::SAMPLER_DESC desc{};
	m_sampler = graphics->CreateSampler(desc, 0);

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
			blendDesc.srcColorBlend		= pipeline::blend::BLEND::SRC_ALPHA;
			blendDesc.dstColorBlend		= pipeline::blend::BLEND::INV_SRC_ALPHA;
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
			shaderDesc.vsName		= "planeVS";
			shaderDesc.vsEntryName	= "main";
			shaderDesc.psName		= "planePS";
			shaderDesc.psEntryName	= "main";
		}

		// �p�C�v���C���̍쐬
		m_pipeline = graphics->CreatePipeline(
			m_vbo->k_STRIDE,
			layouts,
			blendDesc,
			rasterizerDesc,
			pipeline::primitive::TRIANGLE_LIST,
			shaderDesc,
			{ m_constantBuffer},
			{ m_texture },
			{ m_sampler }
		);
	}
}

void ObjectPlane::Uninit()
{
	IGraphics* graphics = Application::Get()->Graphics();

	// �T���v���[�̔j��
	graphics->DestroySampler(m_sampler);

	// �e�N�X�`���̔j��
	graphics->DestroyTexture(m_texture);

	// �萔�o�b�t�@�̔j��
	graphics->DestroyShaderResource(m_constantBuffer);

	// �p�C�v���C���̔j��
	graphics->DestroyPipeline(m_pipeline);

	// �C���f�b�N�X�o�b�t�@�̔j��
	graphics->DestroyIndexBuffer(m_ebo);

	// ���_�o�b�t�@�̔j��
	graphics->DestroyVertexBuffer(m_vbo);
}

void ObjectPlane::Draw()
{
	IGraphics* graphics = Application::Get()->Graphics();

	graphics::MATRICES shaderParams{};
	{// "ShanderParams" �X�V����
		shaderParams.world		= graphics->CulculateWorldMatrix(m_position, m_rotate, m_scale);
		shaderParams.view		= graphics->CulculateViewMatrix();
		shaderParams.projection = graphics->CulculateProjectionMatrix();
	}
	m_constantBuffer->Map(&shaderParams, sizeof(graphics::MATRICES));

	// �p�C�v���C���̐ݒ�
	graphics->SetPipeline(m_pipeline);

	// ���_�o�b�t�@�̐ݒ�
	graphics->SetVertexBuffer(m_vbo);

	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	graphics->SetIndexBuffer(m_ebo);

	// �`��Ăяo��
	graphics->DrawIndexed(m_ebo->k_INDEX_NUM, 1);
}
