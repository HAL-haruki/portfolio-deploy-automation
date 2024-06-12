#pragma once

#include <debugapi.h>
#include <Windows.h>
#include <stdio.h>

void PrintLog(const char* format, ...)
{
	//参考サイト
	//https://ez-net.jp/article/E3/CQ4fxR9H/br4mR3gSb_sE/
	//https://donadona.hatenablog.jp/entry/2016/02/07/171459

	//--- 変数宣言
	char buf[1024];		//バッファ
	va_list ap;			//可変長を操作する為の構造体
	int resultNum = 0;	//処理結果の数値を格納

	va_start(ap, format);	//可変長引数の操作を開始

	resultNum = vsprintf_s(buf, format, ap);

	//--- エラー検出処理
	if (resultNum < 0)
	{
		OutputDebugString("\nLog output failed");	//エラーメッセージを出力

		//--- 終了処理
		va_end(ap);		//可変長引数の操作を終了
		return;			//強制終了
	}

	OutputDebugString(buf);		//指定された文字列を出力

	va_end(ap);	//可変長引数の操作を終了
}