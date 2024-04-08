/*===================================================================================
*    Date : 2023/01/11(水)
*        Author		: Gakuto.S
*        File		: Object_Pin_Whole.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_ObjectGLTF.h"

class ObjectPinWhole : public IObjectGLTF
{
public:
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
	void Draw(const DirectX::XMMATRIX& parentMatrix) override;

private:
	graphics::MATERIAL m_material;
};

