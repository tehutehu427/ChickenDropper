//---------------------------------------
//ゲーム制作：AGS夏2024
//氏名		：2316002 相部航平
//---------------------------------------

#include<DxLib.h>
#include"Application.h"

//WinMain
//-------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application::CreateInstance();

	Application::GetInstance().Run();

	Application::GetInstance().Release();

	return 0;	//システムの終了処理
}