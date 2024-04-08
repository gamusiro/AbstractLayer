/*===================================================================================
*    Date : 2022/11/28(月)
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
		/// \brief ブレンド
		//**************************************************
		enum class FORMAT
		{
			R32G32B32_FLOAT,
			R32G32_FLOAT,
		};

		//**************************************************
		/// \brief 頂点レイアウト
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
		/// \brief ブレンド
		//**************************************************
		enum class BLEND
		{
			ZERO,
			ONE,
			SRC_ALPHA,
			INV_SRC_ALPHA
		};

		//**************************************************
		/// \brief ブレンド方法
		//**************************************************
		enum class BLEND_OP
		{
			ADD
		};

		//**************************************************
		/// \brief ロジック方法
		//**************************************************
		enum class LOGIC_OP
		{
			NONE,
			CLEAR
		};

		//**************************************************
		/// \brief シェーダの構造体
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
		/// \brief カリングモード
		//**************************************************
		enum class CULL_MODE
		{
			NONE,
			FRONT,
			BACK
		};

		//**************************************************
		/// \brief フィルモード
		//**************************************************
		enum class FILL_MODE
		{
			WIREFRAME,
			SOLID
		};

		//**************************************************
		/// \brief ラスタライザステートの構造体
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
		/// \brief シェーダの構造体
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
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	virtual ~IPipeline() {}
};

