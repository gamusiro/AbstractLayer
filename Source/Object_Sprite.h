/*===================================================================================
*    Date : 2022/01/24(火)
*        Author		: Gakuto.S
*        File		: Object_Sprite.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Graphics.h"
#include "IF_Object.h"

class ObjectSprite : public IObject
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	ObjectSprite()
		:m_fileName("Lenna.png"),
		m_degree(),
		m_size(),
		m_uvSize({1.0f, 1.0f}),
		m_uvStart(),
		m_material(),
		m_alpha(1.0f),
		IObject()
	{}

	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	ObjectSprite(std::string fileName, float column = 1.0f, float row = 1.0f)
		:m_fileName(fileName),
		m_degree(),
		m_size(),
		m_uvSize({ 1.0f / column, 1.0f / row }),
		m_uvStart(),
		m_material(),
		m_alpha(1.0f),
		IObject()
	{}

public:
	//**************************************************
	/// \brief 初期化処理
	/// 
	/// \return なし
	//**************************************************
	void Init() override;

	//**************************************************
	/// \brief 終了処理
	/// 
	/// \return なし
	//**************************************************
	void Uninit() override;

	//**************************************************
	/// \brief 更新処理
	/// 
	/// \return なし
	//**************************************************
	void Update() override;

	//**************************************************
	/// \brief 描画処理
	/// 
	/// \return なし
	//**************************************************
	void Draw() override;

public:
	//**************************************************
	/// \brief サイズの設定処理
	/// 
	/// \return なし
	//**************************************************
	void SetSize(const DirectX::XMFLOAT2& size) { m_size = size; }

	//**************************************************
	/// \brief サイズの取得処理
	/// 
	/// \return サイズ
	//**************************************************
	DirectX::XMFLOAT2 GetSize() { return m_size; }

	//**************************************************
	/// \brief 角度設定
	/// 
	/// \return なし
	//**************************************************
	void SetRotate(float degree) { m_degree = degree; }

	//**************************************************
	/// \brief UV座標の設定
	/// 
	/// \return なし
	//**************************************************
	void SetUVStart(const DirectX::XMFLOAT2& start) { m_uvStart = start; }

protected:
	IVertexBufferObject*	m_vbo{};
	IShaderResource*		m_constantBuffer{};
	IShaderResource*		m_materialBuffer{};
	ITexture*				m_texture{};
	ISampler*				m_sampler{};
	IPipeline*				m_pipeline{};
	graphics::MATERIAL		m_material;

protected:
	std::string			m_fileName;
	DirectX::XMFLOAT2	m_uvSize;
	DirectX::XMFLOAT2	m_uvStart;
	DirectX::XMFLOAT2	m_size;
	float				m_degree;
	float				m_alpha;
};

