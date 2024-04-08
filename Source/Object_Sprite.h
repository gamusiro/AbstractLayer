/*===================================================================================
*    Date : 2022/01/24(��)
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
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
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
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
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
	/// \brief ����������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Init() override;

	//**************************************************
	/// \brief �I������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Uninit() override;

	//**************************************************
	/// \brief �X�V����
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Update() override;

	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Draw() override;

public:
	//**************************************************
	/// \brief �T�C�Y�̐ݒ菈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetSize(const DirectX::XMFLOAT2& size) { m_size = size; }

	//**************************************************
	/// \brief �T�C�Y�̎擾����
	/// 
	/// \return �T�C�Y
	//**************************************************
	DirectX::XMFLOAT2 GetSize() { return m_size; }

	//**************************************************
	/// \brief �p�x�ݒ�
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetRotate(float degree) { m_degree = degree; }

	//**************************************************
	/// \brief UV���W�̐ݒ�
	/// 
	/// \return �Ȃ�
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

