#pragma once

#include <debugapi.h>
#include <Windows.h>
#include <stdio.h>

void PrintLog(const char* format, ...)
{
	//�Q�l�T�C�g
	//https://ez-net.jp/article/E3/CQ4fxR9H/br4mR3gSb_sE/
	//https://donadona.hatenablog.jp/entry/2016/02/07/171459

	//--- �ϐ��錾
	char buf[1024];		//�o�b�t�@
	va_list ap;			//�ϒ��𑀍삷��ׂ̍\����
	int resultNum = 0;	//�������ʂ̐��l���i�[

	va_start(ap, format);	//�ϒ������̑�����J�n

	resultNum = vsprintf_s(buf, format, ap);

	//--- �G���[���o����
	if (resultNum < 0)
	{
		OutputDebugString("\nLog output failed");	//�G���[���b�Z�[�W���o��

		//--- �I������
		va_end(ap);		//�ϒ������̑�����I��
		return;			//�����I��
	}

	OutputDebugString(buf);		//�w�肳�ꂽ��������o��

	va_end(ap);	//�ϒ������̑�����I��
}