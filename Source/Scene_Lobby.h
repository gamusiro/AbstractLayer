/*===================================================================================
*    Date : 2023/01/26(木)
*        Author		: Gakuto.S
*        File		: Scene_Lobby.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Scene.h"

class ObjectSprite;

class SceneLobby : public IScene
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	SceneLobby(bool join);

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~SceneLobby();

public:
	//**************************************************
	/// \brief 更新処理
	/// 
	/// \return なし
	//**************************************************
	void Init() override;

	//**************************************************
	/// \brief 更新処理
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

public:
	//**************************************************
	/// \brief クライアントが来た時
	/// 
	/// \return なし
	//**************************************************
	void Join(bool host);

	//**************************************************
	/// \brief どちらかが部屋を離れた
	/// 
	/// \return なし
	//**************************************************
	void Leave()		{ m_joinClient = false;  m_joinHost = false; }

private:
	ObjectSprite*	m_host;
	ObjectSprite*	m_client;
	ObjectSprite*	m_hostDummy;
	ObjectSprite*	m_clientDummy;
	ObjectSprite*	m_hostOption;
	ObjectSprite*	m_clientOption;
	ObjectSprite*	m_backTitle;

	bool			m_joinHost;
	bool			m_joinClient;
};

