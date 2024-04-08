/*===================================================================================
*    Date : 2023/01/11(水)
*        Author		: Gakuto.S
*        File		: Object_Pin_White.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_ObjectGLTF.h"

class ObjectPin : public IObjectGLTF
{
public:
	enum class COLOR
	{
		BLUE,
		RED,
		YELLOW,
		GREEN,
		PINK,
		WHITE
	};


public:
	//**************************************************
	/// \brief 初期化処理
	/// 
	/// \return なし
	//**************************************************
	ObjectPin(COLOR color, bool useMaterial = true)
		:m_type(color),
		m_useMaterial(useMaterial),
		IObjectGLTF()
	{}

	//**************************************************
	/// \brief 初期化処理
	/// 
	/// \return なし
	//**************************************************
	void Init() override;

	//**************************************************
	/// \brief 描画処理
	/// 
	/// \return なし
	//**************************************************
	void Draw() override;

	//**************************************************
	/// \brief カラーの取得
	/// 
	/// \return ピンのカラー
	//**************************************************
	COLOR GetColor() { return m_type; }

	//**************************************************
	/// \brief マテリアルの書き込みを許可
	/// 
	/// \return なし
	//**************************************************
	void SetUseMaterial(bool use) { m_useMaterial = use; }

private:
	//**************************************************
	/// \brief 色を返す
	/// 
	/// \return なし
	//**************************************************
	DirectX::XMFLOAT4 getColor();

private:
	const COLOR m_type;
	bool m_useMaterial;
};

