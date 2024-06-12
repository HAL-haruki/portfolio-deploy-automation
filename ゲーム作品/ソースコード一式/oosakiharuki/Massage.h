#pragma	once

// ================================
// �}�N���萔��`��
// ================================

#define MAX_NAME                     (32)    // ���[�U��������
#define MAX_DATA_SIZE                (128)    // �ő�f�[�^��
#define MSGTYPE_MACH_REQUEST (0)// �}�b�`���O�v��
#define MSGTYPE_MACH_RESPONSE (1)// �}�b�`���O����
#define MSGTYPE_PLAYER_STATE (2)// ����̃v���C���[���


// ================================
// �\���̐錾��
// ================================
// ���b�Z�[�W�w�b�_�[
typedef struct {
	int nType;	// ���b�Z�[�W�^�C�v(0=�񋓗v���A1=�񋓉��� 
	int nSeqNo;	// �V�[�P���X�ԍ�

}MSGHEADER;

// ���b�Z�[�W�{�f�B�i���b�Z�[�W�^�C�v�O�j
typedef struct {
	GUID guid;		// �v���O�������ʗpDUID

}MSGBODY0;

// �|�W�V����
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

// ���b�Z�[�W�{�f�B�i���b�Z�[�W�^�C�v�P�j
typedef struct {
	Pos Position;
	Rate Rotation;
	bool Attack;
	bool Hit;
	int HP;
}MSGBODY1;

// ���b�Z�[�W�̍\��
typedef struct {
	MSGHEADER Header;
	union {
		MSGBODY0 Body0;
		MSGBODY1 Body1;
	};
}MESSAGE;

// ����M���b�Z�[�W�\����
typedef struct {
	union {
		MESSAGE Msg;
		char szData[MAX_DATA_SIZE];
	};
}MSGDATA;

