/*===================================================================================
*    Date : 2023/01/26(��)
*        Author		: Gakuto.S
*        File		: Photon_Base.cpp
*        Detail		:
===================================================================================*/
#include "Photon_Base.h"

#include "../Application.h"
#include "../Summary_Scenes.h"

#include <iostream>

#define EVENT_CODE_LOBBY	1
#define EVENT_CODE_ANSWER	2
#define EVENT_CODE_GAME		3

static const ExitGames::Common::JString appId		= L"XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"; // ����ID
static const ExitGames::Common::JString appVersion	= L"1.0";
static const ExitGames::Common::JString PLAYER_NAME = L"Windows";


void PhotonBase::connect(void)
{
#ifdef _DEBUG
	std::wcout << "�ڑ���..." << std::endl;
#endif // _DEBUG
	
	m_loadBalancingClient.connect(ExitGames::LoadBalancing::AuthenticationValues());
}

void PhotonBase::sendAnswer(std::array<int, 5> ansAndTurn)
{
	m_loadBalancingClient.opRaiseEvent(true, &ansAndTurn[0], int(ansAndTurn.size()), EVENT_CODE_ANSWER);
}

void PhotonBase::sendInput(int buttonNum, int comitType, int columnIndex, int rowCursorIndex, int colorCursorIndex)
{
	int data[photon::SEND_INDEX::SEND_DATA_MAX]{};
	data[photon::SEND_INDEX::BUTTON_TYPE]			= buttonNum;
	data[photon::SEND_INDEX::COMIT_TYPE]			= comitType;
	data[photon::SEND_INDEX::COLUMN_INDEX]			= columnIndex;
	data[photon::SEND_INDEX::ROW_CURSOR_INDEX]		= rowCursorIndex;
	data[photon::SEND_INDEX::COLOR_CURSOR_INDEX]	= colorCursorIndex;

	m_loadBalancingClient.opRaiseEvent(true, data, photon::SEND_INDEX::SEND_DATA_MAX, EVENT_CODE_GAME);
}

void PhotonBase::sendComingClient()
{
	bool comingClient = true;
	m_loadBalancingClient.opRaiseEvent(true, &comingClient, 1, EVENT_CODE_LOBBY);
}

void PhotonBase::disconnect(void)
{
	//Application::Get()->SetIsHost(false);
#ifdef _DEBUG
	std::wcout << "�ؒf���܂���" << std::endl;
#endif // _DEBUG

	m_loadBalancingClient.disconnect();
}

PhotonBase& PhotonBase::getInstance()
{
	static PhotonBase p;
	return p;
}

PhotonBase::PhotonBase()
	:m_loadBalancingClient(*this, appId, appVersion, ExitGames::Photon::ConnectionProtocol::TCP)
{
}

void PhotonBase::run(void)
{
	m_loadBalancingClient.service();
}

// protocol implementations

void PhotonBase::debugReturn(int debugLevel, const ExitGames::Common::JString& string)
{
}

void PhotonBase::connectionErrorReturn(int errorCode)
{
#ifdef _DEBUG
	std::wcout << "�ڑ��Ɏ��s : " << errorCode << std::endl;
#endif // _DEBUG
}

void PhotonBase::clientErrorReturn(int errorCode)
{
#ifdef _DEBUG
	std::wcout << "clientErrorReturn : " << errorCode << std::endl;
#endif // _DEBUG
}

void PhotonBase::warningReturn(int warningCode)
{
#ifdef _DEBUG
	std::wcout << "warningReturn : " << warningCode << std::endl;
#endif // _DEBUG
}

void PhotonBase::serverErrorReturn(int errorCode)
{
#ifdef _DEBUG
	std::wcout << "serverErrorReturn : " << errorCode << std::endl;
#endif // _DEBUG
}

void PhotonBase::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player)
{
#ifdef _DEBUG
	std::wcout << playerNr << "�������܂���" << std::endl;
#endif // _DEBUG
	
	SceneLobby* sceneLobby = (SceneLobby*)Application::Get()->GetCurrentlyScene();

	// �z�X�g�łȂ��ꍇ��
	if (!Application::Get()->IsHost())
		sceneLobby->Join(false);
}

void PhotonBase::leaveRoomEventAction(int playerNr, bool isInactive)
{
	Application::Get()->SetIsHost(false);
	Application::Get()->SetTransion(new SceneTitle());
	
#ifdef _DEBUG
	std::wcout << "�ގ����܂���" << std::endl;
#endif // _DEBUG
}

void PhotonBase::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
	int* data = ExitGames::Common::ValueObject<int*>(eventContent).getDataCopy();
	switch (eventCode)
	{
	case EVENT_CODE_LOBBY:
	{
		SceneLobby* sceneLobby = (SceneLobby*)Application::Get()->GetCurrentlyScene();

		// �z�X�g�ł���ꍇ��
		if (Application::Get()->IsHost())
			sceneLobby->Join(false);
	}
		break;
	case EVENT_CODE_ANSWER:	// �𓚂̐ݒ�
	{
		Application::Get()->SetTransion(new SceneGameOnline(
			{ data[0], data[1], data[2], data[3] },	// �𓚃J���[�̐ݒ�
			data[4])								// �^�[���̐ݒ�
		);
	}
		break;

	case EVENT_CODE_GAME:	// �Q�[�����̃C�x���g���擾
	{
		SceneGameOnline* game = (SceneGameOnline*)Application::Get()->GetCurrentlyScene();
		game->Receive(data);
	}
		break;
	default:
		break;
	}
}

void PhotonBase::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster)
{
	if (errorCode)
	{
#ifdef _DEBUG
		std::wcout << "�t�H�g���T�[�o�[�ւ̐ڑ��Ɏ��s���܂����B : " << errorString.cstr() << std::endl;
#endif // _DEBUG
	}
	else
	{
#ifdef _DEBUG
		std::wcout << "�t�H�g���T�[�o�[�ւ̐ڑ��ɐ������܂����B" << std::endl;
		std::wcout << "���[�����������܂�" << std::endl;
#endif // _DEBUG

		m_loadBalancingClient.opJoinRandomRoom();
	}
}

void PhotonBase::disconnectReturn(void)
{
#ifdef _DEBUG
	std::wcout << "disconnected" << std::endl;
#endif // _DEBUG
	
}

void PhotonBase::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	Application::Get()->SetIsHost(true);
	SceneLobby* lobby = (SceneLobby*)Application::Get()->GetCurrentlyScene();
	lobby->Join(true);

#ifdef _DEBUG
	std::wcout << "���[�����쐬����܂���" << std::endl;
#endif // _DEBUG
}

void PhotonBase::joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
}

void PhotonBase::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
}

void PhotonBase::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	if (errorCode)
	{
#ifdef _DEBUG
		std::wcout << "�ڑ��\�ȃ��[��������܂���ł��� : " << errorCode << ", " << errorString.cstr() << std::endl;
#endif // _DEBUG

		if (errorCode == 32760) //no match found error code
		{
#ifdef _DEBUG
			std::wcout << "���[�����쐬���܂�..." << std::endl;
#endif // _DEBUG
			
			//try to create a room and wait;
			ExitGames::Common::JString name;
			name += GETTIMEMS();
			m_loadBalancingClient.opCreateRoom(name, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(2));
		}

		return;
	}
}

void PhotonBase::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
}

void PhotonBase::joinLobbyReturn(void)
{
}

void PhotonBase::leaveLobbyReturn(void)
{
}

void PhotonBase::onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
}

void PhotonBase::onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
}

void PhotonBase::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers)
{
	m_loadBalancingClient.selectRegion("asia");
}

