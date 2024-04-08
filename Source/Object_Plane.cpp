/*===================================================================================
*    Date : 2022/12/02(金)
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

	// 頂点バッファの生成
	m_vbo = graphics->CreateVertexBuffer(sizeof(vertices), sizeof(VERTEX_3D));
	m_vbo->Map(vertices, sizeof(vertices));

	// インデックスバッファの生成
	m_ebo = graphics->CreateIndexBuffer(sizeof(indices), _countof(indices), INDEX_FORMAT::UINT32);
	m_ebo->Map(indices, sizeof(indices));

	// 定数バッファの作成
	m_constantBuffer = graphics->CreateShaderReource(sizeof(graphics::MATRICES), 0);

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
			shaderDesc.vsName		= "planeVS";
			shaderDesc.vsEntryName	= "main";
			shaderDesc.psName		= "planePS";
			shaderDesc.psEntryName	= "main";
		}

		// パイプラインの作成
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

	// サンプラーの破棄
	graphics->DestroySampler(m_sampler);

	// テクスチャの破棄
	graphics->DestroyTexture(m_texture);

	// 定数バッファの破棄
	graphics->DestroyShaderResource(m_constantBuffer);

	// パイプラインの破棄
	graphics->DestroyPipeline(m_pipeline);

	// インデックスバッファの破棄
	graphics->DestroyIndexBuffer(m_ebo);

	// 頂点バッファの破棄
	graphics->DestroyVertexBuffer(m_vbo);
}

void ObjectPlane::Draw()
{
	IGraphics* graphics = Application::Get()->Graphics();

	graphics::MATRICES shaderParams{};
	{// "ShanderParams" 更新処理
		shaderParams.world		= graphics->CulculateWorldMatrix(m_position, m_rotate, m_scale);
		shaderParams.view		= graphics->CulculateViewMatrix();
		shaderParams.projection = graphics->CulculateProjectionMatrix();
	}
	m_constantBuffer->Map(&shaderParams, sizeof(graphics::MATRICES));

	// パイプラインの設定
	graphics->SetPipeline(m_pipeline);

	// 頂点バッファの設定
	graphics->SetVertexBuffer(m_vbo);

	// インデックスバッファの設定
	graphics->SetIndexBuffer(m_ebo);

	// 描画呼び出し
	graphics->DrawIndexed(m_ebo->k_INDEX_NUM, 1);
}
