/*===================================================================================
*    Date : 2023/01/11(��)
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
	/// \brief ����������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Init() override;

	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Draw(const DirectX::XMMATRIX& parentMatrix) override;

private:
	graphics::MATERIAL m_material;
};

