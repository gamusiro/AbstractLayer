/*===================================================================================
*    Date : 2022/11/29(Thurs)
*        Author		: Gakuto.S
*        File		: Object_TexCube.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Object.h"

class ObjectPlane : public IObject3D
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	ObjectPlane()
		:m_fileName("Lenna.png")
	{}

	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	ObjectPlane(std::string fileName)
		:m_fileName(fileName)
	{}

public:
	//**************************************************
	/// \brief ����������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Init();

	//**************************************************
	/// \brief �I������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Uninit();

	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Draw();

private:
	IVertexBufferObject*	m_vbo{};
	IElementBufferObject*	m_ebo{};
	IShaderResource*		m_constantBuffer{};
	ITexture*				m_texture{};
	ISampler*				m_sampler{};
	IPipeline*				m_pipeline{};

private:
	std::string m_fileName;
};

