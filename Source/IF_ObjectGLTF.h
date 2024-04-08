/*===================================================================================
*    Date : 2022/12/18(日)
*        Author		: Gakuto.S
*        File		: IF_ObjectGLTF.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Object.h"
#include "Application.h"
#include "Common/gltf_loader.h"

class IObjectGLTF : public IObject3D
{
public:
	//**************************************************
	/// \brief コンストラクタ
	///	
	/// \return なし
	//**************************************************
	IObjectGLTF()
		:IObject3D()
	{}

	//**************************************************
	/// \brief デストラクタ
	///	
	/// \return なし
	//**************************************************
	virtual ~IObjectGLTF()
	{
	}


public:
	//**************************************************
	/// \brief 初期化処理
	///	
	/// \return なし
	//**************************************************]
	virtual void Init() = 0;

	//**************************************************
	/// \brief 終了処理
	///	
	/// \return なし
	//**************************************************
	virtual void Uninit()
	{
		this->DestroyMeshes();
	}

	//**************************************************
	/// \brief 更新処理
	///	
	/// \return なし
	//**************************************************
	virtual void Update()
	{
	}

	//**************************************************
	/// \brief 描画処理
	///	
	/// \return なし
	//**************************************************
	virtual void Draw()
	{
		this->DrawMeshes(DirectX::XMMatrixIdentity());
	}
	virtual void Draw(const DirectX::XMMATRIX& parentMatrix)
	{
		this->DrawMeshes(parentMatrix);
	}


protected:
	//**************************************************
	/// \brief メッシュオブジェクトの描画
	//**************************************************
	virtual void DrawMeshes(const DirectX::XMMATRIX& parentMatrix)
	{
		IGraphics* graphics = Application::Get()->Graphics();

		for (auto& mesh : m_objects)
		{
			// 定数データの書き込み
			graphics::MATRICES shaderParams{};
			{// "ShanderParams" 更新処理
				shaderParams.world		= graphics->Multiply(parentMatrix, graphics->CulculateWorldMatrix(m_position, m_rotate, m_scale));
				shaderParams.view		= graphics->CulculateViewMatrix();
				shaderParams.projection = graphics->CulculateProjectionMatrix();
			}
			mesh.second.localBuffer->Map(&shaderParams, sizeof(shaderParams));

			for (auto& primitive : mesh.second.primitives)
			{
				// パイプラインの設定
				graphics->SetPipeline(primitive.pipeline);

				// 頂点バッファの設定
				graphics->SetVertexBuffer(primitive.vbo);

				// インデックスバッファの設定
				graphics->SetIndexBuffer(primitive.ebo);

				// 描画呼び出し
				graphics->DrawIndexed(primitive.ebo->k_INDEX_NUM, 1);
			}
		}
	}

	//**************************************************
	/// \brief メッシュオブジェクトの破棄
	//**************************************************
	void DestroyMeshes()
	{
		IGraphics* graphics = Application::Get()->Graphics();

		for (auto& mesh : m_objects)
		{
			for (auto& primitive : mesh.second.primitives)
			{
				graphics->DestroyVertexBuffer(primitive.vbo);
				graphics->DestroyIndexBuffer(primitive.ebo);
				graphics->DestroyShaderResource(primitive.materialBuffer);
				graphics->DestroyPipeline(primitive.pipeline);
			}
			graphics->DestroyShaderResource(mesh.second.localBuffer);
		}
	}

protected:
	//**************************************************
	/// \brief プリミティブオブジェクト
	//**************************************************
	struct PRIMITIVE_OBJECT
	{
		IVertexBufferObject*	vbo;
		IElementBufferObject*	ebo;
		IShaderResource*		materialBuffer;
		IPipeline*				pipeline;
	};

	//**************************************************
	/// \brief メッシュオブジェクト
	//**************************************************
	struct MESH_OBJECT
	{
		std::vector<PRIMITIVE_OBJECT>	primitives;
		IShaderResource*				localBuffer;
	};

	std::string							m_fileName;
	std::map<std::string, MESH_OBJECT>	m_objects;
};

