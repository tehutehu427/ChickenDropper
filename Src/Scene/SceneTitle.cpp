#include<DxLib.h>
#include"../Application.h"
#include"../Common/CommonData.h"
#include"../Manager/InputManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/SceneManager.h"
#include"../Object/Grid.h"
#include"SceneTitle.h"

//�R���X�g���N�^
//SceneTitle::SceneTitle(void)
//{
//	bgImage_ = 0;
//}

// �f�X�g���N�^
//SceneTitle::~SceneTitle(void)
//{
//
//}

//����������
bool SceneTitle::Init(void)
{
	//�T�E���h�̏�����
	sound_ = new SoundManager();
	sound_->SoundInit();

	//�T�E���h�̃��[�h
	sound_->BGMLoad(SoundManager::BGM_TYPE::TITLE);
	sound_->SELoad(SoundManager::SE_TYPE::CLICK);

	//BGM�̍Đ�
	sound_->PlayBGM(SoundManager::BGM_TYPE::TITLE, DX_PLAYTYPE_BACK);

	//���ʂ̔z����폜���Ă���
	CommonData::GetInstance().DeleteArray();

	//�w�i�̃O���t�B�b�N�o�^
	bgImage_ = LoadGraph((Application::PATH_IMAGE + "TitleBack.png").c_str());
	cloudImage_ = LoadGraph((Application::PATH_IMAGE + "cloud.png").c_str());

	cloudPos_ = { 0.0f,Application::SCREEN_SIZE_Y / 2 };

	//�^�C�g�����S�̃O���t�B�b�N�o�^
	logoImage_ = LoadGraph((Application::PATH_IMAGE + "TitleLogo.png").c_str());

	return true;
}

//�X�V����
void SceneTitle::Update(void)
{
	if (InputManager::GetInstance().IsJoypadKeyPush(DX_INPUT_KEY_PAD1,PAD_INPUT_1) == 1)
	{
		sound_->PlaySE(SoundManager::SE_TYPE::CLICK,DX_PLAYTYPE_BACK, CLICK_VOLUME);

		//���̃V�[���ɑJ��
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);
	}

	//�_�̓���
	cloudPos_.x--;
	if (cloudPos_.x < -Application::SCREEN_SIZE_X)
	{
		cloudPos_.x = 0;
	}
}

//�`�揈��
void SceneTitle::Draw(void)
{
	//�t�H���g
	int fontSize = 45;
	int fontTick = 5;
	auto font = CreateFontToHandle(NULL, fontSize, fontTick, DX_FONTTYPE_EDGE);
	unsigned int color = 0;

	//�w�i�摜
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0, 0.0, bgImage_, true,false);
	DrawRotaGraph(cloudPos_.x
				, cloudPos_.y
				, 1.0
				, 0.0, cloudImage_, true,false);
	DrawRotaGraph(cloudPos_.x + Application::SCREEN_SIZE_X
				, cloudPos_.y
				, 1.0
				, 0.0, cloudImage_, true,false);
	DrawRotaGraph(cloudPos_.x + Application::SCREEN_SIZE_X * 2
				, cloudPos_.y
				, 1.0
				, 0.0, cloudImage_, true,false);

	//���S
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 3, 1.0, 0.0, logoImage_, true);

	//�t�H���g�̐F
	color = ((static_cast<int>(cloudPos_.x) / 20) % 2 == 0) ? 0xffffff : 0xaaaaaa;

	//����
	DrawFormatStringToHandle(Application::SCREEN_SIZE_X / 2 - CHAR_POS_X
		, Application::SCREEN_SIZE_Y / 2 + CHAR_POS_Y
		, color
		, font
		, GetJoypadNum() >= 1 ? " A�{�^���������Ă�������" : " Z�L�[�������Ă�������");

	//�t�H���g���
	DeleteFontToHandle(font);
}

//�������
bool SceneTitle::Release(void)
{
	//�T�E���h�̉��
	sound_->SoundRelease();
	delete sound_;
	sound_ = nullptr;

	//�^�C�g���摜�̉��
	DeleteGraph(bgImage_);
	DeleteGraph(cloudImage_);
	DeleteGraph(logoImage_);

	return true;
}