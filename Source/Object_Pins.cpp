/*===================================================================================
*    Date : 2023/01/14(土)
*        Author		: Gakuto.S
*        File		: Object_Pins.cpp
*        Detail		:
===================================================================================*/
#include "Object_Pins.h"

#include "Application.h"
#include "Summary_Objects.h"
using namespace std;
using namespace DirectX;

ObjectPins::ObjectPins(ObjectTable* table)
	:m_table(table)
{
}

void ObjectPins::Init()
{
	// 解答を作成
	random_device seed_gen;
	mt19937 engine(seed_gen());
	
	int index = ObjectTable::k_COL * ObjectTable::k_ROW;

	for (int i = 0; i < ObjectTable::k_ROW; ++i)
	{
		// ピンオブジェクトの生成
		int random = engine() % k_COLORLIST.size();
		m_answerPins[i] = new ObjectPin(k_COLORLIST[random], false);
		m_answerPins[i]->Init();

		// ピンのポジション設定
		m_answerPins[i]->SetPosition(m_table->GetPinWholePinSetPosition(index + i));
		m_answerPins[i]->SetScale(k_PIN_SCALE);
	}

	// カラーピンの作成
	for (int i = 0; i < ObjectTable::k_COLOR_NUM; ++i)
	{
		// ピンオブジェクトの生成
		m_colorPins[i] = new ObjectPin(k_COLORLIST[i]);
		m_colorPins[i]->Init();

		// ピンのポジション設定
		m_colorPins[i]->SetPosition(m_table->GetColorPinWholePinSetPosition(i));
		m_colorPins[i]->SetScale(k_PIN_SCALE);
	}
}

void ObjectPins::Uninit()
{
	for (auto& ansPin : m_answerPins)
	{
		if (ansPin.second)
		{
			ansPin.second->Uninit();
			delete ansPin.second;
		}
	}

	for (auto& HBPin : m_hitBrowPins)
	{
		if (HBPin.second)
		{
			HBPin.second->Uninit();
			delete HBPin.second;
		}
	}

	for (auto& colorPin : m_colorPins)
	{
		if (colorPin.second)
		{
			colorPin.second->Uninit();
			delete colorPin.second;
		}
	}

	for (auto& pin : m_pins)
	{
		if (pin.second)
		{
			pin.second->Uninit();
			delete pin.second;
		}
	}
}

void ObjectPins::Draw()
{
	// 解答ピンの描画
	for (auto& ansPin : m_answerPins)
	{
		if (ansPin.second)
			ansPin.second->Draw();
	}

	// ヒット、ブローの表示処理
	for (auto& HBPin : m_hitBrowPins)
	{
		if (HBPin.second)
			HBPin.second->Draw();
	}

	// カラーピンの表示処理
	for (auto& colorPin : m_colorPins)
	{
		if (colorPin.second)
			colorPin.second->Draw();
	}

	// 回答ピンの表示処理
	for (auto& pin : m_pins)
	{
		if (pin.second)
			pin.second->Draw();
	}
}

void ObjectPins::RegisterPins(int pinWholeIndex, int colorIndex)
{
	// データの解放
	this->UnRegisterPins(pinWholeIndex);

	// ピンオブジェクトの生成
	m_pins[pinWholeIndex] = new ObjectPin(k_COLORLIST[colorIndex]);
	m_pins[pinWholeIndex]->Init();

	// ピンのポジション設定
	m_pins[pinWholeIndex]->SetPosition(m_table->GetPinWholePinSetPosition(pinWholeIndex));
	m_pins[pinWholeIndex]->SetScale(k_PIN_SCALE);
}

void ObjectPins::UnRegisterPins(int pinWholeIndex)
{
	// データがある場合はデータの削除
	if (m_pins[pinWholeIndex])
	{
		delete m_pins[pinWholeIndex];
		m_pins[pinWholeIndex] = nullptr;
	}
}

bool ObjectPins::GetPinWholeStateOfAColumn(int currentlyColumnIndex)
{
	for (int i = 0; i < ObjectTable::k_ROW; ++i)
	{
		if (!m_pins[ObjectTable::k_ROW * currentlyColumnIndex + i])
			return false;
	}

	// 列に4つのピンが刺さっているかどうか
	return true;
}

bool ObjectPins::CheckHitOrBrow(int currentlyColumnIndex, int& hit, int& brow)
{
	vector<ObjectPinAns::ANSWER> needPinList;
	map<int, ObjectPinAns::ANSWER> answerState;
	vector<int> noHitPinIndexList;

	// ヒットのチェック
	for (int i = 0; i < ObjectTable::k_ROW; ++i)
	{
		int pinIndex = ObjectTable::k_ROW * currentlyColumnIndex + i;
		int answerIndex = i;
		
		// ヒットであれば
		ObjectPinAns::ANSWER ans = {};
		if (m_pins[pinIndex]->GetColor() == m_answerPins[answerIndex]->GetColor())
		{
			needPinList.push_back(ObjectPinAns::ANSWER::HIT);		// 必要なピンとして追加
			answerState[answerIndex] = ObjectPinAns::ANSWER::HIT;	// 回答の状態をヒットとして設定
			hit++;
		}
		else
			noHitPinIndexList.push_back(pinIndex);					// ヒットしていないデータをリストアップ
	}

	// ブローのチェック
	for (int i = 0; i < noHitPinIndexList.size(); ++i)
	{
		int pinIndex = noHitPinIndexList[i];	// ヒットしていないピンインデックスを取得

		for (int j = 0; j < ObjectTable::k_ROW; ++j)
		{			
			int answerIndex = j;
			if (answerState[answerIndex] != ObjectPinAns::ANSWER::NONE)
				continue;	// 回答状態がヒットやブローであれば次を調べる

			if (m_pins[pinIndex]->GetColor() == m_answerPins[answerIndex]->GetColor())
			{
				needPinList.push_back(ObjectPinAns::ANSWER::BROW);		// 必要なピンとして追加
				answerState[answerIndex] = ObjectPinAns::ANSWER::BROW;	// 回答の状態をブローとして設定
				brow++;
				break;
			}
		}
	}

	for (int i = 0; i < int(needPinList.size()); ++i)
	{
		// ヒット又はブローピンのオブジェクトを生成する
		int index = ObjectTable::k_ROW * currentlyColumnIndex + i;
		m_hitBrowPins[index] = new ObjectPinAns(needPinList[i]);
		m_hitBrowPins[index]->Init();

		// ポジションを設定する
		m_hitBrowPins[index]->SetPosition(m_table->GetHitBrowPinWholePinSetPosition(index));
	}

	// 最後の列で成功しなかった場合
	bool ret = true;
	if (needPinList.size() < ObjectTable::k_ROW)
		ret = false;

	// 全てヒットであるか調べる
	if (ret)
	{
		for (size_t i = 0; i < needPinList.size(); ++i)
		{
			if (needPinList[i] != ObjectPinAns::ANSWER::HIT)
			{
				ret = false;
				break;
			}
		}
	}

	// ゲームオーバー
	if (!ret && currentlyColumnIndex >= ObjectTable::k_COL -1)
		ret = true;

	return ret;
}

bool ObjectPins::SetAnswerColor(int answerRowIndex, int finishColumn)
{
	// 色の設定
	m_answerPins[answerRowIndex]->SetUseMaterial(true);

	// カラーが一致
	if (m_answerPins[answerRowIndex]->GetColor() == m_pins[finishColumn * ObjectTable::k_ROW + answerRowIndex]->GetColor())
		return true;
	else
		return false;
}

void ObjectPins::SetCommonAnswer(std::array<int, 4> ans)
{
	for (int i = 0; i < ObjectTable::k_ROW; ++i)
	{
		ObjectPin* work = new ObjectPin(k_COLORLIST[ans[i]], false);
		work->Init();
		work->SetPosition(m_answerPins[i]->GetPosition());
		work->SetScale(m_answerPins[i]->GetScale());
		m_answerPins[i]->Uninit();
		delete m_answerPins[i];

		m_answerPins[i] = work;
	}
}
