/*===================================================================================
*    Date : 2023/01/20(金)
*        Author		: Gakuto.S
*        File		: Object_Result.cpp
*        Detail		:
===================================================================================*/
#include "Scene_Result.h"

#include "Application.h"
#include "Summary_Objects.h"
#include "Summary_Scenes.h"

SceneResult::SceneResult(
	ObjectTable* table,
	ObjectPins* pins,
	IObject3D* plane,
	int finishColumn,
	int myTurn,
	bool lobby
)
	:m_table(table),
	m_pins(pins),
	m_plane(plane),
	m_winSprite(new ObjectSprite("Result_Win.png")),
	m_loseSprite(new ObjectSprite("Result_Lose.png")),
	m_finishColumn(finishColumn - 1),	// 列が先に行っているので1列戻す
	m_rowIndex(0),
	m_correctSE(),
	m_failedSE(),
	m_successSE(),
	m_frameCount(0),
	m_correctCount(0),
	m_myTurn(myTurn),
	m_result(RESULT_STATE::NONE),
	k_Lobby(lobby),
	k_UpdateFrame(WindowWin::s_FRAME_COUNT)
{
	// オーディオの読み込み
	ISound* sound = Application::Get()->Sound();
	m_correctSE = sound->CreateSoundController("Correct.mp3");
	m_correctSE->SetVolume(10);

	m_successSE =  sound->CreateSoundController("Success.mp3");
	m_successSE->SetVolume(10);

	m_failedSE = sound->CreateSoundController("Failed.mp3");
	m_failedSE->SetVolume(10);
}

SceneResult::~SceneResult()
{
	// サウンドの解放処理
	ISound* sound = Application::Get()->Sound();
	sound->DestroySoundController(m_failedSE);
	sound->DestroySoundController(m_successSE);
	sound->DestroySoundController(m_correctSE);

	m_pins->Uninit();
	delete m_pins;

	m_table->Uninit();
	delete m_table;

	m_plane->Uninit();
	delete m_plane;
}

void SceneResult::Init()
{
	const DirectX::XMFLOAT3 pos = { IGraphics::k_SCREEN_UI_WIDTH / 2.0f, IGraphics::k_SCREEN_UI_HEIGHT / 10.0f, 0.0f };
	const DirectX::XMFLOAT2 size = { 600.0f, 120.0f };

	m_winSprite->Init();
	m_winSprite->SetPosition(pos);
	m_winSprite->SetSize(size);

	m_loseSprite->Init();
	m_loseSprite->SetPosition(pos);
	m_loseSprite->SetSize(size);
}

void SceneResult::Uninit()
{
	m_loseSprite->Uninit();
	delete m_loseSprite;

	m_winSprite->Uninit();
	delete m_winSprite;
}

void SceneResult::Update()
{
	// 行のインデックスが範囲内であれば
	if (m_rowIndex <= ObjectTable::k_ROW)
	{
		if (m_frameCount >= k_UpdateFrame)
		{
			m_frameCount = 0;

			// 最後の行までチェックが終わっている
			if (m_rowIndex >= ObjectTable::k_ROW)
			{
				this->resultSound(m_myTurn);
			}
			else
			{
				// カラーが一致していれば
				if (m_pins->SetAnswerColor(m_rowIndex, m_finishColumn))
				{
					m_correctCount++;
					m_correctSE->Stop();
					m_correctSE->Play();
				}
			}

			// 次の行へ
			m_rowIndex++;
		}
	}
	else
	{
		// 次のシーンへ移動する
		if (m_frameCount >= k_UpdateFrame * 2)
		{
			if(k_Lobby)
				Application::Get()->SetTransion(new SceneLobby(true));
			else	
				Application::Get()->SetTransion(new SceneTitle());
		}
	}

	// フレームの更新
	m_frameCount++;
}

void SceneResult::Draw()
{
	m_plane->Draw();
	m_table->Draw();
	m_pins->Draw();

	switch (m_result)
	{
	case SceneResult::RESULT_STATE::WIN:
		m_winSprite->Draw();
		break;
	case SceneResult::RESULT_STATE::LOSE:
		m_loseSprite->Draw();
		break;
	}
}

void SceneResult::resultSound(int myTurn)
{
	// 解答とすべてのカラーが一致している
	if (m_correctCount >= ObjectTable::k_ROW)
	{
		// オンライン対戦であれば
		if (m_myTurn >= 0)
		{
			// 最後に置いた列がプレイヤーのターンであれば
			if (m_myTurn == m_finishColumn % 2)
				m_result = RESULT_STATE::WIN;
			else
				m_result = RESULT_STATE::LOSE;
		}
		else
		{// ソロモードであれば
			m_result = RESULT_STATE::WIN;
		}
	}
	else
	{
		m_result = RESULT_STATE::LOSE;
	}
		

	// サウンドエフェクトを再生
	if (m_result == RESULT_STATE::WIN)
		m_successSE->Play();
	else
		m_failedSE->Play();
}
