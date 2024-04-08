/*===================================================================================
*    Date : 2023/01/26(��)
*        Author		: Gakuto.S
*        File		: Photon_Base.h
*        Detail		:
===================================================================================*/
#pragma once
#include "LoadBalancing-cpp/inc/Client.h"

#define SEND_DATA_NULL -1

namespace photon
{
	typedef enum SEND_INDEX
	{
		BUTTON_TYPE = 0,
		COMIT_TYPE,
		COLUMN_INDEX,
		ROW_CURSOR_INDEX,
		COLOR_CURSOR_INDEX,
		SEND_DATA_MAX
	}SEND_INDEX;
}

class PhotonBase : private ExitGames::LoadBalancing::Listener
{
public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	PhotonBase();

	//**************************************************
	/// \brief �X�V����
	/// 
	/// \return �Ȃ�
	//**************************************************
	void run(void);

	//**************************************************
	/// \brief �ڑ�����
	/// 
	/// \return �Ȃ�
	//**************************************************
	void connect(void);
	//void opCreateRoom(void);
	//void opJoinRandomRoom(void);
	//void opJoinOrCreateRoom(void);

	//**************************************************
	/// \brief �𓚂̑��M
	/// 
	/// \return �Ȃ�
	//**************************************************
	void sendAnswer(std::array<int, 5> ansAndTurn);

	//**************************************************
	/// \brief ���b�Z�[�W�]������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void sendInput(
		int buttonNum,
		int comitType,
		int columnIndex,
		int rowCursorIndex,
		int colorCursorIndex
	);

	void sendComingClient();

	//**************************************************
	/// \brief �ؒf����
	/// 
	/// \return �Ȃ�
	//**************************************************
	void disconnect(void);

	//**************************************************
	/// \brief �C���X�^���X�擾
	/// 
	/// \return �Ȃ�
	//**************************************************
	static PhotonBase& getInstance();

private:
	//**************************************************
	/// \brief �f�o�b�N
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void debugReturn(
		int debugLevel,
		const ExitGames::Common::JString& string
	) override;

	// implement your error-handling here
	virtual void connectionErrorReturn(int errorCode);
	virtual void clientErrorReturn(int errorCode);
	virtual void warningReturn(int warningCode);
	virtual void serverErrorReturn(int errorCode);

	//**************************************************
	/// \brief ���[���ڑ��C�x���g
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void joinRoomEventAction(
		int playerNr,
		const ExitGames::Common::JVector<int>& playernrs,
		const ExitGames::LoadBalancing::Player& player
	) override;

	//**************************************************
	/// \brief ���[���ޏo���C�x���g
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void leaveRoomEventAction(
		int playerNr,
		bool isInactive
	) override;
	
	//**************************************************
	/// \brief �J�X�^���C�x���g�A�N�V����
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void customEventAction(
		int playerNr,
		nByte eventCode,
		const ExitGames::Common::Object& eventContent
	) override;

	virtual void onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats) override;
	virtual void onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats) override;
	
	virtual void onAvailableRegions(
		const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions,
		const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers
	) override;

	// callbacks for operations on PhotonLoadBalancing server
	//**************************************************
	/// \brief �ڑ���̃C�x���g
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void connectReturn(
		int errorCode,
		const ExitGames::Common::JString& errorString,
		const ExitGames::Common::JString& region,
		const ExitGames::Common::JString& cluster
	) override;

	//**************************************************
	/// \brief �ؒf��̃C�x���g
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void disconnectReturn(void);

	//**************************************************
	/// \brief ���[���쐬��̃C�x���g
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	
	//**************************************************
	/// \brief ���[���Q���E�쐬��̃C�x���g
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void joinOrCreateRoomReturn(
		int localPlayerNr,
		const ExitGames::Common::Hashtable& gameProperties,
		const ExitGames::Common::Hashtable& playerProperties,
		int errorCode,
		const ExitGames::Common::JString& errorString
	) override;

	//**************************************************
	/// \brief ���[���Q����̃C�x���g
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void joinRoomReturn(
		int localPlayerNr,
		const ExitGames::Common::Hashtable& gameProperties,
		const ExitGames::Common::Hashtable& playerProperties,
		int errorCode,
		const ExitGames::Common::JString& errorString
	) override;

	//**************************************************
	/// \brief �����_�����[���Q����̃C�x���g
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void joinRandomRoomReturn(
		int localPlayerNr,
		const ExitGames::Common::Hashtable& gameProperties,
		const ExitGames::Common::Hashtable& playerProperties,
		int errorCode,
		const ExitGames::Common::JString& errorString
	) override;

	//**************************************************
	/// \brief ���[���ގ���̃C�x���g
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void leaveRoomReturn(
		int errorCode,
		const ExitGames::Common::JString& errorString
	) override;

	//**************************************************
	/// \brief ���r�[�Q����̃C�x���g
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void joinLobbyReturn(void);
	
	//**************************************************
	/// \brief ���r�[�ގ���̃C�x���g
	/// 
	/// \return �Ȃ�
	//**************************************************
	virtual void leaveLobbyReturn(void);

private:
	ExitGames::LoadBalancing::Client m_loadBalancingClient;
};

