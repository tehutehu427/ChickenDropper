//---------------------------------------
//�Q�[������FAGS��2024
//����		�F2316002 �����q��
//---------------------------------------

#include<DxLib.h>
#include"Application.h"

//WinMain
//-------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Application::CreateInstance();

	Application::GetInstance().Run();

	Application::GetInstance().Release();

	return 0;	//�V�X�e���̏I������
}