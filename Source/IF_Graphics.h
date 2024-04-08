/*===================================================================================
*    Date : 2022/11/28(月)
*        Author		: Gakuto.S
*        File		: IF_Graphics.h
*        Detail		:
===================================================================================*/
#pragma once

// マクロ定義
#define GRAPHICS_DEFAULT_FOV	  1.0f
#define GRAPHICS_DEFAULT_NEAR_Z	  0.1f
#define GRAPHICS_DEFAULT_FAR_Z	100.0f

// ヘルパーインクルード
#include <DirectXMath.h>

#include "Common/FileIO.h"

// 標準インクルード
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <map>

// 抽象オブジェクト
#include "IF_VertexBufferObject.h"
#include "IF_ElementBufferObject.h"
#include "IF_Pipeline.h"
#include "IF_ShaderResource.h"
#include "IF_Texture.h"
#include "IF_Sampler.h"

namespace graphics
{
	//**************************************************
	/// \brief 頂点構造体
	//**************************************************
	struct VERTEX_3D
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord;
	};

	//**************************************************
	/// \brief 定数行列
	//**************************************************
	struct MATRICES
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	//**************************************************
	/// \brief マテリアル情報
	//**************************************************
	struct MATERIAL
	{
		DirectX::XMFLOAT4	albedo;
		float				metalic;
		float				roughness;
	};

	//**************************************************
	/// \brief ライト情報
	//**************************************************
	struct LIGHT
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 direction;
	};
	
	//**************************************************
	/// \brief カメラ情報
	//**************************************************
	struct CAMERA
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 focus;
		DirectX::XMFLOAT3 up;
	};
}


class IGraphics
{
public:
	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	virtual ~IGraphics() {}


		////////	抽象化メソッド	////////
public:
	//**************************************************
	/// \brief 画面クリア処理
	/// 
	/// \return なし
	//**************************************************
	virtual void Clear() = 0;

	//**************************************************
	/// \brief バッファ切り替え処理
	/// 
	/// \return なし
	//**************************************************
	virtual void Present() = 0;

	//**************************************************
	/// \brief 頂点バッファの生成処理
	/// 
	/// \param[in] size
	/// \param[in] stride
	/// 
	/// \return 頂点バッファポインタ
	//**************************************************
	virtual IVertexBufferObject* CreateVertexBuffer(
		size_t size,
		size_t stride
	) = 0;

	//**************************************************
	/// \brief 頂点バッファの設定処理
	/// 
	/// \param[in] vertexBuffer
	/// 
	/// \return なし
	//**************************************************
	virtual void SetVertexBuffer(IVertexBufferObject* vertexBuffer) = 0;

	//**************************************************
	/// \brief 頂点バッファの解放処理
	/// 
	/// \param[in] vertexBuffer
	/// 
	/// \return なし
	//**************************************************
	void DestroyVertexBuffer(IVertexBufferObject* vertexBuffer) { delete vertexBuffer; }

	//**************************************************
	/// \brief インデックスバッファの生成処理
	/// 
	/// \param[in] size
	/// \param[in] stride
	/// 
	/// \return インデックスバッファポインタ
	//**************************************************
	virtual IElementBufferObject* CreateIndexBuffer(
		size_t size,
		unsigned int countIndex,
		INDEX_FORMAT format
	) = 0;

	//**************************************************
	/// \brief インデックスバッファの設定処理
	/// 
	/// \param[in] elementBuffer
	/// 
	/// \return なし
	//**************************************************
	virtual void SetIndexBuffer(IElementBufferObject* elementBuffer) = 0;

	//**************************************************
	/// \brief インデックスバッファの解放処理
	/// 
	/// \param[in] elementBuffer
	/// 
	/// \return なし
	//**************************************************
	void DestroyIndexBuffer(IElementBufferObject* elementBuffer) { delete elementBuffer; }

	//**************************************************
	/// \brief パイプラインの作成処理
	/// 
	/// \param[in] layouts
	/// \param[in] blendDesc
	/// \param[in] rasterizerDesc
	/// \param[in] shaderDesc
	/// 
	/// \return パイプライン
	//**************************************************
	virtual IPipeline* CreatePipeline(
		size_t stride,
		std::vector<pipeline::vertexLayout::VERTEX_LAYOUT_DESC> layouts,
		pipeline::blend::BLEND_DESC blendDesc,
		pipeline::rasterizer::RASTERIZER_DESC rasterizerDesc,
		pipeline::primitive::PRIMITIVE_TOPOLOGY primitive,
		pipeline::shader::SHADER_DESC shaderDesc,
		std::vector<IShaderResource*> shaderResources,
		std::vector<ITexture*>  textureResources,
		std::vector<ISampler*>	samplerResources
	) = 0;

	//**************************************************
	/// \brief パイプラインの設定処理
	/// 
	/// \param[in] pipeline
	/// 
	/// \return なし
	//**************************************************
	virtual void SetPipeline(
		IPipeline* pipeline
	) = 0;

	//**************************************************
	/// \brief パイプラインの解放処理
	/// 
	/// \param[in] pipeline
	/// 
	/// \return なし
	//**************************************************
	void DestroyPipeline(IPipeline* pipeline) { delete pipeline; };

	//**************************************************
	/// \brief カメラデータの設定
	/// 
	/// \param[in] param
	/// 
	/// \return なし
	//**************************************************
	void SetCamera(const graphics::CAMERA& param) { m_camera = param; }

	//**************************************************
	/// \brief 視野角の設定
	/// 
	/// \param[in] fov
	/// 
	/// \return なし
	//**************************************************
	void SetFov(float fov) { m_fov = fov; }

	//**************************************************
	/// \brief nearZの設定
	/// 
	/// \param[in] nearZ
	/// 
	/// \return なし
	//**************************************************
	void SetNearZ(float nearZ) { m_nearZ = nearZ; }

	//**************************************************
	/// \brief farZの設定
	/// 
	/// \param[in] farZ
	/// 
	/// \return なし
	//**************************************************
	void SetFarZ(float farZ) { m_farZ = farZ; }

	//**************************************************
	/// \brief 定数バッファの作成処理
	/// 
	/// \param[in] size
	/// \param[in] slotNum
	/// 
	/// \return シェーダリソース
	//**************************************************
	virtual IShaderResource* CreateShaderReource(
		size_t size,
		unsigned int slotNum
	) = 0;

	//**************************************************
	/// \brief 定数バッファの破棄処理
	/// 
	/// \param[in] srv
	/// 
	/// \return なし
	//**************************************************
	void DestroyShaderResource(IShaderResource* srv) { delete srv; }

	//**************************************************
	/// \brief テクスチャの作成処理
	/// 
	/// \param[in] fileName
	/// \param[in] slotNum
	/// 
	/// \return テクスチャリソース
	//**************************************************
	virtual ITexture* CreateTextureFromFile(
		const char* fileName,
		unsigned int slotNum
	) = 0;

	//**************************************************
	/// \brief テクスチャの破棄処理
	/// 
	/// \param[in] texture
	/// 
	/// \return なし
	//**************************************************
	void DestroyTexture(ITexture* texture) { delete texture; }

	//**************************************************
	/// \brief サンプラーの作成処理
	/// 
	/// \param[in] samplerDesc
	/// \param[in] slotNum
	/// 
	/// \return サンプラー
	//**************************************************
	virtual ISampler* CreateSampler(
		sampler::SAMPLER_DESC samplerDesc,
		unsigned int binding
	) = 0;

	//**************************************************
	/// \brief サンプラーの破棄処理
	/// 
	/// \param[in] sampler
	/// 
	/// \return なし
	//**************************************************
	void DestroySampler(ISampler* sampler) { delete sampler; }

	//**************************************************
	/// \brief モデルマトリックスの作成
	/// 
	/// \param[in] position
	/// \param[in] rotate
	/// \param[in] scale
	/// 
	/// \return モデルマトリックス
	//**************************************************
	virtual DirectX::XMMATRIX CulculateWorldMatrix(
		const DirectX::XMFLOAT3 position,
		const DirectX::XMFLOAT3 rotate,
		const DirectX::XMFLOAT3 scale
	)
	{// Direct系用
		DirectX::XMMATRIX trl, rot, scl;
		trl = DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&position));
		//rot = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotate));

		auto test = DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&rotate));
		rot = DirectX::XMMatrixRotationQuaternion(test);

		scl = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&scale));

		return DirectX::XMMatrixTranspose(scl * rot * trl);
	}

	//**************************************************
	/// \brief カメラマトリックスの作成
	/// 
	/// \return カメラマトリックス
	//**************************************************
	virtual DirectX::XMMATRIX CulculateViewMatrix()
	{// Direct系用
		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
			XMLoadFloat3(&m_camera.position),
			XMLoadFloat3(&m_camera.focus),
			XMLoadFloat3(&m_camera.up)
		);

		return DirectX::XMMatrixTranspose(view);
	}

	//**************************************************
	/// \brief プロジェクションマトリックスの作成
	/// 
	/// \param[in] fov
	/// \param[in] nearZ
	/// \param[in] farZ
	/// 
	/// \return プロジェクションマトリックス
	//**************************************************
	virtual DirectX::XMMATRIX CulculateProjectionMatrix(bool perspective = true)
	{// Direct系用
		DirectX::XMMATRIX proj{};
		if (perspective)
		{
			 proj = DirectX::XMMatrixPerspectiveFovLH(
				m_fov,
				m_aspect,
				m_nearZ,
				m_farZ
			);
		}
		else
		{
			proj = DirectX::XMMatrixOrthographicOffCenterLH(
				0.0f,
				k_SCREEN_UI_WIDTH,
				k_SCREEN_UI_HEIGHT,
				0.0f,
				0.0f,
				1.0f
			);
		}
		
		return DirectX::XMMatrixTranspose(proj);
	}

	//**************************************************
	/// \brief 親子行列の乗算の作成
	/// 
	/// \return 行列
	//**************************************************
	virtual DirectX::XMMATRIX Multiply(DirectX::XMMATRIX parentMat, DirectX::XMMATRIX localMat)
	{
		return parentMat * localMat;
	}


	//**************************************************
	/// \brief 描画処理
	/// 
	/// \param[in] numIndex
	/// \param[in] numInstance
	/// 
	/// \return なし
	//**************************************************
	virtual void DrawIndexed(
		unsigned int numIndex,
		unsigned int numInstance
	) = 0;

	//**************************************************
	/// \brief 描画処理
	/// 
	/// \param[in] numVertex
	/// \param[in] numInstance
	/// 
	/// \return なし
	//**************************************************
	virtual void DrawInstanced(
		unsigned int numVertex,
		unsigned int numInstance
	) = 0;


	//**************************************************
	/// \brief 画面リサイズ処理
	/// 
	/// \param[in] width
	/// \param[in] height
	/// 
	/// \return なし
	//**************************************************
	virtual void Resize(int width, int height) = 0;


public:
	constexpr static unsigned int k_BUFFER_NUM = 2;	// ダブルバッファリング
	constexpr static float k_SCREEN_UI_WIDTH	= 1280.0f;
	constexpr static float k_SCREEN_UI_HEIGHT	=  720.0f;
	
protected:
	float m_fov		= GRAPHICS_DEFAULT_FOV;
	float m_aspect	= {};
	float m_nearZ	= GRAPHICS_DEFAULT_NEAR_Z;
	float m_farZ	= GRAPHICS_DEFAULT_FAR_Z;

	graphics::CAMERA m_camera;
};

