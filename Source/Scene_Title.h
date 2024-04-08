/*===================================================================================
*    Date : 2023/01/05(木)
*        Author		: Gakuto.S
*        File		: Scene_Title.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Scene.h"

class ObjectSprite;
class ObjectTable;
class ISoundController;

class SceneTitle : public IScene
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	SceneTitle();

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~SceneTitle();

public:
	//**************************************************
	/// \brief 初期化処理
	/// 
	/// \return なし
	//**************************************************
	void Init() override;

	//**************************************************
	/// \brief 終了処理
	/// 
	/// \return なし
	//**************************************************
	void Uninit() override;

	//**************************************************
	/// \brief 更新処理
	/// 
	/// \return なし
	//**************************************************
	void Update() override;

	//**************************************************
	/// \brief 描画処理
	/// 
	/// \return なし
	//**************************************************
	void Draw() override;

private:
	ObjectSprite*			m_soloModeSprite;
	ObjectSprite*			m_onlineModeSprite;
	ObjectSprite*			m_titleSprite;

	ISoundController*		m_bgmController;

	std::vector<IObject3D*> m_objects;
	float					m_radian;

	const DirectX::XMFLOAT3 k_PLANE_SCALE = { 30.0f, 1.0f, 30.0f };
	const DirectX::XMFLOAT2 k_DEFAULT_SPRITE_SIZE = {400.0f, 100.0f};
	const DirectX::XMFLOAT2 k_PREVIEW_SPRITE_SIZE = {450.0f, 125.0f};
	const int				k_SPRITE_NUM = 2;
	int						m_cursorIndex;
};

