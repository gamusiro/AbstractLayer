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
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	ObjectPlane()
		:m_fileName("Lenna.png")
	{}

	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	ObjectPlane(std::string fileName)
		:m_fileName(fileName)
	{}

public:
	//**************************************************
	/// \brief 初期化処理
	/// 
	/// \return なし
	//**************************************************
	void Init();

	//**************************************************
	/// \brief 終了処理
	/// 
	/// \return なし
	//**************************************************
	void Uninit();

	//**************************************************
	/// \brief 描画処理
	/// 
	/// \return なし
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

