/*===================================================================================
*    Date : 2022/11/28(��)
*        Author		: Gakuto.S
*        File		: IF_PipelineObject.h
*        Detail		:
===================================================================================*/
#pragma once
#include <string>
#include <vector>


namespace pipeline
{
	namespace vertexLayout
	{
		//**************************************************
		/// \brief �u�����h
		//**************************************************
		enum class FORMAT
		{
			R32G32B32_FLOAT,
			R32G32_FLOAT,
		};

		//**************************************************
		/// \brief ���_���C�A�E�g
		//**************************************************
		struct VERTEX_LAYOUT_DESC
		{
			const char*		semanticName;
			unsigned int	binding;
			FORMAT			format;
			unsigned int	offset;
		};
	}

	namespace blend
	{
		//**************************************************
		/// \brief �u�����h
		//**************************************************
		enum class BLEND
		{
			ZERO,
			ONE,
			SRC_ALPHA,
			INV_SRC_ALPHA
		};

		//**************************************************
		/// \brief �u�����h���@
		//**************************************************
		enum class BLEND_OP
		{
			ADD
		};

		//**************************************************
		/// \brief ���W�b�N���@
		//**************************************************
		enum class LOGIC_OP
		{
			NONE,
			CLEAR
		};

		//**************************************************
		/// \brief �V�F�[�_�̍\����
		//**************************************************
		struct BLEND_DESC
		{
			bool            blendEnable;
			bool			logicOpEnable;
			BLEND			srcColorBlend;
			BLEND			dstColorBlend;
			BLEND_OP		colorBlendOp;
			BLEND			srcAlphaBlend;
			BLEND			dstAlphaBlend;
			BLEND_OP		alphaBlendOp;
			LOGIC_OP		logicOp;
			unsigned char	colorWriteMask;
		};
	}

	namespace rasterizer
	{
		//**************************************************
		/// \brief �J�����O���[�h
		//**************************************************
		enum class CULL_MODE
		{
			NONE,
			FRONT,
			BACK
		};

		//**************************************************
		/// \brief �t�B�����[�h
		//**************************************************
		enum class FILL_MODE
		{
			WIREFRAME,
			SOLID
		};

		//**************************************************
		/// \brief ���X�^���C�U�X�e�[�g�̍\����
		//**************************************************
		struct RASTERIZER_DESC
		{
			CULL_MODE cMode;
			FILL_MODE fMode;
		};
	}

	namespace primitive
	{
		typedef enum PRIMITIVE_TOPOLOGY
		{
			TRIANGLE_LIST,
			TRIANGLE_STRIP,

		}PRIMITIVE_TOPOROGY;
	}

	namespace depthStencil
	{

	}

	namespace shader
	{
		//**************************************************
		/// \brief �V�F�[�_�̍\����
		//**************************************************
		struct SHADER_DESC
		{
			std::string vsName;
			std::string vsEntryName;
			std::string psName;
			std::string psEntryName;
		};
	}
}

class IPipeline
{
public:
	//**************************************************
	/// \brief �f�X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual ~IPipeline() {}
};

