/*===================================================================================
*    Date : 2023/01/11(水)
*        Author		: Gakuto.S
*        File		: Object_Table.cpp
*        Detail		:
===================================================================================*/
#include "Object_Table.h"

#include "Summary_Objects.h"
#include "Summary_Scenes.h"

using namespace DirectX;
using namespace std;

void ObjectTable::Init()
{
	float objectSize = 2.0f;

	// ピンのホールを作成
	for (int i = 0; i < k_COL + 1; ++i)
	{
		for (int j = 0; j < k_ROW; ++j)
		{
			int index = i * k_ROW + j;
			m_pinWholes[index] = new ObjectPinWhole();
			m_pinWholes[index]->Init();

			// 中心は5列目とする
			XMFLOAT3 pos{};
			pos.x = objectSize * float(i) - objectSize * float((k_COL + 1) / 2);
			pos.y = 0.0f;
			pos.z = objectSize * float(k_ROW - j) - objectSize * float(k_ROW / 2) + (objectSize * 0.5f);

			m_pinWholes[index]->SetPosition(pos);
		}
	}

	// カラー選択用ピンホールを作成
	for (int i = 0; i < k_COLOR_NUM; ++i)
	{
		m_colorPinWholes[i] = new ObjectPinWhole();
		m_colorPinWholes[i]->Init();

		XMFLOAT3 pos{};
		pos.x = objectSize * float(i) - objectSize * float((k_COL + 1) / 2) + objectSize;
		pos.y = 0.0f;
		pos.z = objectSize * float(-1) - objectSize * float(k_ROW / 2) + (objectSize * 0.5f) + objectSize;

		m_colorPinWholes[i]->SetPosition(pos);
	}

	// Hit or Browピンのホールを作成
	objectSize *= 0.5f;
	for (int i = 0; i < k_COL; ++i)
	{
		for (int j = 0; j < k_ROW / 2; ++j)
		{
			for (int k = 0; k < k_ROW / 2; ++k)
			{
				int index = i * k_ROW + j * (k_ROW / 2) + k;
				m_ansPinWholes[index] = new ObjectPinWhole();
				m_ansPinWholes[index]->Init();
				m_ansPinWholes[index]->SetScale({ 0.5f, 0.5f, 0.5f });

				XMFLOAT3 pos{};
				pos.x = (-objectSize * 2 * k_ROW) + (objectSize * 2) * float(i) + objectSize * float(k) - (objectSize * 0.5f);
				pos.y = 0.1f;
				pos.z = (objectSize * k_ROW) + objectSize * float(k_ROW - j) - (objectSize * 0.5f);

				m_ansPinWholes[index]->SetPosition(pos);
			}
		}
	}
}

void ObjectTable::Uninit()
{
	// Hit or Brow用ピンホールの解放
	for (auto& ansPinWhole : m_ansPinWholes)
	{
		ansPinWhole.second->Uninit();
		delete ansPinWhole.second;
	}

	// カラー選択用ピンホールの解放
	for (auto& colorPinWhole : m_colorPinWholes)
	{
		colorPinWhole.second->Uninit();
		delete colorPinWhole.second;
	}

	// 回答ピンホールの解放
	for (auto& pinWhole : m_pinWholes)
	{
		pinWhole.second->Uninit();
		delete pinWhole.second;
	}
}

void ObjectTable::Draw()
{
	IGraphics* graphics = Application::Get()->Graphics();
	XMMATRIX world = graphics->CulculateWorldMatrix(
		m_position,
		m_rotate,
		m_scale
	);

	// ピンホールを描画
	for (auto& pinWhole : m_pinWholes)
		pinWhole.second->Draw(world);

	for (auto& ansPinWhole : m_ansPinWholes)
		ansPinWhole.second->Draw(world);

	// カラー選択用ピンホールの解放
	for (auto& colorPinWhole : m_colorPinWholes)
		colorPinWhole.second->Draw(world);
}

DirectX::XMFLOAT3 ObjectTable::GetPinWholePinSetPosition(int index)
{
	XMFLOAT3 retPosition{};

	// 添え字が範囲外であれば
	if (index < 0 || index >= k_ROW * k_COL + k_ROW)
		return retPosition;

	// y座標を補正する
	retPosition = m_pinWholes[index]->GetPosition();
	retPosition.x += m_position.x;
	retPosition.y += 0.8f + m_position.y;
	retPosition.z += m_position.z;

	return retPosition;
}

DirectX::XMFLOAT3 ObjectTable::GetHitBrowPinWholePinSetPosition(int index)
{
	XMFLOAT3 retPosition{};

	// 添え字が範囲外であれば
	if (index < 0 || index >= k_ROW * k_COL)
		return retPosition;

	// y座標を補正する
	retPosition = m_ansPinWholes[index]->GetPosition();
	retPosition.x += m_position.x;
	retPosition.y += 0.8f + m_position.y;
	retPosition.z += m_position.z;

	return retPosition;
}

DirectX::XMFLOAT3 ObjectTable::GetColorPinWholePinSetPosition(int index)
{
	XMFLOAT3 retPosition{};

	// 添え字が範囲外であれば
	if (index < 0 || index >= k_ROW * k_COL + k_ROW)
		return retPosition;

	// y座標を補正する
	retPosition = m_colorPinWholes[index]->GetPosition();
	retPosition.x += m_position.x;
	retPosition.y += 0.8f + m_position.y;
	retPosition.z += m_position.z;

	return retPosition;
}

DirectX::XMFLOAT3 ObjectTable::GetComitButtonPosition(int currentlyColumnIndex)
{
	int index = currentlyColumnIndex * k_ROW;	// 左上のHIT or Browピンホールのインデックス

	// 座標を取得して位置を調節
	XMFLOAT3 pos = GetHitBrowPinWholePinSetPosition(index);
	pos.x += 0.5f;
	pos.z -= 0.5f;
	return pos;
}
