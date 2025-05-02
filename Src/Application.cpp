#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include"Utility/NodyUtility.h"
#include"Manager/InputManager.h"
#include"Manager/EffectManager.h"
#include"Manager/SceneManager.h"
#include"Common/CommonData.h"
#include"Application.h"

//�V���O���g����
//---------------------------
//instance�̏�����
Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_BGM = "Data/Sound/BGM/";
const std::string Application::PATH_SE = "Data/Sound/SE/";

//�ݒ�t���[�����[�g(60����1�b)
constexpr float FRAME_RATE (1000 / 60);

//�R���X�g���N�^
Application::Application(void)
{

}

//�f�X�g���N�^
Application::~Application(void)
{

}

//����������
bool Application::Init(void)
{
	//�V�X�e������
	//------------------------
	SetWindowText("�`�L���h���b�p�[");					//�Q�[���E�B���h�E�̃^�C�g��
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);		//�Q�[���E�B���h�E�̃T�C�Y�ƐF���[�h
	ChangeWindowMode(true);								//�Q�[���E�B���h�E�̕\���ݒ�(false:�t���X�N���[��)
	
	//DxLib�̏�����
	if (DxLib_Init() == 1)								
	{
		OutputDebugString("DxLib�̏��������s\n");
		return false;		//DxLib�̏��������s
	}

	//�C���X�^���X�̗p��
	//------------------------
	//�R�����f�[�^�̃C���X�^���X����(�V���O���g����)
	CommonData::CreateInstance();

	//�V�[���}�l�[�W���̃C���X�^���X����(�V���O���g����)
	SceneManager::CreateInstance();

	//�C���v�b�g�}�l�[�W���̃C���X�^���X����(�V���O���g����)
	InputManager::CreateInstance();

	return true;
}

//�Q�[�����[�v����
void Application::Run(void)
{
	//�Q�[�����[�v
	//-------------------------
	while (ProcessMessage() == 0)
	{
		Sleep(1);		//�V�X�e���ɏ�����Ԃ�

		//���݂̎������擾
		currentTime_ = GetNowCount();

		//���݂̎������A�O��̃t���[�������
		//1/60�b�o�߂��Ă����珈�������s����
		if (currentTime_ - lastFrameTime_ >= FRAME_RATE)
		{
			//�t���[�����s���̎��Ԃ��X�V
			lastFrameTime_ = currentTime_;

			//�t���[�����̃J�E���g
			frameCnt_++;

			//ESCAPE�L�[�������ꂽ��I��
			if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			{
				return;
			}

			//�Q�[���̃��C������
			//-------------------------
			//�V�[���}�l�[�W���̍X�V����(�V���O���g����)
			SceneManager::GetInstance().Update();

			//�`�揈��
			//-------------------------
			SetDrawScreen(DX_SCREEN_BACK);	//�`�悷���ʂ𗠂̉�ʂɐݒ�
			ClearDrawScreen();				//�`�悷���ʂ̓��e������

			//�V�[���}�l�[�W���̕`�揈��(�V���O���g����)
			SceneManager::GetInstance().Draw();

			//�L�[��Ԃ̎擾����(�V���O���g����)
			InputManager::GetInstance().StepInput();

			//�R���g���[���[�̓��͏�Ԃ̎擾����(�V���O���g����)
			InputManager::GetInstance().StepPadInput();


			//�t���[�����[�g�v�Z
			CalcFrameRate();

			//�t���[�����[�g�̕\��(�f�o�b�O)
			DrawFrameRate();

			//���̉�ʂ�\�̉�ʂɃR�s�[
			ScreenFlip();
		}
	}
}

//�������
bool Application::Release(void)
{
	//�L�[�}�l�[�W���̉������
	InputManager::GetInstance().InputRelease();

	//�Ǘ��}�l�[�W���̉������
	//--------------------------------
	//�V�[���}�l�[�W���̉������(�V���O���g����)
	SceneManager::GetInstance().Release();

	//�R�����f�[�^�̉������
	CommonData::GetInstance().Release();

	Destroy();			//�C���X�^���X�̔j��

	//�V�X�e���̏I��
	//----------------------
	DxLib_End();		//DX���C�u�����̏I��

	return true;		//�Q�[���I��
}

//�V���O���g����
//---------------------------
//�O������ÓI�ɃC���X�^���X�̐���������
void Application::CreateInstance(void)
{
	//instance_��nullptr�̏ꍇ�͏��������ꂽ�̂������������s��
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}

	instance_->Init();
}

//�O���ŃC���X�^���X�𗘗p�\�Ƃ��邽�߂ɁA
//�C���X�^���X��Ԃ��֐������
Application& Application::GetInstance(void)
{
	return *instance_;
}

//�C���X�^���X�̔j��
void Application::Destroy(void)
{
	delete instance_;		//�C���X�^���X�̍폜
	instance_ = nullptr;	//�C���X�^���X�̊i�[�̈��������

	//DxLib�I��
	DxLib_End();
}

void Application::CalcFrameRate()
{
	//�O��̃t���[�����[�g�X�V����̌o�ߎ��Ԃ����߂�
	int nDifTime = currentTime_ - updateFrameRateTime_;

	//�O��̃t���[�����[�g���X�V����
	//1�b�ȏ�o�߂��Ă�����t���[�����[�g���X�V����
	if (nDifTime > ONE_SEC_TO_FRAME)
	{
		//�t���[���񐔂�1�~���b�ɍ��킹��
		//�����܂ŏo�����̂�float�ɃL���X�g
		float fFrameCnt = (float)(frameCnt_ * ONE_SEC_TO_FRAME);

		//�t���[�����[�g�����߂�
		//���z�ʂ�Ȃ� 60000 / 1000 �� 60 �ɂȂ�
		frameRate_ = fFrameCnt / nDifTime;

		//�t���[���J�E���g���N���A
		frameCnt_ = 0;

		//�t���[�����[�g�X�V���Ԃ��X�V
		updateFrameRateTime_ = currentTime_;
	}
}

void Application::DrawFrameRate()
{
	int fontHandle;
	int fontSize = 20;
	int fontTickness = 8;

	//�t�H���g�쐬
	fontHandle = CreateFontToHandle(NULL, fontSize, fontTickness, NULL);

	if (InputManager::GetInstance().IsKeyPush(KEY_INPUT_TAB))
	{
		//�t���[�����[�g��`�悷��
		isDrawFrameRate_ = isDrawFrameRate_ ? false : true;
	}

	if (isDrawFrameRate_)
	{
		DrawFormatStringToHandle(SCREEN_SIZE_X + DRAW_FPS_SHIFT_POS_X, 2, NodyUtility::COLOR_YELLOW, fontHandle, "FPS[%.2f]", frameRate_);
	}

	//�t�H���g���
	DeleteFontToHandle(fontHandle);
}
