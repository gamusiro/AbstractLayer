/*===================================================================================
*    Date : 2023/01/20(金)
*        Author		: Gakuto.S
*        File		: Object_Result.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Scene.h"

class ObjectTable;
class ObjectPin;
class ObjectPins;
class ISoundController;
class ObjectSprite;

class SceneResult : public IScene
{
private:
	enum class RESULT_STATE
	{
		NONE,
		WIN,
		LOSE
	};


public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	SceneResult(
		ObjectTable* table,
		ObjectPins* pins,
		IObject3D* plane,
		int finishColumn,
		int myTurn = -1,
		bool lobby = false
	);

	//**************************************************
	/// \brief デストラクタ
	/// 
	/// \return なし
	//**************************************************
	~SceneResult();

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
	//**************************************************
	/// \brief リザルトの音声処理
	/// 
	/// \return なし
	//**************************************************
	void resultSound(int myTurn);

private:
	ObjectTable*		m_table;
	ObjectPins*			m_pins;
	IObject3D*			m_plane;
	ObjectSprite*		m_winSprite;
	ObjectSprite*		m_loseSprite;

	ISoundController*	m_correctSE;
	ISoundController*	m_successSE;
	ISoundController*	m_failedSE;
	int					m_finishColumn;
	int					m_rowIndex;
	int					m_correctCount;
	int					m_myTurn;
	unsigned int		m_frameCount;
	RESULT_STATE		m_result;

	const bool			k_Lobby;
	const unsigned int	k_UpdateFrame;
};

