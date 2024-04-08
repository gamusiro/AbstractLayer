/*===================================================================================
*    Date : 2022/12/03(�y)
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

// �O���錾
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
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	IObject()
		:m_position()
	{}

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual ~IObject() {}

public:
	//**************************************************
	/// \brief �|�W�V�����̐ݒ�
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetPosition(DirectX::XMFLOAT3 position) { m_position = position; }

	//**************************************************
	/// \brief �|�W�V�����̎擾
	/// 
	/// \return �|�W�V����
	//**************************************************
	DirectX::XMFLOAT3 GetPosition() { return m_position; }

	//**************************************************
	/// \brief ����������
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void Init() = 0;

	//**************************************************
	/// \brief �I������
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void Uninit() = 0;

	//**************************************************
	/// \brief �X�V����
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void Update() {};

	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void Draw() = 0;

protected:
	DirectX::XMFLOAT3 m_position{};
};

class IObject3D : public IObject
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	IObject3D()
		:IObject(),
		m_rotate(),
		m_scale({ 1.0f, 1.0f, 1.0f })
	{}

	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual ~IObject3D() {}

public:
	//**************************************************
	/// \brief �|�W�V�����̐ݒ�
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetRotate(DirectX::XMFLOAT3 rotate) { m_rotate = rotate; }

	//**************************************************
	/// \brief �|�W�V�����̎擾
	/// 
	/// \return �|�W�V����
	//**************************************************
	DirectX::XMFLOAT3 GetRotate() { return m_rotate; }

	//**************************************************
	/// \brief �|�W�V�����̐ݒ�
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetScale(DirectX::XMFLOAT3 scale) { m_scale = scale; }

	//**************************************************
	/// \brief �|�W�V�����̎擾
	/// 
	/// \return �|�W�V����
	//**************************************************
	DirectX::XMFLOAT3 GetScale() { return m_scale; }

	

protected:
	DirectX::XMFLOAT3 m_rotate{};
	DirectX::XMFLOAT3 m_scale{1, 1, 1};
};

