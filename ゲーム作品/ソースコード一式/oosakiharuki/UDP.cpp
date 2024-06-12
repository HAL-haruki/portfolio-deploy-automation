// 警告対応
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//------------------------------------
// インクルード部
//------------------------------------
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

#include <process.h>	// マルチスレッド化(ブロッキング関数対応)
#include <time.h>		// ログ機能用

#include "UDP.h"
#include "Massage.h"

#include "input.h"

// ライブラリ
#pragma comment(lib,"ws2_32.lib")
#pragma	comment(lib,"winmm.lib")

// マクロ定義
#define		PORT_NO  	(20250)

#define		INTERVAL	   (10)
#define		MAX_PLAYER	   (10)

// 構造体定義
// スレッドに渡す構造体
typedef struct
{
	SOCKET	sockNo;		// ソケット番号
	HANDLE	hEvent;		// イベントハンドル
}RecvInfo;

// クライアント情報
typedef struct
{
	sockaddr_in		fromAddr;			// アドレ
	Pos Position;
	Rate Rotation;
}ClientInfo;

// グローバル変数宣言
u_short		g_portNo = PORT_NO;		// ポート番号
SOCKET		g_sockNo;				// ソケット番号
sockaddr_in	g_myAddr;				// 自分のアドレス
sockaddr_in	g_enemyAddr;				// aiteのアドレス

ClientInfo	g_ClientList[MAX_PLAYER];
HANDLE		g_hEvent;				// イベントハンドル
MSGDATA		g_recvBuffer;			// 受信バッファ
char		g_szServerIP[128];		// サーバーIPアドレス

char		g_szMyName[MAX_NAME];	// 自分の名前
int			g_nListCnt;				// 列挙件数
int			g_nRequestNo;			// 列挙要求番号
int			g_nNextRequestNo;		// 次に使用される列挙要求番号
std::string g_IpAdoless;

// 現在の状態
bool g_SetMach = false;

// GUID

// {A7C22E16-9F27-409B-AAA7-19FE6970F6B3}
static const GUID g_MyGuid =
{ 0xa7c22e16, 0x9f27, 0x409b,{ 0xaa, 0xa7, 0x19, 0xfe, 0x69, 0x70, 0xf6, 0xb3 } };



// 自分のIPアドレスを取得
char* GetMyIP() {
	char	szHostName[128];
	hostent* lphe;

	gethostname(szHostName, sizeof(szHostName));
	lphe = gethostbyname(szHostName);
	if (lphe)
		return inet_ntoa(((in_addr)*((in_addr*)lphe->h_addr_list[0])));

	return NULL;
}

UDP* UDP::instance = nullptr;

UDP::UDP() {
}

UDP::~UDP() {
}

void UDP::SetPlayer(DirectX::XMFLOAT3 playerPos, DirectX::XMFLOAT3 playerRate, bool Attack, bool Hit, int HP) {
	m_PlayerPos = playerPos;
	m_PlayerRate = playerRate;
	m_Attack = Attack;
	m_Hit = Hit;
	m_HP = HP;
}

DirectX::XMFLOAT3 UDP::GetPlayerPos() {
	return m_PlayerPos;
}

DirectX::XMFLOAT3 UDP::GetPlayerRate() {
	return m_PlayerRate;
}

DirectX::XMFLOAT3 UDP::GetPlayerPos2() {
	 //plyaerPos2 = m_EnemyPos;
	 return m_EnemyPos;
}
DirectX::XMFLOAT3 UDP::GetPlayerRate2() {
	return m_EnemyRate;
}
bool UDP::GetAttack()
{
	return m_Attack;
}
bool UDP::GetHit()
{
	return m_Hit;
}
int UDP::GetHP()
{
	return m_HP;
}
bool UDP::GetAttackEnemy()
{
	return m_AttackEnemy;
}
bool UDP::GetHitEnemy()
{
	return m_HitEnemy;
}
int UDP::GetHPEnemy()
{
	return m_HPEnemy;
}
void UDP::SetEnemy(DirectX::XMFLOAT3 enemyPos, DirectX::XMFLOAT3 enemyRate, bool Attack, bool Hit, int HP) {
	m_EnemyPos = enemyPos;
	m_EnemyRate = enemyRate;
	m_AttackEnemy = Attack;
	m_HitEnemy = Hit;
	m_HPEnemy = HP;

}
void UDP::sendMatchRequest() {
	MSGDATA		SendMessage;		// ユーザ列挙要求送信用
	int			nStatus;			// socket関数戻値格納用
	sockaddr_in	toAddr;				// 送信先アドレス
	SendMessage.Msg.Header.nType = MSGTYPE_MACH_REQUEST;
	SendMessage.Msg.Header.nSeqNo = g_nNextRequestNo;
	memcpy(&SendMessage.Msg.Body0.guid, &g_MyGuid, sizeof(g_MyGuid));

	toAddr.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");

	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(PORT_NO);
	// 送信
	sendto(g_sockNo, SendMessage.szData, sizeof(SendMessage.szData), 0, (sockaddr *)&toAddr, sizeof(toAddr));

}
void UDP::sendPlayerState() {

	MSGDATA		SendMessage;		// ユーザ列挙要求送信用
	int			nStatus;			// socket関数戻値格納用
	sockaddr_in	toAddr;				// 送信先アドレス
	SendMessage.Msg.Header.nType = MSGTYPE_PLAYER_STATE;
	SendMessage.Msg.Header.nSeqNo = g_nNextRequestNo;
	//memcpy(&SendMessage.Msg.Body1, &g_MyGuid, sizeof(g_MyGuid));
	// ポジション
	auto pos = UDP::get_instance().GetPlayerPos();
	SendMessage.Msg.Body1.Position.x = pos.x;
	SendMessage.Msg.Body1.Position.y = pos.y;
	SendMessage.Msg.Body1.Position.z = pos.z;
	// 向き
	auto rote = UDP::get_instance().GetPlayerRate();
	SendMessage.Msg.Body1.Rotation.x = rote.x;
	SendMessage.Msg.Body1.Rotation.y = rote.y;
	SendMessage.Msg.Body1.Rotation.z = rote.z;
	// アタック
	auto Attack = UDP::get_instance().GetAttack();
	SendMessage.Msg.Body1.Attack = Attack;
	// 当たったかどうか
	auto Hit = UDP::get_instance().GetHit();
	SendMessage.Msg.Body1.Hit = Hit;
	// HP
	auto HP = UDP::get_instance().GetHP();
	SendMessage.Msg.Body1.HP = HP;

	// 送信先の設定
	toAddr.sin_addr.S_un.S_addr = g_enemyAddr.sin_addr.S_un.S_addr;
	//toAddr.sin_addr.S_un.S_addr = inet_addr("10.130.21.14");
	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(PORT_NO);


	// シーケンス番号の更新
	// 列挙数のクリア
	g_nListCnt = 0;
	// 現在の番号を更新
	g_nRequestNo = g_nNextRequestNo;
	// 次使う番号を更新
	g_nNextRequestNo++;

	// 送信処理
	sendto(g_sockNo, SendMessage.szData, sizeof(SendMessage.szData), 0, (sockaddr *)&toAddr, sizeof(toAddr));

	// プレイヤー情報の初期化

}

bool UDP::SetMach()
{
	return g_SetMach;
}

void UDP::InitUdp() {
	WSADATA		wsaData;					// WSADATA格納用
	int			nStatus;					// socket関数戻値格納用
	int			nErrCode;					// エラーコード格納用
	bool		flg = true;

	g_IpAdoless = "255.255.255.255";

	// WINSOCK初期化
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	strcpy(g_szServerIP, GetMyIP());

	// ソケット生成(IPv4,ダイアグラム通信,UDPプロトコル)
	g_sockNo = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// 自分のソケットにＩＰアドレス、ポート番号を割り当てる
	g_myAddr.sin_port = htons(g_portNo);
	g_myAddr.sin_family = AF_INET;
	g_myAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(g_sockNo, (sockaddr*)&g_myAddr, sizeof(sockaddr));

	// ブロードキャストを可能にする
	setsockopt(g_sockNo, SOL_SOCKET, SO_BROADCAST, (char*)&flg, sizeof(bool));

	g_hEvent = CreateEvent(NULL, true, false, NULL);		// イベントハンドル生成

	// データを受信するためのスレッドを開始させる
	_beginthread(RecvClientData, 2048, NULL);
}

 void UDP::UninitUdp() {
	 // ソケットをクローズ
	 closesocket(g_sockNo);

	 // サブスレッドの終了を待つ
	 WaitForMultipleObjects(1, &g_hEvent, true, INFINITE);

	 // イベントハンドルのクローズ
	 CloseHandle(g_hEvent);

	 // WINSOCKの後処理
	 WSACleanup();
}

 // --列挙要求の受信と列挙応答の送信(別スレッド)
void RecvClientData(void *p1) {
	sockaddr_in		fromAddr;			// 送信元アドレス
	int				nStatus;
	MSGDATA			SendMessage;		// ユーザ列挙応答送信用バッファ
	int				nLen;

	// 無限ループ
	while (1)
	{
		nLen = sizeof(sockaddr);	// 送信元アドレス長

		// データ受信
		nStatus = recvfrom(g_sockNo,		// ソケット番号
			(char *)g_recvBuffer.szData,	// 受信バッファ
			sizeof(g_recvBuffer),			// 受信バッファバイト長
			0,
			(sockaddr*)&fromAddr,			// 送信元アドレス
			&nLen);							// 第５引数のバイト長をセット
		 
		g_IpAdoless = fromAddr.sin_addr.S_un.S_addr;

		// ソケットエラーでなければ処理
		if (nStatus != SOCKET_ERROR)
		{
			// 列挙要求の受信時の処理をプレイヤー情報に変更
			switch (g_recvBuffer.Msg.Header.nType) {
			case MSGTYPE_MACH_REQUEST:// マッチング要求を受け取った時（マッチング前）
				if (IsEqualGUID(g_recvBuffer.Msg.Body0.guid, g_MyGuid)) {
					SendMessage.Msg.Header.nType = MSGTYPE_MACH_RESPONSE;
					//strcpy(SendMessage.szData, "test");

					g_enemyAddr = fromAddr;	// 相手のを入れる

					//　相手に送り返す
					sendto(g_sockNo, SendMessage.szData, sizeof(SendMessage.szData), 0, (sockaddr *)&fromAddr, sizeof(fromAddr));
				}
				break;
			case MSGTYPE_MACH_RESPONSE://マッチング要求への応答を受け取った時（マッチング後）
				g_enemyAddr = fromAddr;
				g_SetMach = true;
				break;
			case MSGTYPE_PLAYER_STATE:	// 相手のプレイヤー情報
				// 列挙応答を受信した際の処理をプレイヤー情報に変更する
				// g_ClientList にプレイヤー情報を追加または更新する処理を記述する
				g_ClientList[g_nListCnt].fromAddr = fromAddr;
				// ポジション
				DirectX::XMFLOAT3 pos2;
				pos2.x = g_recvBuffer.Msg.Body1.Position.x;
				pos2.y = g_recvBuffer.Msg.Body1.Position.y;
				pos2.z = g_recvBuffer.Msg.Body1.Position.z;
				// 向き
				DirectX::XMFLOAT3 rote2;
				rote2.x = g_recvBuffer.Msg.Body1.Rotation.x;
				rote2.y = g_recvBuffer.Msg.Body1.Rotation.y;
				rote2.z = g_recvBuffer.Msg.Body1.Rotation.z;
				// アタック
				bool Attack;
				Attack = g_recvBuffer.Msg.Body1.Attack;
				// 当たったかどうか
				bool Hit;
				Hit = g_recvBuffer.Msg.Body1.Hit;
				// HP
				int HP;
				HP = g_recvBuffer.Msg.Body1.HP;
				
				UDP::get_instance().SetEnemy(pos2, rote2, Attack, Hit, HP);
				break;
			default:
				break;
			}

		}
		else
		{
			break;
		}
	}

	// イベントフラグセット
	SetEvent(g_hEvent);

}