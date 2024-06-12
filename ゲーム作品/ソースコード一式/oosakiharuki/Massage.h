#pragma	once

// ================================
// マクロ定数定義部
// ================================

#define MAX_NAME                     (32)    // ユーザ名文字数
#define MAX_DATA_SIZE                (128)    // 最大データ長
#define MSGTYPE_MACH_REQUEST (0)// マッチング要求
#define MSGTYPE_MACH_RESPONSE (1)// マッチング応答
#define MSGTYPE_PLAYER_STATE (2)// 相手のプレイヤー情報


// ================================
// 構造体宣言部
// ================================
// メッセージヘッダー
typedef struct {
	int nType;	// メッセージタイプ(0=列挙要求、1=列挙応答 
	int nSeqNo;	// シーケンス番号

}MSGHEADER;

// メッセージボディ（メッセージタイプ０）
typedef struct {
	GUID guid;		// プログラム識別用DUID

}MSGBODY0;

// ポジション
typedef struct{
	float x;
	float y;
	float z;
}Pos;

typedef struct {
	float x;
	float y;
	float z;
}Rate;

// メッセージボディ（メッセージタイプ１）
typedef struct {
	Pos Position;
	Rate Rotation;
	bool Attack;
	bool Hit;
	int HP;
}MSGBODY1;

// メッセージの構造
typedef struct {
	MSGHEADER Header;
	union {
		MSGBODY0 Body0;
		MSGBODY1 Body1;
	};
}MESSAGE;

// 送受信メッセージ構造体
typedef struct {
	union {
		MESSAGE Msg;
		char szData[MAX_DATA_SIZE];
	};
}MSGDATA;

