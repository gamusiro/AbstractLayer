/*===================================================================================
*    Date : 2022/12/03(土)
*        Author		: Gakuto.S
*        File		: IF_Object.h
*        Detail		:
===================================================================================*/
#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <random>
#include <DirectXMath.h>

// 前方宣言
class IVertexBufferObject;
class IElementBufferObject;
class IPipeline;
class IShaderResource;
class ITexture;
class ISampler;

class IObject
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	IObject()
		:m_position()
	{}

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	virtual ~IObject() {}

public:
	//**************************************************
	/// \brief ポジションの設定
	/// 
	/// \return なし
	//**************************************************
	void SetPosition(DirectX::XMFLOAT3 position) { m_position = position; }

	//**************************************************
	/// \brief ポジションの取得
	/// 
	/// \return ポジション
	//**************************************************
	DirectX::XMFLOAT3 GetPosition() { return m_position; }

	//**************************************************
	/// \brief 初期化処理
	/// 
	/// \return なし
	//**************************************************
	virtual void Init() = 0;

	//**************************************************
	/// \brief 終了処理
	/// 
	/// \return なし
	//**************************************************
	virtual void Uninit() = 0;

	//**************************************************
	/// \brief 更新処理
	/// 
	/// \return なし
	//**************************************************
	virtual void Update() {};

	//**************************************************
	/// \brief 描画処理
	/// 
	/// \return なし
	//**************************************************
	virtual void Draw() = 0;

protected:
	DirectX::XMFLOAT3 m_position{};
};

class IObject3D : public IObject
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	IObject3D()
		:IObject(),
		m_rotate(),
		m_scale({ 1.0f, 1.0f, 1.0f })
	{}

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	virtual ~IObject3D() {}

public:
	//**************************************************
	/// \brief ポジションの設定
	/// 
	/// \return なし
	//**************************************************
	void SetRotate(DirectX::XMFLOAT3 rotate) { m_rotate = rotate; }

	//**************************************************
	/// \brief ポジションの取得
	/// 
	/// \return ポジション
	//**************************************************
	DirectX::XMFLOAT3 GetRotate() { return m_rotate; }

	//**************************************************
	/// \brief ポジションの設定
	/// 
	/// \return なし
	//**************************************************
	void SetScale(DirectX::XMFLOAT3 scale) { m_scale = scale; }

	//**************************************************
	/// \brief ポジションの取得
	/// 
	/// \return ポジション
	//**************************************************
	DirectX::XMFLOAT3 GetScale() { return m_scale; }

	

protected:
	DirectX::XMFLOAT3 m_rotate{};
	DirectX::XMFLOAT3 m_scale{1, 1, 1};
};

