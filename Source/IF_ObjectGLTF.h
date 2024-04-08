/*===================================================================================
*    Date : 2022/12/18(��)
*        Author		: Gakuto.S
*        File		: IF_ObjectGLTF.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Object.h"
#include "Application.h"
#include "Common/gltf_loader.h"

class IObjectGLTF : public IObject3D
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	///	
	/// \return �Ȃ�
	//**************************************************
	IObjectGLTF()
		:IObject3D()
	{}

	//**************************************************
	/// \brief �f�X�g���N�^
	///	
	/// \return �Ȃ�
	//**************************************************
	virtual ~IObjectGLTF()
	{
	}


public:
	//**************************************************
	/// \brief ����������
	///	
	/// \return �Ȃ�
	//**************************************************]
	virtual void Init() = 0;

	//**************************************************
	/// \brief �I������
	///	
	/// \return �Ȃ�
	//**************************************************
	virtual void Uninit()
	{
		this->DestroyMeshes();
	}

	//**************************************************
	/// \brief �X�V����
	///	
	/// \return �Ȃ�
	//**************************************************
	virtual void Update()
	{
	}

	//**************************************************
	/// \brief �`�揈��
	///	
	/// \return �Ȃ�
	//**************************************************
	virtual void Draw()
	{
		this->DrawMeshes(DirectX::XMMatrixIdentity());
	}
	virtual void Draw(const DirectX::XMMATRIX& parentMatrix)
	{
		this->DrawMeshes(parentMatrix);
	}


protected:
	//**************************************************
	/// \brief ���b�V���I�u�W�F�N�g�̕`��
	//**************************************************
	virtual void DrawMeshes(const DirectX::XMMATRIX& parentMatrix)
	{
		IGraphics* graphics = Application::Get()->Graphics();

		for (auto& mesh : m_objects)
		{
			// �萔�f�[�^�̏�������
			graphics::MATRICES shaderParams{};
			{// "ShanderParams" �X�V����
				shaderParams.world		= graphics->Multiply(parentMatrix, graphics->CulculateWorldMatrix(m_position, m_rotate, m_scale));
				shaderParams.view		= graphics->CulculateViewMatrix();
				shaderParams.projection = graphics->CulculateProjectionMatrix();
			}
			mesh.second.localBuffer->Map(&shaderParams, sizeof(shaderParams));

			for (auto& primitive : mesh.second.primitives)
			{
				// �p�C�v���C���̐ݒ�
				graphics->SetPipeline(primitive.pipeline);

				// ���_�o�b�t�@�̐ݒ�
				graphics->SetVertexBuffer(primitive.vbo);

				// �C���f�b�N�X�o�b�t�@�̐ݒ�
				graphics->SetIndexBuffer(primitive.ebo);

				// �`��Ăяo��
				graphics->DrawIndexed(primitive.ebo->k_INDEX_NUM, 1);
			}
		}
	}

	//**************************************************
	/// \brief ���b�V���I�u�W�F�N�g�̔j��
	//**************************************************
	void DestroyMeshes()
	{
		IGraphics* graphics = Application::Get()->Graphics();

		for (auto& mesh : m_objects)
		{
			for (auto& primitive : mesh.second.primitives)
			{
				graphics->DestroyVertexBuffer(primitive.vbo);
				graphics->DestroyIndexBuffer(primitive.ebo);
				graphics->DestroyShaderResource(primitive.materialBuffer);
				graphics->DestroyPipeline(primitive.pipeline);
			}
			graphics->DestroyShaderResource(mesh.second.localBuffer);
		}
	}

protected:
	//**************************************************
	/// \brief �v���~�e�B�u�I�u�W�F�N�g
	//**************************************************
	struct PRIMITIVE_OBJECT
	{
		IVertexBufferObject*	vbo;
		IElementBufferObject*	ebo;
		IShaderResource*		materialBuffer;
		IPipeline*				pipeline;
	};

	//**************************************************
	/// \brief ���b�V���I�u�W�F�N�g
	//**************************************************
	struct MESH_OBJECT
	{
		std::vector<PRIMITIVE_OBJECT>	primitives;
		IShaderResource*				localBuffer;
	};

	std::string							m_fileName;
	std::map<std::string, MESH_OBJECT>	m_objects;
};

