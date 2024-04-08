/*===================================================================================
*    Date : 2022/01/24(��)
*        Author		: Gakuto.S
*        File		: Object_Sprite.cpp
*        Detail		:
===================================================================================*/
#include "Object_Sprite.h"

#include "Application.h"
using namespace DirectX;
using namespace graphics;
using namespace std;

void ObjectSprite::Init()
{
	IGraphics* graphics = Application::Get()->Graphics();

	// ���_�o�b�t�@�̐���
	m_vbo = graphics->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, sizeof(VERTEX_3D));
	//m_vbo->Map(vertices, sizeof(vertices));

	// �萔�o�b�t�@�̍쐬
	m_constantBuffer = graphics->CreateShaderReource(sizeof(graphics::MATRICES), 0);
	m_materialBuffer = graphics->CreateShaderReource(sizeof(graphics::MATERIAL), 1);

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
			shaderDesc.vsName		= "spliteVS";
			shaderDesc.vsEntryName	= "main";
			shaderDesc.psName		= "splitePS";
			shaderDesc.psEntryName	= "main";
		}

		// �p�C�v���C���̍쐬
		m_pipeline = graphics->CreatePipeline(
			m_vbo->k_STRIDE,
			layouts,
			blendDesc,
			rasterizerDesc,
			pipeline::primitive::TRIANGLE_STRIP,
			shaderDesc,
			{ m_constantBuffer, m_materialBuffer},
			{ m_texture },
			{ m_sampler }
		);
	}

	// �����J���[�ݒ�
	m_material.albedo = { 1.0f, 1.0f, 1.0f, m_alpha };
}

void ObjectSprite::Uninit()
{
	IGraphics* graphics = Application::Get()->Graphics();

	// �T���v���[�̔j��
	graphics->DestroySampler(m_sampler);

	// �e�N�X�`���̔j��
	graphics->DestroyTexture(m_texture);

	// �萔�o�b�t�@�̔j��
	graphics->DestroyShaderResource(m_materialBuffer);
	graphics->DestroyShaderResource(m_constantBuffer);

	// �p�C�v���C���̔j��
	graphics->DestroyPipeline(m_pipeline);

	// ���_�o�b�t�@�̔j��
	graphics->DestroyVertexBuffer(m_vbo);
}

void ObjectSprite::Update()
{
}

void ObjectSprite::Draw()
{
	IGraphics* graphics = Application::Get()->Graphics();

	// ���_�̏������ݏ���
	float x = m_uvSize.x * m_uvStart.x;
	float y = m_uvSize.y * m_uvStart.y;

	VERTEX_3D vertices[]
	{
		{ XMFLOAT3(-0.5f, -0.5f, 0.0f), {}, {x, y + m_uvSize.y} },				// ����
		{ XMFLOAT3( 0.5f, -0.5f, 0.0f), {}, {x - m_uvSize.x, y + m_uvSize.y} },	// �E��
		{ XMFLOAT3(-0.5f,  0.5f, 0.0f), {},	{x, y} },							// ����
		{ XMFLOAT3( 0.5f,  0.5f, 0.0f), {}, {x - m_uvSize.x, y} },				// �E��
	};
	m_vbo->Map(vertices, sizeof(VERTEX_3D) * 4);

	// �}�e���A���̏�������
	m_material.albedo.w = m_alpha;
	m_materialBuffer->Map(&m_material, sizeof(graphics::MATERIAL));

	graphics::MATRICES shaderParams{};
	{// "ShanderParams" �X�V����
		shaderParams.world		= graphics->CulculateWorldMatrix(m_position, {0, 0, XMConvertToRadians(180.0f + m_degree)}, {m_size.x, m_size.y, 0});
		shaderParams.view		= DirectX::XMMatrixIdentity();
		shaderParams.projection = graphics->CulculateProjectionMatrix(false);
	}
	m_constantBuffer->Map(&shaderParams, sizeof(graphics::MATRICES));

	// �p�C�v���C���̐ݒ�
	graphics->SetPipeline(m_pipeline);

	// ���_�o�b�t�@�̐ݒ�
	graphics->SetVertexBuffer(m_vbo);

	// �`��Ăяo��
	graphics->DrawInstanced(4, 1);
}
