#include<DxLib.h>
#include"../Application.h"
#include"../Utility/Utility.h"
#include"../Common/CommonData.h"
#include"../Manager/Resource.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/InputManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/SceneManager.h"
#include"../Object/Grid.h"
#include"SceneTitle.h"

 //�f�X�g���N�^
SceneTitle::~SceneTitle(void)
{

}

//����������
bool SceneTitle::Init(void)
{
	//�C���X�^���X�擾
	ResourceManager& res = ResourceManager::GetInstance();
	CommonData& common = CommonData::GetInstance();

	//�T�E���h�̏�����
	sound_ = new SoundManager();
	sound_->SoundInit();

	//�T�E���h�̃��[�h
	sound_->BGMLoad(SoundManager::BGM_TYPE::TITLE);
	sound_->SELoad(SoundManager::SE_TYPE::CLICK);

	//BGM�̍Đ�
	sound_->PlayBGM(SoundManager::BGM_TYPE::TITLE, DX_PLAYTYPE_BACK);

	//���ʂ̔z����폜���Ă���
	common.DeleteArray();

	//�w�i�̃O���t�B�b�N�o�^
	bgImage_ = res.Load(ResourceManager::SRC::TITLE_BACK).handleId_;
	cloudImage_ = res.Load(ResourceManager::SRC::CLOUD).handleId_;

	//�^�C�g�����S�̃O���t�B�b�N�o�^
	logoImage_ = res.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;

	//�_�̈ʒu
	cloudPos_ = { 0.0f,Application::SCREEN_SIZE_Y / 2 };

	return true;
}

//�X�V����
void SceneTitle::Update(void)
{
	//�C���X�^���X�擾
	InputManager& input = InputManager::GetInstance();

	if (input.IsJoypadKeyPush(DX_INPUT_KEY_PAD1,PAD_INPUT_1) == 1)
	{
		sound_->PlaySE(SoundManager::SE_TYPE::CLICK,DX_PLAYTYPE_BACK, CLICK_VOLUME);

		//���̃V�[���ɑJ��
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);
	}

	//�_�̓���
	cloudPos_.x--;
	if (cloudPos_.x < -Application::SCREEN_SIZE_X)
	{
		//�����ʒu�ɖ߂�
		cloudPos_.x = 0;
	}
}

//�`�揈��
void SceneTitle::Draw(void)
{
	//�t�H���g
	int font = CreateFontToHandle(NULL, FONT_SIZE, FONT_TICKNESS, DX_FONTTYPE_EDGE);
	unsigned int color;

	//�t�H���g�̐F
	color = ((static_cast<int>(cloudPos_.x) / FONT_BLINKING_INTERVAL) % 2 == 0) ? Utility::COLOR_WHITE : Utility::COLOR_GRAY;

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

	//����(�p�b�h�ڑ��ɂ���ĕς��)
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