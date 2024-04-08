/*===================================================================================
*    Date : 2023/01/26(木)
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
	/// \brief コンストラクタ
	/// 
	/// \return なし
	//**************************************************
	PhotonBase();

	//**************************************************
	/// \brief 更新処理
	/// 
	/// \return なし
	//**************************************************
	void run(void);

	//**************************************************
	/// \brief 接続処理
	/// 
	/// \return なし
	//**************************************************
	void connect(void);
	//void opCreateRoom(void);
	//void opJoinRandomRoom(void);
	//void opJoinOrCreateRoom(void);

	//**************************************************
	/// \brief 解答の送信
	/// 
	/// \return なし
	//**************************************************
	void sendAnswer(std::array<int, 5> ansAndTurn);

	//**************************************************
	/// \brief メッセージ転送処理
	/// 
	/// \return なし
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
	/// \brief 切断処理
	/// 
	/// \return なし
	//**************************************************
	void disconnect(void);

	//**************************************************
	/// \brief インスタンス取得
	/// 
	/// \return なし
	//**************************************************
	static PhotonBase& getInstance();

private:
	//**************************************************
	/// \brief デバック
	/// 
	/// \return なし
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
	/// \brief ルーム接続イベント
	/// 
	/// \return なし
	//**************************************************
	virtual void joinRoomEventAction(
		int playerNr,
		const ExitGames::Common::JVector<int>& playernrs,
		const ExitGames::LoadBalancing::Player& player
	) override;

	//**************************************************
	/// \brief ルーム退出時イベント
	/// 
	/// \return なし
	//**************************************************
	virtual void leaveRoomEventAction(
		int playerNr,
		bool isInactive
	) override;
	
	//**************************************************
	/// \brief カスタムイベントアクション
	/// 
	/// \return なし
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
	/// \brief 接続後のイベント
	/// 
	/// \return なし
	//**************************************************
	virtual void connectReturn(
		int errorCode,
		const ExitGames::Common::JString& errorString,
		const ExitGames::Common::JString& region,
		const ExitGames::Common::JString& cluster
	) override;

	//**************************************************
	/// \brief 切断後のイベント
	/// 
	/// \return なし
	//**************************************************
	virtual void disconnectReturn(void);

	//**************************************************
	/// \brief ルーム作成後のイベント
	/// 
	/// \return なし
	//**************************************************
	virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	
	//**************************************************
	/// \brief ルーム参加・作成後のイベント
	/// 
	/// \return なし
	//**************************************************
	virtual void joinOrCreateRoomReturn(
		int localPlayerNr,
		const ExitGames::Common::Hashtable& gameProperties,
		const ExitGames::Common::Hashtable& playerProperties,
		int errorCode,
		const ExitGames::Common::JString& errorString
	) override;

	//**************************************************
	/// \brief ルーム参加後のイベント
	/// 
	/// \return なし
	//**************************************************
	virtual void joinRoomReturn(
		int localPlayerNr,
		const ExitGames::Common::Hashtable& gameProperties,
		const ExitGames::Common::Hashtable& playerProperties,
		int errorCode,
		const ExitGames::Common::JString& errorString
	) override;

	//**************************************************
	/// \brief ランダムルーム参加後のイベント
	/// 
	/// \return なし
	//**************************************************
	virtual void joinRandomRoomReturn(
		int localPlayerNr,
		const ExitGames::Common::Hashtable& gameProperties,
		const ExitGames::Common::Hashtable& playerProperties,
		int errorCode,
		const ExitGames::Common::JString& errorString
	) override;

	//**************************************************
	/// \brief ルーム退室後のイベント
	/// 
	/// \return なし
	//**************************************************
	virtual void leaveRoomReturn(
		int errorCode,
		const ExitGames::Common::JString& errorString
	) override;

	//**************************************************
	/// \brief ロビー参加後のイベント
	/// 
	/// \return なし
	//**************************************************
	virtual void joinLobbyReturn(void);
	
	//**************************************************
	/// \brief ロビー退室後のイベント
	/// 
	/// \return なし
	//**************************************************
	virtual void leaveLobbyReturn(void);

private:
	ExitGames::LoadBalancing::Client m_loadBalancingClient;
};

