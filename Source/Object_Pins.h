/*===================================================================================
*    Date : 2023/01/14(土)
*        Author		: Gakuto.S
*        File		: Object_Pins.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Object.h"
#include "Object_Pin.h"

class IObjectGLTF;
class ObjectTable;
class ObjectPin;

class ObjectPins : public IObject3D
{
private:
	// カラーの辞書
	std::map<int, ObjectPin::COLOR> k_COLORLIST
	{
		{0, ObjectPin::COLOR::BLUE},
		{1, ObjectPin::COLOR::RED},
		{2, ObjectPin::COLOR::YELLOW},
		{3, ObjectPin::COLOR::GREEN},
		{4, ObjectPin::COLOR::PINK},
		{5, ObjectPin::COLOR::WHITE},
	};

public:
	//**************************************************
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	ObjectPins(ObjectTable* table);

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
	/// \brief 描画処理
	/// 
	/// \return なし
	//**************************************************
	void Draw() override;

public:
	//**************************************************
	/// \brief ピンの設定処理
	/// 
	/// \return なし
	//**************************************************
	void RegisterPins(int pinWholeIndex, int colorIndex);

	//**************************************************
	/// \brief ピンの設定処理
	/// 
	/// \return なし
	//**************************************************
	void UnRegisterPins(int pinWholeIndex);

	//**************************************************
	/// \brief 確定可能かどうかチェックする処理
	/// 
	/// \return 4ホール埋まっていればtrue
	//**************************************************
	bool GetPinWholeStateOfAColumn(int currentlyColumnIndex);

	//**************************************************
	/// \brief ヒット、ブローのチェック処理
	/// 
	/// \return true ならゲーム終了
	//**************************************************
	bool CheckHitOrBrow(int currentlyColumn, int& hit, int& brow);

	//**************************************************
	/// \brief 解答カラーを設定する
	/// 
	/// \return なし
	//**************************************************
	bool SetAnswerColor(int answerRowIndex, int finishColumn);

	//**************************************************
	/// \brief オンライン用、共通解答の作成処理
	/// 
	/// \return なし
	//**************************************************
	void SetCommonAnswer(std::array<int, 4> ans);

public:
	static constexpr int k_COLOR_NUM = 6;
	static constexpr DirectX::XMFLOAT3 k_PIN_SCALE = { 0.8f, 0.8f, 0.8f };

private:
	std::map<int, IObjectGLTF*>	m_hitBrowPins;
	std::map<int, ObjectPin*>	m_answerPins;
	std::map<int, ObjectPin*>	m_pins;
	std::map<int, IObjectGLTF*>	m_colorPins;

	ObjectTable* m_table;
};

