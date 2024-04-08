/*===================================================================================
*    Date : 2023/01/11(êÖ)
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
	/// \brief èâä˙âªèàóù
	/// 
	/// \return Ç»Çµ
	//**************************************************
	void Init() override;

	//**************************************************
	/// \brief ï`âÊèàóù
	/// 
	/// \return Ç»Çµ
	//**************************************************
	void Draw(const DirectX::XMMATRIX& parentMatrix) override;

private:
	graphics::MATERIAL m_material;
};

