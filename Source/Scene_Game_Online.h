/*===================================================================================
*    Date : 2023/01/26(木)
*        Author		: Gakuto.S
*        File		: Scene_Game_Online.h
*        Detail		:
===================================================================================*/
#pragma once
#include <array>
#include "Scene_Game.h"

class SceneGameOnline : public SceneGame
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	SceneGameOnline(std::array<int, 4> ans, int turn);

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~SceneGameOnline();

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
	void Update() override;

public:
	//**************************************************
	/// \brief 受信処理
	/// 
	/// \return なし
	//**************************************************
	void Receive(int* data);

private:
	//**************************************************
	/// \brief 通信メッセージ処理
	/// 
	/// \return なし
	//**************************************************
	void sendAction();

	//**************************************************
	/// \brief 表示するUIを追加
	/// 
	/// \return なし
	//**************************************************
	void drawUI() override;

	bool				m_isTurn;
	const int			k_MY_TURN;
	std::array<int, 4>	m_ans;
	ObjectSprite*		m_drawTurn;
};

