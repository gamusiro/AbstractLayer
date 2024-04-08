/*===================================================================================
*    Date : 2022/01/24(火)
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

	// 頂点バッファの生成
	m_vbo = graphics->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, sizeof(VERTEX_3D));
	//m_vbo->Map(vertices, sizeof(vertices));

	// 定数バッファの作成
	m_constantBuffer = graphics->CreateShaderReource(sizeof(graphics::MATRICES), 0);
	m_materialBuffer = graphics->CreateShaderReource(sizeof(graphics::MATERIAL), 1);

	// テクスチャの作成
	m_texture = graphics->CreateTextureFromFile((std::string("Material\\Texture\\") + m_fileName).c_str(), 0);

	// サンプラーの作成
	sampler::SAMPLER_DESC desc{};
	m_sampler = graphics->CreateSampler(desc, 0);

	{// パイプラインの作成処理
		
		// 頂点レイアウトの設定
		vector<pipeline::vertexLayout::VERTEX_LAYOUT_DESC> layouts
		{
			{"POSITION", 0, pipeline::vertexLayout::FORMAT::R32G32B32_FLOAT,	 0},
			{"NORMAL",	 0, pipeline::vertexLayout::FORMAT::R32G32B32_FLOAT,	12},
			{"TEXCOORD", 0, pipeline::vertexLayout::FORMAT::R32G32_FLOAT,		24},
		};

		pipeline::blend::BLEND_DESC blendDesc{};
		{// ブレンドステートの設定
			blendDesc.blendEnable		= true;
			blendDesc.colorWriteMask	= 0xf;
			blendDesc.srcColorBlend		= pipeline::blend::BLEND::SRC_ALPHA;
			blendDesc.dstColorBlend		= pipeline::blend::BLEND::INV_SRC_ALPHA;
			blendDesc.srcAlphaBlend		= pipeline::blend::BLEND::ONE;
			blendDesc.dstAlphaBlend		= pipeline::blend::BLEND::ZERO;
			blendDesc.colorBlendOp		= pipeline::blend::BLEND_OP::ADD;
			blendDesc.alphaBlendOp		= pipeline::blend::BLEND_OP::ADD;
		}

		{// プリミティブの設定
			
		}

		pipeline::rasterizer::RASTERIZER_DESC rasterizerDesc{};
		{// ラスタライザの設定
			rasterizerDesc.cMode = pipeline::rasterizer::CULL_MODE::NONE;
			rasterizerDesc.fMode = pipeline::rasterizer::FILL_MODE::SOLID;
		}

		{// サンプルの設定

		}

		{// 深度ステートの設定

		}

		pipeline::shader::SHADER_DESC shaderDesc{};
		{// シェーダ情報の指定
			shaderDesc.vsName		= "spliteVS";
			shaderDesc.vsEntryName	= "main";
			shaderDesc.psName		= "splitePS";
			shaderDesc.psEntryName	= "main";
		}

		// パイプラインの作成
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

	// 初期カラー設定
	m_material.albedo = { 1.0f, 1.0f, 1.0f, m_alpha };
}

void ObjectSprite::Uninit()
{
	IGraphics* graphics = Application::Get()->Graphics();

	// サンプラーの破棄
	graphics->DestroySampler(m_sampler);

	// テクスチャの破棄
	graphics->DestroyTexture(m_texture);

	// 定数バッファの破棄
	graphics->DestroyShaderResource(m_materialBuffer);
	graphics->DestroyShaderResource(m_constantBuffer);

	// パイプラインの破棄
	graphics->DestroyPipeline(m_pipeline);

	// 頂点バッファの破棄
	graphics->DestroyVertexBuffer(m_vbo);
}

void ObjectSprite::Update()
{
}

void ObjectSprite::Draw()
{
	IGraphics* graphics = Application::Get()->Graphics();

	// 頂点の書き込み処理
	float x = m_uvSize.x * m_uvStart.x;
	float y = m_uvSize.y * m_uvStart.y;

	VERTEX_3D vertices[]
	{
		{ XMFLOAT3(-0.5f, -0.5f, 0.0f), {}, {x, y + m_uvSize.y} },				// 左下
		{ XMFLOAT3( 0.5f, -0.5f, 0.0f), {}, {x - m_uvSize.x, y + m_uvSize.y} },	// 右下
		{ XMFLOAT3(-0.5f,  0.5f, 0.0f), {},	{x, y} },							// 左上
		{ XMFLOAT3( 0.5f,  0.5f, 0.0f), {}, {x - m_uvSize.x, y} },				// 右上
	};
	m_vbo->Map(vertices, sizeof(VERTEX_3D) * 4);

	// マテリアルの書き込み
	m_material.albedo.w = m_alpha;
	m_materialBuffer->Map(&m_material, sizeof(graphics::MATERIAL));

	graphics::MATRICES shaderParams{};
	{// "ShanderParams" 更新処理
		shaderParams.world		= graphics->CulculateWorldMatrix(m_position, {0, 0, XMConvertToRadians(180.0f + m_degree)}, {m_size.x, m_size.y, 0});
		shaderParams.view		= DirectX::XMMatrixIdentity();
		shaderParams.projection = graphics->CulculateProjectionMatrix(false);
	}
	m_constantBuffer->Map(&shaderParams, sizeof(graphics::MATRICES));

	// パイプラインの設定
	graphics->SetPipeline(m_pipeline);

	// 頂点バッファの設定
	graphics->SetVertexBuffer(m_vbo);

	// 描画呼び出し
	graphics->DrawInstanced(4, 1);
}
