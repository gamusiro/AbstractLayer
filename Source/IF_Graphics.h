/*===================================================================================
*    Date : 2022/11/28(��)
*        Author		: Gakuto.S
*        File		: IF_Graphics.h
*        Detail		:
===================================================================================*/
#pragma once

// �}�N����`
#define GRAPHICS_DEFAULT_FOV	  1.0f
#define GRAPHICS_DEFAULT_NEAR_Z	  0.1f
#define GRAPHICS_DEFAULT_FAR_Z	100.0f

// �w���p�[�C���N���[�h
#include <DirectXMath.h>

#include "Common/FileIO.h"

// �W���C���N���[�h
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <map>

// ���ۃI�u�W�F�N�g
#include "IF_VertexBufferObject.h"
#include "IF_ElementBufferObject.h"
#include "IF_Pipeline.h"
#include "IF_ShaderResource.h"
#include "IF_Texture.h"
#include "IF_Sampler.h"

namespace graphics
{
	//**************************************************
	/// \brief ���_�\����
	//**************************************************
	struct VERTEX_3D
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord;
	};

	//**************************************************
	/// \brief �萔�s��
	//**************************************************
	struct MATRICES
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	//**************************************************
	/// \brief �}�e���A�����
	//**************************************************
	struct MATERIAL
	{
		DirectX::XMFLOAT4	albedo;
		float				metalic;
		float				roughness;
	};

	//**************************************************
	/// \brief ���C�g���
	//**************************************************
	struct LIGHT
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 direction;
	};
	
	//**************************************************
	/// \brief �J�������
	//**************************************************
	struct CAMERA
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 focus;
		DirectX::XMFLOAT3 up;
	};
}


class IGraphics
{
public:
	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual ~IGraphics() {}


		////////	���ۉ����\�b�h	////////
public:
	//**************************************************
	/// \brief ��ʃN���A����
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void Clear() = 0;

	//**************************************************
	/// \brief �o�b�t�@�؂�ւ�����
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void Present() = 0;

	//**************************************************
	/// \brief ���_�o�b�t�@�̐�������
	/// 
	/// \param[in] size
	/// \param[in] stride
	/// 
	/// \return ���_�o�b�t�@�|�C���^
	//**************************************************
	virtual IVertexBufferObject* CreateVertexBuffer(
		size_t size,
		size_t stride
	) = 0;

	//**************************************************
	/// \brief ���_�o�b�t�@�̐ݒ菈��
	/// 
	/// \param[in] vertexBuffer
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void SetVertexBuffer(IVertexBufferObject* vertexBuffer) = 0;

	//**************************************************
	/// \brief ���_�o�b�t�@�̉������
	/// 
	/// \param[in] vertexBuffer
	/// 
	/// \return �Ȃ�
	//**************************************************
	void DestroyVertexBuffer(IVertexBufferObject* vertexBuffer) { delete vertexBuffer; }

	//**************************************************
	/// \brief �C���f�b�N�X�o�b�t�@�̐�������
	/// 
	/// \param[in] size
	/// \param[in] stride
	/// 
	/// \return �C���f�b�N�X�o�b�t�@�|�C���^
	//**************************************************
	virtual IElementBufferObject* CreateIndexBuffer(
		size_t size,
		unsigned int countIndex,
		INDEX_FORMAT format
	) = 0;

	//**************************************************
	/// \brief �C���f�b�N�X�o�b�t�@�̐ݒ菈��
	/// 
	/// \param[in] elementBuffer
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void SetIndexBuffer(IElementBufferObject* elementBuffer) = 0;

	//**************************************************
	/// \brief �C���f�b�N�X�o�b�t�@�̉������
	/// 
	/// \param[in] elementBuffer
	/// 
	/// \return �Ȃ�
	//**************************************************
	void DestroyIndexBuffer(IElementBufferObject* elementBuffer) { delete elementBuffer; }

	//**************************************************
	/// \brief �p�C�v���C���̍쐬����
	/// 
	/// \param[in] layouts
	/// \param[in] blendDesc
	/// \param[in] rasterizerDesc
	/// \param[in] shaderDesc
	/// 
	/// \return �p�C�v���C��
	//**************************************************
	virtual IPipeline* CreatePipeline(
		size_t stride,
		std::vector<pipeline::vertexLayout::VERTEX_LAYOUT_DESC> layouts,
		pipeline::blend::BLEND_DESC blendDesc,
		pipeline::rasterizer::RASTERIZER_DESC rasterizerDesc,
		pipeline::primitive::PRIMITIVE_TOPOLOGY primitive,
		pipeline::shader::SHADER_DESC shaderDesc,
		std::vector<IShaderResource*> shaderResources,
		std::vector<ITexture*>  textureResources,
		std::vector<ISampler*>	samplerResources
	) = 0;

	//**************************************************
	/// \brief �p�C�v���C���̐ݒ菈��
	/// 
	/// \param[in] pipeline
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void SetPipeline(
		IPipeline* pipeline
	) = 0;

	//**************************************************
	/// \brief �p�C�v���C���̉������
	/// 
	/// \param[in] pipeline
	/// 
	/// \return �Ȃ�
	//**************************************************
	void DestroyPipeline(IPipeline* pipeline) { delete pipeline; };

	//**************************************************
	/// \brief �J�����f�[�^�̐ݒ�
	/// 
	/// \param[in] param
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetCamera(const graphics::CAMERA& param) { m_camera = param; }

	//**************************************************
	/// \brief ����p�̐ݒ�
	/// 
	/// \param[in] fov
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetFov(float fov) { m_fov = fov; }

	//**************************************************
	/// \brief nearZ�̐ݒ�
	/// 
	/// \param[in] nearZ
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetNearZ(float nearZ) { m_nearZ = nearZ; }

	//**************************************************
	/// \brief farZ�̐ݒ�
	/// 
	/// \param[in] farZ
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetFarZ(float farZ) { m_farZ = farZ; }

	//**************************************************
	/// \brief �萔�o�b�t�@�̍쐬����
	/// 
	/// \param[in] size
	/// \param[in] slotNum
	/// 
	/// \return �V�F�[�_���\�[�X
	//**************************************************
	virtual IShaderResource* CreateShaderReource(
		size_t size,
		unsigned int slotNum
	) = 0;

	//**************************************************
	/// \brief �萔�o�b�t�@�̔j������
	/// 
	/// \param[in] srv
	/// 
	/// \return �Ȃ�
	//**************************************************
	void DestroyShaderResource(IShaderResource* srv) { delete srv; }

	//**************************************************
	/// \brief �e�N�X�`���̍쐬����
	/// 
	/// \param[in] fileName
	/// \param[in] slotNum
	/// 
	/// \return �e�N�X�`�����\�[�X
	//**************************************************
	virtual ITexture* CreateTextureFromFile(
		const char* fileName,
		unsigned int slotNum
	) = 0;

	//**************************************************
	/// \brief �e�N�X�`���̔j������
	/// 
	/// \param[in] texture
	/// 
	/// \return �Ȃ�
	//**************************************************
	void DestroyTexture(ITexture* texture) { delete texture; }

	//**************************************************
	/// \brief �T���v���[�̍쐬����
	/// 
	/// \param[in] samplerDesc
	/// \param[in] slotNum
	/// 
	/// \return �T���v���[
	//**************************************************
	virtual ISampler* CreateSampler(
		sampler::SAMPLER_DESC samplerDesc,
		unsigned int binding
	) = 0;

	//**************************************************
	/// \brief �T���v���[�̔j������
	/// 
	/// \param[in] sampler
	/// 
	/// \return �Ȃ�
	//**************************************************
	void DestroySampler(ISampler* sampler) { delete sampler; }

	//**************************************************
	/// \brief ���f���}�g���b�N�X�̍쐬
	/// 
	/// \param[in] position
	/// \param[in] rotate
	/// \param[in] scale
	/// 
	/// \return ���f���}�g���b�N�X
	//**************************************************
	virtual DirectX::XMMATRIX CulculateWorldMatrix(
		const DirectX::XMFLOAT3 position,
		const DirectX::XMFLOAT3 rotate,
		const DirectX::XMFLOAT3 scale
	)
	{// Direct�n�p
		DirectX::XMMATRIX trl, rot, scl;
		trl = DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&position));
		//rot = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotate));

		auto test = DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&rotate));
		rot = DirectX::XMMatrixRotationQuaternion(test);

		scl = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&scale));

		return DirectX::XMMatrixTranspose(scl * rot * trl);
	}

	//**************************************************
	/// \brief �J�����}�g���b�N�X�̍쐬
	/// 
	/// \return �J�����}�g���b�N�X
	//**************************************************
	virtual DirectX::XMMATRIX CulculateViewMatrix()
	{// Direct�n�p
		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
			XMLoadFloat3(&m_camera.position),
			XMLoadFloat3(&m_camera.focus),
			XMLoadFloat3(&m_camera.up)
		);

		return DirectX::XMMatrixTranspose(view);
	}

	//**************************************************
	/// \brief �v���W�F�N�V�����}�g���b�N�X�̍쐬
	/// 
	/// \param[in] fov
	/// \param[in] nearZ
	/// \param[in] farZ
	/// 
	/// \return �v���W�F�N�V�����}�g���b�N�X
	//**************************************************
	virtual DirectX::XMMATRIX CulculateProjectionMatrix(bool perspective = true)
	{// Direct�n�p
		DirectX::XMMATRIX proj{};
		if (perspective)
		{
			 proj = DirectX::XMMatrixPerspectiveFovLH(
				m_fov,
				m_aspect,
				m_nearZ,
				m_farZ
			);
		}
		else
		{
			proj = DirectX::XMMatrixOrthographicOffCenterLH(
				0.0f,
				k_SCREEN_UI_WIDTH,
				k_SCREEN_UI_HEIGHT,
				0.0f,
				0.0f,
				1.0f
			);
		}
		
		return DirectX::XMMatrixTranspose(proj);
	}

	//**************************************************
	/// \brief �e�q�s��̏�Z�̍쐬
	/// 
	/// \return �s��
	//**************************************************
	virtual DirectX::XMMATRIX Multiply(DirectX::XMMATRIX parentMat, DirectX::XMMATRIX localMat)
	{
		return parentMat * localMat;
	}


	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \param[in] numIndex
	/// \param[in] numInstance
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void DrawIndexed(
		unsigned int numIndex,
		unsigned int numInstance
	) = 0;

	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \param[in] numVertex
	/// \param[in] numInstance
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void DrawInstanced(
		unsigned int numVertex,
		unsigned int numInstance
	) = 0;


	//**************************************************
	/// \brief ��ʃ��T�C�Y����
	/// 
	/// \param[in] width
	/// \param[in] height
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void Resize(int width, int height) = 0;


public:
	constexpr static unsigned int k_BUFFER_NUM = 2;	// �_�u���o�b�t�@�����O
	constexpr static float k_SCREEN_UI_WIDTH	= 1280.0f;
	constexpr static float k_SCREEN_UI_HEIGHT	=  720.0f;
	
protected:
	float m_fov		= GRAPHICS_DEFAULT_FOV;
	float m_aspect	= {};
	float m_nearZ	= GRAPHICS_DEFAULT_NEAR_Z;
	float m_farZ	= GRAPHICS_DEFAULT_FAR_Z;

	graphics::CAMERA m_camera;
};

