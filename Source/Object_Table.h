/*===================================================================================
*    Date : 2023/01/11(水)
*        Author		: Gakuto.S
*        File		: Object_Table.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Object.h"

class IObjectGLTF;

class ObjectTable : public IObject3D
{
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
	/// \brief 描画処理
	/// 
	/// \return なし
	//**************************************************
	void Draw() override;

	//**************************************************
	/// \brief ピンホールのピンをセットする位置を取得
	/// 
	/// \return 位置
	//**************************************************
	DirectX::XMFLOAT3 GetPinWholePinSetPosition(int index);

	//**************************************************
	/// \brief 回答用ピンホールのピンをセットする位置を取得
	/// 
	/// \return 位置
	//**************************************************
	DirectX::XMFLOAT3 GetHitBrowPinWholePinSetPosition(int index);

	//**************************************************
	/// \brief カラーピンホールのピンをセットする位置を取得
	/// 
	/// \return 位置
	//**************************************************
	DirectX::XMFLOAT3 GetColorPinWholePinSetPosition(int index);

	//**************************************************
	/// \brief コミットボタンやカーソルの表示位置を取得
	/// 
	/// \return 位置
	//**************************************************
	DirectX::XMFLOAT3 GetComitButtonPosition(int currentlyColumnIndex);

private:
	std::map<int, IObjectGLTF*> m_pinWholes;
	std::map<int, IObjectGLTF*> m_ansPinWholes;
	std::map<int, IObjectGLTF*>	m_colorPinWholes;
	IObject3D*					m_comitButton;

public:
	static const int k_ROW = 4;
	static const int k_COL = 8;
	static const int k_COLOR_NUM = 6;
};

