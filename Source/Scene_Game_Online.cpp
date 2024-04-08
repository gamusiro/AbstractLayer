/*===================================================================================
*    Date : 2023/01/26(木)
*        Author		: Gakuto.S
*        File		: Scene_Game_Online.h
*        Detail		:
===================================================================================*/
#include "Scene_Game_Online.h"
#include "Photon/Photon_Base.h"

#include "Application.h"
#include "Summary_Objects.h"
#include "Summary_Scenes.h"
using namespace std;

SceneGameOnline::SceneGameOnline(std::array<int, 4> ans, int turn)
	:k_MY_TURN(turn),
	m_isTurn(0 == turn),	// 0とturnの値が等しい場合先行となる
	m_ans(ans),
	m_drawTurn(new ObjectSprite("TurnReference.png")),
	SceneGame()
{
}

SceneGameOnline::~SceneGameOnline()
{
	m_drawTurn->Uninit();
	delete m_drawTurn;
}

void SceneGameOnline::Init()
{
	this->gameObjectInit();
	m_pins->SetCommonAnswer({ m_ans[0], m_ans[1], m_ans[2], m_ans[3] });

	m_drawTurn->Init();
	m_drawTurn->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 8.0f * 7.0f, IGraphics::k_SCREEN_UI_HEIGHT / 8.0f * 7.0f, 0.0f });
	m_drawTurn->SetSize({ 300.0f, 100.0f });
}

void SceneGameOnline::Update()
{
	// カーソルの更新
	int column = m_cursors->GetCurrentlyColumn();
	bool check = m_pins->GetPinWholeStateOfAColumn(column);

	m_cursors->SetCurrentlyColumnFull(check);
	m_cursors->Update(m_isTurn);
	
	if (m_isTurn)
	{
		// ピンの設定
		updatePinControl(column);
		sendAction();
	}

	// 次の列へ移動可能な場合はボタンを描画する
	m_drawButton = false;
	if (check)
	{
		m_drawButton = true;
		DirectX::XMFLOAT3 pos = m_table->GetComitButtonPosition(column);
		pos.y -= 0.01f;
		m_comitButton->SetPosition(pos);
	}

	// シーンを切り替える
	if (m_finish && m_frameCounter > WindowWin::s_FRAME_COUNT)
		Application::Get()->SetScene(new SceneResult(m_table, m_pins, m_plane, m_cursors->GetCurrentlyColumn(), k_MY_TURN, true));
}

void SceneGameOnline::Receive(int* data)
{
	// データ種別をチェック
	int buttonType						= data[photon::SEND_INDEX::BUTTON_TYPE];
	ObjectCursors::COMIT_TYPE comitType = ObjectCursors::COMIT_TYPE(data[photon::SEND_INDEX::COMIT_TYPE]);
	int currentlyColumn					= data[photon::SEND_INDEX::COLUMN_INDEX];
	int currentlyRow					= data[photon::SEND_INDEX::ROW_CURSOR_INDEX];
	int colorIndex						= data[photon::SEND_INDEX::COLOR_CURSOR_INDEX];

	switch (comitType)
	{
	case ObjectCursors::COMIT_TYPE::ERASE:
		m_pins->UnRegisterPins(m_cursors->GetPinWholeIndex());
		break;
	case ObjectCursors::COMIT_TYPE::PUTON:
		m_pins->RegisterPins(m_cursors->GetPinWholeIndex(), m_cursors->GetColorIndex());
		break;
	case ObjectCursors::COMIT_TYPE::CHECK:
		// ヒットかブローかチェックする
		m_hitCount	= 0;
		m_browCount = 0;
		m_finish = m_pins->CheckHitOrBrow(m_cursors->GetCurrentlyColumn(), m_hitCount, m_browCount);

		// UV位置と、文字の表示位置を調節(UI表示文字がUV値をずらすと上下に移動してしまうので補正する処理)
		const float hitSpriteStride = 2.5f;
		m_hitSprite->SetUVStart({ 0.0f, float(m_hitCount) });
		m_hitSprite->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f - 100.0f, IGraphics::k_SCREEN_UI_HEIGHT / 3.0f + (hitSpriteStride * float(m_hitCount)), 0.0f});

		const float offset = 3.0f;				// ヒットスプライトの文字の高さに合わせるための数値
		const float browSpriteStride = 1.5f;
		m_browSprite->SetUVStart({ 0.0f, float(m_browCount) });
		m_browSprite->SetPosition({ IGraphics::k_SCREEN_UI_WIDTH / 2.0f + 100.0f, IGraphics::k_SCREEN_UI_HEIGHT / 3.0f + offset - (browSpriteStride * float(m_browCount)), 0.0f });

		m_frameCounter = 0;
		break;
	}

	// 現在の列と場所が代わったら
	if (currentlyColumn != SEND_DATA_NULL)
	{
		m_cursors->SetCurrentlyColumn(currentlyColumn);
		m_isTurn = (currentlyColumn % 2 == k_MY_TURN);
	}
		
	// 現在の行と場所が代わったら
	if (currentlyRow != SEND_DATA_NULL)
		m_cursors->SetCurrentlyRow(currentlyRow, true);

	// 現在のカラーインデックスと場所が代わったら
	if (colorIndex != SEND_DATA_NULL)
		m_cursors->SetColorIndex(colorIndex, true);
}

void SceneGameOnline::sendAction()
{
	int key = SEND_DATA_NULL;
	if(m_cursors)
		key = m_cursors->GetKeyDir();

	if (key != -1)
	{// カーソルが移動した場合
		PhotonBase::getInstance().sendInput(
			key,							// キー
			SEND_DATA_NULL,					// コミットタイプ
			SEND_DATA_NULL,					// 現在の列
			m_cursors->GetCurrentlyRow(),	// 現在の行(更新後の値を送信) 
			m_cursors->GetColorIndex()		// カラー番号
		);
	}

	ObjectCursors::COMIT_TYPE type = m_cursors->GetComitButton(false);
	switch (type)
	{
	case ObjectCursors::COMIT_TYPE::ERASE:
		// ピンを削除する
		PhotonBase::getInstance().sendInput(
			VK_BACK,								// キー
			int(ObjectCursors::COMIT_TYPE::ERASE),	// コミットタイプ
			SEND_DATA_NULL,							// 現在の列
			m_cursors->GetCurrentlyRow(),			// 現在の行 
			SEND_DATA_NULL							// カラー番号
		);
		break;
	case ObjectCursors::COMIT_TYPE::PUTON:
		// ピンを登録する
		PhotonBase::getInstance().sendInput(
			VK_RETURN,								// キー
			int(ObjectCursors::COMIT_TYPE::PUTON),	// コミットタイプ
			SEND_DATA_NULL,							// 現在の列
			m_cursors->GetCurrentlyRow(),			// 現在の行 
			m_cursors->GetColorIndex()				// カラー番号
		);
		break;
	case ObjectCursors::COMIT_TYPE::CHECK:
		// ターンを変更する
		m_isTurn = false;
		m_cursors->SetCurrentlyRow(0, false);
		PhotonBase::getInstance().sendInput(
			VK_RETURN,								// キー
			int(ObjectCursors::COMIT_TYPE::CHECK),	// コミットタイプ
			m_cursors->GetCurrentlyColumn(),		// 現在の列
			m_cursors->GetCurrentlyRow(),			// 現在の行 
			SEND_DATA_NULL							// カラー番号
		);
		break;
	}
}

void SceneGameOnline::drawUI()
{
	if (m_isTurn)
		m_drawTurn->Draw();
}
