// �x���Ή�
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//------------------------------------
// �C���N���[�h��
//------------------------------------
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

#include <process.h>	// �}���`�X���b�h��(�u���b�L���O�֐��Ή�)
#include <time.h>		// ���O�@�\�p

#include "UDP.h"
#include "Massage.h"

#include "input.h"

// ���C�u����
#pragma comment(lib,"ws2_32.lib")
#pragma	comment(lib,"winmm.lib")

// �}�N����`
#define		PORT_NO  	(20250)

#define		INTERVAL	   (10)
#define		MAX_PLAYER	   (10)

// �\���̒�`
// �X���b�h�ɓn���\����
typedef struct
{
	SOCKET	sockNo;		// �\�P�b�g�ԍ�
	HANDLE	hEvent;		// �C�x���g�n���h��
}RecvInfo;

// �N���C�A���g���
typedef struct
{
	sockaddr_in		fromAddr;			// �A�h��
	Pos Position;
	Rate Rotation;
}ClientInfo;

// �O���[�o���ϐ��錾
u_short		g_portNo = PORT_NO;		// �|�[�g�ԍ�
SOCKET		g_sockNo;				// �\�P�b�g�ԍ�
sockaddr_in	g_myAddr;				// �����̃A�h���X
sockaddr_in	g_enemyAddr;				// aite�̃A�h���X

ClientInfo	g_ClientList[MAX_PLAYER];
HANDLE		g_hEvent;				// �C�x���g�n���h��
MSGDATA		g_recvBuffer;			// ��M�o�b�t�@
char		g_szServerIP[128];		// �T�[�o�[IP�A�h���X

char		g_szMyName[MAX_NAME];	// �����̖��O
int			g_nListCnt;				// �񋓌���
int			g_nRequestNo;			// �񋓗v���ԍ�
int			g_nNextRequestNo;		// ���Ɏg�p�����񋓗v���ԍ�
std::string g_IpAdoless;

// ���݂̏��
bool g_SetMach = false;

// GUID

// {A7C22E16-9F27-409B-AAA7-19FE6970F6B3}
static const GUID g_MyGuid =
{ 0xa7c22e16, 0x9f27, 0x409b,{ 0xaa, 0xa7, 0x19, 0xfe, 0x69, 0x70, 0xf6, 0xb3 } };



// ������IP�A�h���X���擾
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
	MSGDATA		SendMessage;		// ���[�U�񋓗v�����M�p
	int			nStatus;			// socket�֐��ߒl�i�[�p
	sockaddr_in	toAddr;				// ���M��A�h���X
	SendMessage.Msg.Header.nType = MSGTYPE_MACH_REQUEST;
	SendMessage.Msg.Header.nSeqNo = g_nNextRequestNo;
	memcpy(&SendMessage.Msg.Body0.guid, &g_MyGuid, sizeof(g_MyGuid));

	toAddr.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");

	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(PORT_NO);
	// ���M
	sendto(g_sockNo, SendMessage.szData, sizeof(SendMessage.szData), 0, (sockaddr *)&toAddr, sizeof(toAddr));

}
void UDP::sendPlayerState() {

	MSGDATA		SendMessage;		// ���[�U�񋓗v�����M�p
	int			nStatus;			// socket�֐��ߒl�i�[�p
	sockaddr_in	toAddr;				// ���M��A�h���X
	SendMessage.Msg.Header.nType = MSGTYPE_PLAYER_STATE;
	SendMessage.Msg.Header.nSeqNo = g_nNextRequestNo;
	//memcpy(&SendMessage.Msg.Body1, &g_MyGuid, sizeof(g_MyGuid));
	// �|�W�V����
	auto pos = UDP::get_instance().GetPlayerPos();
	SendMessage.Msg.Body1.Position.x = pos.x;
	SendMessage.Msg.Body1.Position.y = pos.y;
	SendMessage.Msg.Body1.Position.z = pos.z;
	// ����
	auto rote = UDP::get_instance().GetPlayerRate();
	SendMessage.Msg.Body1.Rotation.x = rote.x;
	SendMessage.Msg.Body1.Rotation.y = rote.y;
	SendMessage.Msg.Body1.Rotation.z = rote.z;
	// �A�^�b�N
	auto Attack = UDP::get_instance().GetAttack();
	SendMessage.Msg.Body1.Attack = Attack;
	// �����������ǂ���
	auto Hit = UDP::get_instance().GetHit();
	SendMessage.Msg.Body1.Hit = Hit;
	// HP
	auto HP = UDP::get_instance().GetHP();
	SendMessage.Msg.Body1.HP = HP;

	// ���M��̐ݒ�
	toAddr.sin_addr.S_un.S_addr = g_enemyAddr.sin_addr.S_un.S_addr;
	//toAddr.sin_addr.S_un.S_addr = inet_addr("10.130.21.14");
	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(PORT_NO);


	// �V�[�P���X�ԍ��̍X�V
	// �񋓐��̃N���A
	g_nListCnt = 0;
	// ���݂̔ԍ����X�V
	g_nRequestNo = g_nNextRequestNo;
	// ���g���ԍ����X�V
	g_nNextRequestNo++;

	// ���M����
	sendto(g_sockNo, SendMessage.szData, sizeof(SendMessage.szData), 0, (sockaddr *)&toAddr, sizeof(toAddr));

	// �v���C���[���̏�����

}

bool UDP::SetMach()
{
	return g_SetMach;
}

void UDP::InitUdp() {
	WSADATA		wsaData;					// WSADATA�i�[�p
	int			nStatus;					// socket�֐��ߒl�i�[�p
	int			nErrCode;					// �G���[�R�[�h�i�[�p
	bool		flg = true;

	g_IpAdoless = "255.255.255.255";

	// WINSOCK������
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	strcpy(g_szServerIP, GetMyIP());

	// �\�P�b�g����(IPv4,�_�C�A�O�����ʐM,UDP�v���g�R��)
	g_sockNo = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// �����̃\�P�b�g�ɂh�o�A�h���X�A�|�[�g�ԍ������蓖�Ă�
	g_myAddr.sin_port = htons(g_portNo);
	g_myAddr.sin_family = AF_INET;
	g_myAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(g_sockNo, (sockaddr*)&g_myAddr, sizeof(sockaddr));

	// �u���[�h�L���X�g���\�ɂ���
	setsockopt(g_sockNo, SOL_SOCKET, SO_BROADCAST, (char*)&flg, sizeof(bool));

	g_hEvent = CreateEvent(NULL, true, false, NULL);		// �C�x���g�n���h������

	// �f�[�^����M���邽�߂̃X���b�h���J�n������
	_beginthread(RecvClientData, 2048, NULL);
}

 void UDP::UninitUdp() {
	 // �\�P�b�g���N���[�Y
	 closesocket(g_sockNo);

	 // �T�u�X���b�h�̏I����҂�
	 WaitForMultipleObjects(1, &g_hEvent, true, INFINITE);

	 // �C�x���g�n���h���̃N���[�Y
	 CloseHandle(g_hEvent);

	 // WINSOCK�̌㏈��
	 WSACleanup();
}

 // --�񋓗v���̎�M�Ɨ񋓉����̑��M(�ʃX���b�h)
void RecvClientData(void *p1) {
	sockaddr_in		fromAddr;			// ���M���A�h���X
	int				nStatus;
	MSGDATA			SendMessage;		// ���[�U�񋓉������M�p�o�b�t�@
	int				nLen;

	// �������[�v
	while (1)
	{
		nLen = sizeof(sockaddr);	// ���M���A�h���X��

		// �f�[�^��M
		nStatus = recvfrom(g_sockNo,		// �\�P�b�g�ԍ�
			(char *)g_recvBuffer.szData,	// ��M�o�b�t�@
			sizeof(g_recvBuffer),			// ��M�o�b�t�@�o�C�g��
			0,
			(sockaddr*)&fromAddr,			// ���M���A�h���X
			&nLen);							// ��T�����̃o�C�g�����Z�b�g
		 
		g_IpAdoless = fromAddr.sin_addr.S_un.S_addr;

		// �\�P�b�g�G���[�łȂ���Ώ���
		if (nStatus != SOCKET_ERROR)
		{
			// �񋓗v���̎�M���̏������v���C���[���ɕύX
			switch (g_recvBuffer.Msg.Header.nType) {
			case MSGTYPE_MACH_REQUEST:// �}�b�`���O�v�����󂯎�������i�}�b�`���O�O�j
				if (IsEqualGUID(g_recvBuffer.Msg.Body0.guid, g_MyGuid)) {
					SendMessage.Msg.Header.nType = MSGTYPE_MACH_RESPONSE;
					//strcpy(SendMessage.szData, "test");

					g_enemyAddr = fromAddr;	// ����̂�����

					//�@����ɑ���Ԃ�
					sendto(g_sockNo, SendMessage.szData, sizeof(SendMessage.szData), 0, (sockaddr *)&fromAddr, sizeof(fromAddr));
				}
				break;
			case MSGTYPE_MACH_RESPONSE://�}�b�`���O�v���ւ̉������󂯎�������i�}�b�`���O��j
				g_enemyAddr = fromAddr;
				g_SetMach = true;
				break;
			case MSGTYPE_PLAYER_STATE:	// ����̃v���C���[���
				// �񋓉�������M�����ۂ̏������v���C���[���ɕύX����
				// g_ClientList �Ƀv���C���[����ǉ��܂��͍X�V���鏈�����L�q����
				g_ClientList[g_nListCnt].fromAddr = fromAddr;
				// �|�W�V����
				DirectX::XMFLOAT3 pos2;
				pos2.x = g_recvBuffer.Msg.Body1.Position.x;
				pos2.y = g_recvBuffer.Msg.Body1.Position.y;
				pos2.z = g_recvBuffer.Msg.Body1.Position.z;
				// ����
				DirectX::XMFLOAT3 rote2;
				rote2.x = g_recvBuffer.Msg.Body1.Rotation.x;
				rote2.y = g_recvBuffer.Msg.Body1.Rotation.y;
				rote2.z = g_recvBuffer.Msg.Body1.Rotation.z;
				// �A�^�b�N
				bool Attack;
				Attack = g_recvBuffer.Msg.Body1.Attack;
				// �����������ǂ���
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

	// �C�x���g�t���O�Z�b�g
	SetEvent(g_hEvent);

}