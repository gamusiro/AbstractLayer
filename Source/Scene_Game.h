/*===================================================================================
*    Date : 2023/01/05(木)
*        Author		: Gakuto.S
*        File		: SceneGame.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Scene.h"

class ObjectTable;
class ObjectPin;
class ObjectPins;
class ObjectCursors;
class ISoundController;
class ObjectSprite;

class SceneGame : public IScene
{
public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	SceneGame();

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~SceneGame();

public:
	//**************************************************
	/// \brief 初期化処理
	/// 
	/// \return なし
	//**************************************************
	virtual void Init() override;

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
	virtual void Update() override;

	//**************************************************
	/// \brief 描画処理
	/// 
	/// \return なし
	//**************************************************
	void Draw() override;

protected:
	//**************************************************
	/// \brief ゲームオブジェクトの初期化処理
	/// 
	/// \return なし
	//**************************************************
	void gameObjectInit();

	//**************************************************
	/// \brief ピンの設定処理
	/// 
	/// \return なし
	//**************************************************
	void updatePinControl(int column);

	//**************************************************
	/// \brief 表示するUIを追加
	/// 
	/// \return なし
	//**************************************************
	virtual void drawUI() {};

protected:
	ObjectTable*		m_table;
	IObject3D*			m_plane;
	ObjectPins*			m_pins;
	ObjectCursors*		m_cursors;
	IObject3D*			m_comitButton;
	ObjectSprite*		m_hitSprite;
	ObjectSprite*		m_browSprite;
	ObjectSprite*		m_hitRefSprite;
	ObjectSprite*		m_browRefSprite;

	ISoundController*	m_gameBGM;

	unsigned int		m_frameCounter;
	bool				m_drawButton;
	bool				m_finish;

protected:
	int					m_hitCount;
	int					m_browCount;

private:
	bool				m_changeColumn;
};

