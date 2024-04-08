/*===================================================================================
*    Date : 2022/01/30(月)
*        Author		: Gakuto.S
*        File		: Object_Fade.h
*        Detail		:
===================================================================================*/
#pragma once
#include "Object_Sprite.h"

class IScene;

class ObjectFade : public ObjectSprite
{
public:
	enum class MODE
	{
		NONE,
		FADE_IN,
		FADE_OUT,
	};

public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	ObjectFade(const char* fileName);

public:
	//**************************************************
	/// \brief 更新処理
	/// 
	/// \return なし
	//**************************************************
	void Update() override;

	//**************************************************
	/// \brief シーンのセット
	/// 
	/// \return なし
	//**************************************************
	void SetNextScene(IScene* scene);

	//**************************************************
	/// \brief フェード状態の取得
	/// 
	/// \return true ならフェードしていない
	//**************************************************
	bool GetFadeState();

private:
	IScene* m_nextScene;
	MODE	m_mode;
	float	k_ADD_VALUE;
};

