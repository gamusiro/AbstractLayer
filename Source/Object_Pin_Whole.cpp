/*===================================================================================
*    Date : 2023/01/11(水)
*        Author		: Gakuto.S
*        File		: Object_Pin_Whole.cpp
*        Detail		:
===================================================================================*/
#include "Object_Pin_Whole.h"

#include "Application.h"
#include "Common/GLTF_Loader.h"

using namespace DirectX;
using namespace graphics;
using namespace std;

void ObjectPinWhole::Init()
{
	GLTFLoader loader("Material\\Model\\pinWhole.glb", true);

	IGraphics* graphics = Application::Get()->Graphics();


	// プリミティブの数だけ頂点バッファとインデックスバッファの作成
	for (auto& mesh : loader.GetMeshes())
	{
		// 定数バッファの作成
		m_objects[mesh.first].localBuffer = graphics->CreateShaderReource(sizeof(graphics::MATRICES), 0);

		for (auto& primitive : mesh.second.primitives)
		{
			// 頂点の整頓
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

			// インデックスバッファの作成
			INDEX_FORMAT format = INDEX_FORMAT::UINT32;
			if (primitive.perIndexSize == sizeof(unsigned short))
				format = INDEX_FORMAT::UINT16;

			object.ebo = graphics->CreateIndexBuffer(primitive.indices.size(), unsigned int (primitive.indices.size() / primitive.perIndexSize), format);
			object.ebo->Map(primitive.indices.data(), primitive.indices.size());

			// マテリアルの作成
			m_material.albedo = { 0.5f, 0.5f, 0.5f, 1.0f }; // 灰色設定
			object.materialBuffer = graphics->CreateShaderReource(sizeof(MATERIAL), 1);
			//object.materialBuffer->Map(&material, sizeof(MATERIAL));

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
					blendDesc.srcColorBlend		= pipeline::blend::BLEND::ONE;
					blendDesc.dstColorBlend		= pipeline::blend::BLEND::ZERO;
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
					shaderDesc.vsName		= "pinVS";
					shaderDesc.vsEntryName	= "main";
					shaderDesc.psName		= "pinPS";
					shaderDesc.psEntryName	= "main";
				}

				// パイプラインの作成
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

		// ローカル座標を取得
		// 今回はメッシュの数が1つなのでそのままワールド座標として使う
		auto localParam = mesh.second.localeParam;
		m_position		= { float(localParam.translate[0]), float(localParam.translate[1]), float(localParam.translate[2]) };
		m_rotate		= { float(localParam.rotate[0]), float(localParam.rotate[1]), float(localParam.rotate[2]) };
		m_scale			= { float(localParam.scale[0]), float(localParam.scale[1]), float(localParam.scale[2]) };
	}
}

void ObjectPinWhole::Draw(const DirectX::XMMATRIX& parentMatrix)
{
	// プリミティブの数だけ頂点バッファとインデックスバッファの作成
	for (auto& object : m_objects)
	{
		for (auto& primitve : object.second.primitives)
		{
			primitve.materialBuffer->Map(&m_material, sizeof(MATERIAL));
		}
	}

	// 描画処理
	this->DrawMeshes(parentMatrix);
}
