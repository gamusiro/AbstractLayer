/*===================================================================================
*    Date : 2023/01/15(日)
*        Author		: Gakuto.S
*        File		: Object_Cursor.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Object.h"

class ObjectTable;
class ISoundController;

class ObjectCursors : public IObject3D
{
public:
	enum class COMIT_TYPE
	{
		ERASE,
		PUTON,
		CHECK,
		NONE
	};


public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	ObjectCursors(ObjectTable* table)
		:m_table(table),
		m_colorIndex(0),
		m_rowIndex(0),
		m_colorCursor(),
		m_columnCursor(),
		m_degree(),
		m_currentlyColumn(),
		m_currentlyColumnFull(false),
		m_comitCursor(),
		m_cursorMoveSE(),
		m_comitSE()
	{
	}

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
	void Update(bool key = true);

	//**************************************************
	/// \brief 描画処理
	/// 
	/// \return なし
	//**************************************************
	void Draw() override;

public:
	//**************************************************
	/// \brief カラーインデックスの取得処理
	/// 
	/// \return カラーインデックス
	//**************************************************
	int GetColorIndex() { return m_colorIndex; }

	//**************************************************
	/// \brief ピンホールインデックスの取得処理
	/// 
	/// \return ピンホールインデックス
	//**************************************************
	int GetPinWholeIndex();

	//**************************************************
	/// \brief 列インデックスの取得処理
	/// 
	/// \return 列インデックス
	//**************************************************
	int GetCurrentlyColumn() { return m_currentlyColumn; }

	//**************************************************
	/// \brief 行インデックスの取得処理
	/// 
	/// \return 行インデックス
	//**************************************************
	int GetCurrentlyRow() { return m_rowIndex; }

	//**************************************************
	/// \brief 行インデックスの設定処理
	/// 
	/// \return なし
	//**************************************************
	void SetCurrentlyRow(int row, bool sound);

	//**************************************************
	/// \brief 行インデックスの設定処理
	/// 
	/// \return なし
	//**************************************************
	void SetColorIndex(int color, bool sound);

	//**************************************************
	/// \brief 列インデックスの設定処理
	/// 
	/// \return なし
	//**************************************************
	void SetCurrentlyColumn(int column);

	//**************************************************
	/// \brief 4つピンが刺さっているか設定する
	/// 
	/// \return なし
	//**************************************************
	void SetCurrentlyColumnFull(bool full) { m_currentlyColumnFull = full; }

	//**************************************************
	/// \brief 決定ボタンが押されたときの処理
	/// 
	/// \return true 決定
	//**************************************************
	COMIT_TYPE GetComitButton(bool increment = true);

	//**************************************************
	/// \brief カーソルの移動操作が行われた
	/// 
	/// VK_LEFT	 : 左
	/// VK_RIGHT : 右
	/// VK_DOWN  : 下
	/// VK_UP    : 上
	/// 
	/// \return -1は不正 
	//**************************************************
	int GetKeyDir() { return m_keyCode; }

private:
	IObject3D*			m_columnCursor;
	IObject3D*			m_colorCursor;
	IObject3D*			m_comitCursor;
	ObjectTable*		m_table;
	ISoundController*	m_cursorMoveSE;
	ISoundController*	m_comitSE;
	
	int			m_currentlyColumn;		// 0 ~ 8
	int			m_rowIndex;				// 0 ~ 5
	int			m_colorIndex;			// 0 ~ 6
	bool		m_currentlyColumnFull;
	int			m_keyCode;				// オンライン用に押された方向を示すデータを格納する変数

	float		m_degree;

public:
	const DirectX::XMFLOAT3 k_SCALE{2.0f, 2.0f, 2.0f};
};

