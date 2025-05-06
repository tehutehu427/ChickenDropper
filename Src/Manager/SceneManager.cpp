#include<DxLib.h>
#include<EffekseerForDXLib.h>
#include"Camera.h"
#include"../Common/Fader.h"
#include"../Scene/SceneTitle.h"
#include"../Scene/SceneSelect.h"
#include"../Scene/SceneGame.h"
#include"../Scene/SceneResult.h"
#include"Resource.h"
#include"ResourceManager.h"
#include"SceneManager.h"

//�V���O���g����
//----------------------------
//instance�̏�����
SceneManager* SceneManager::instance_ = nullptr;

//�f�t�H���g�R���X�g���N�^
SceneManager::SceneManager(void)
{
	sceneID_ = SCENE_ID::NONE;
	waitSceneID_ = SCENE_ID::NONE;
	isSceneChanging_ = false;
	camera_ = nullptr;
	fader_ = nullptr;
	scene_ = nullptr;
}

// �f�X�g���N�^
SceneManager::~SceneManager(void)
{

}

//����������
bool SceneManager::Init(void)
{
	//�R�c�̏����ݒ�
	Init3D();

	//�ϐ��̏�����
	//---------------------------
	//�t�F�[�h�̐ݒ�
	fader_ = new Fader();
	fader_->Init();

	//�J�����̐ݒ�
	camera_ = new Camera();
	if (camera_->Init() == false)
	{
		OutputDebugString("Camera.cpp�̏��������s");
		return false;
	}

	//�V�[���̐�������
	sceneID_ = SCENE_ID::NONE;
	waitSceneID_ = FIRST_SCENE;		//�ŏ��̃V�[��
	DoChangeScene();

	//�ŏ��̃V�[�����t�F�[�h�C��������
	fader_->SetFade(Fader::STATE::FADE_IN);
	isSceneChanging_ = true;

	return true;
}

//3D�Ɋւ��鏉���ݒ�
void SceneManager::Init3D(void)
{
	//�w�i�F�ݒ�
	SetBackgroundColor(0, 0, 0);

	//Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	//Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	//�o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	//���C�g��L����
	SetUseLighting(true);

	//�f�B���N�V���i�����C�g�̕����̐ݒ�(���K������Ă��Ȃ��Ă��悢)
	//���ʂ���΂߉��Ɍ����������C�g
	ChangeLightTypeDir({ 0.00f,-1.00f,1.00f });
}

//�X�V����
void SceneManager::Update(void)
{
	//�t�F�[�h�̏���
	//�t�F�[�h���s���Ă��Ȃ��Ȃ珈�����N���Ȃ�
	fader_->Update();

	//�t�F�[�h��
	if (isSceneChanging_ == true)
	{
		Fade();
	}
	//�t�F�[�h�I��
	else
	{
		//�e�V�[���̍X�V����
		scene_->Update();

		//�J�����̍X�V����
		camera_->Update();
	}
}

//�`�揈��
void SceneManager::Draw(void)
{
	//�J�����ݒ�
	camera_->SetBeforeDraw();

	//�J�����̕`��
	camera_->Draw();

	//Dx���C�u�����̃J�����ƃG�t�F�N�V�A�̃J�����𓯊�����
	Effekseer_Sync3DSetting();

	//���Ԍ���!!�J���������̌�!!
	//Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V
	UpdateEffekseer3D();

	//�e�V�[���̕`�揈��
	scene_->Draw();

	//�G�t�F�N�g�̕`��
	DrawEffekseer3D();

	//�t�F�[�h(�K���Ō�)
	fader_->Draw();
}

//�V�[���؂�ւ����˗�����֐�
//isToFade��true�Ȃ�΃t�F�[�h�A�E�g
void SceneManager::ChangeScene(const SCENE_ID nextID, const bool isToFade)
{
	//�t�F�[�h�̏������I����Ă�����V�[����ς���K�v�����邽��
	//�J�ڐ�V�[���������o�ϐ��ɕێ�����
	waitSceneID_ = nextID;

	if (isToFade)
	{
		//�t�F�[�h�����{���A�V�[���J�ڂ�����
		fader_->SetFade(Fader::STATE::FADE_OUT);
		isSceneChanging_ = true;
	}
	else
	{
		//�t�F�[�h�����{�����A�V�[���J�ڂ�����
		DoChangeScene();
	}
}

//�V�[���؂�ւ���
void SceneManager::DoChangeScene(void)
{
	//���\�[�X�}�l�[�W���擾
	ResourceManager& res = ResourceManager::GetInstance();

	//���݂̃V�[��()���������
	ReleaseScene();

	//���Ɉړ�����V�[��(waitSceneID_)�𐶐�����
	//---------------------------------------------------
	//���݂̃V�[�������̃V�[���ɓ���ւ���
	sceneID_ = waitSceneID_;

	//�e�V�[���̏���������
	switch (sceneID_)
	{
	case SceneManager::SCENE_ID::TITLE:
		//�^�C�g���p���\�[�X�擾
		res.InitTitle();

		//�^�C�g���V�[���̃C���X�^���X����
		scene_ = new SceneTitle();
		break;

	case SceneManager::SCENE_ID::SELECT:
		//�Z���N�g�V�[���p���\�[�X�擾
		res.InitSelect();

		//�Z���N�g�V�[���̃C���X�^���X����
		scene_ = new SceneSelect();

		//�Z���N�g�V�[���p�̃J�������[�N�ݒ�
		camera_->SetCamerawork(SELECT_CAMERA_POS, SELECT_CAMERA_ANGLE);
		break;

	case SceneManager::SCENE_ID::GAME:
		//�Q�[���V�[���p���\�[�X�擾
		res.InitGame();

		//�Q�[���V�[���̃C���X�^���X
		scene_ = new SceneGame();

		//�Q�[���V�[���p�̃J�������[�N�ݒ�
		camera_->SetCamerawork(GAME_CAMERA_POS, GAME_CAMERA_ANGLE);
		break;

	case SceneManager::SCENE_ID::RESULT:
		//���U���g�V�[���p���\�[�X�擾
		res.InitResult();

		//���U���g�V�[���̃C���X�^���X
		scene_ = new SceneResult();

		//���U���g�V�[���p�̃J�������[�N�ݒ�
		camera_->SetCamerawork(RESULT_CAMERA_POS, RESULT_CAMERA_ANGLE);
		break;

	default:
		break;
	}
	
	//�Ή��V�[���̏�����
	scene_->Init();

	//�V�[���̑@�ۂ��I�������̂ŁA���̃V�[�����N���A����
	waitSceneID_ = SCENE_ID::NONE;
}

//�t�F�[�h���{�p�֐�
void SceneManager::Fade(void)
{
	//���݂̃t�F�[�h�ݒ���擾
	Fader::STATE fState = fader_->GetState();

	//�t�F�[�h����
	switch (fState)
	{
	case Fader::STATE::FADE_OUT:
		if (fader_->IsEnd() == true)
		{
			//�V�[���̐؂�ւ�
			DoChangeScene();

			//�t�F�[�h�Ŗ��邭����
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;

	case Fader::STATE::FADE_IN:
		if (fader_->IsEnd() == true)
		{
			//�t�F�[�h�̏I������
			fader_->SetFade(Fader::STATE::NONE);

			//�V�[���̐؂�ւ��̏I��
			isSceneChanging_ = false;
		}
		break;

	default:
		break;
	}
}

//�w�肵���V�[���̉���p
void SceneManager::ReleaseScene(void)
{
	//�e�V�[���̉������
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
		scene_ = nullptr;
	}
}

//�V���O���g����
//--------------------------
//�O������ÓI�ɃC���X�^���X�̐���������
void SceneManager::CreateInstance(void)
{
	//instance_��nullptr�̏ꍇ�͏��������ꂽ�̂������������s��
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}

	instance_->Init();
}

//�O���ŃC���X�^���X�𗘗p�\�Ƃ��邽�߂�
//�C���X�^���X��Ԃ��֐������(�Q�ƌ^�ł��A�|�C���^�^�ł��悢)
SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

//�C���X�^���X�̔j��
void SceneManager::Destroy(void)
{
	//�V�[���̉��
	ReleaseScene();

	//�J�����̉��
	camera_->Release();
	delete camera_;
	camera_ = nullptr;

	//�t�F�[�h�̉��
	delete fader_;
	fader_ = nullptr;

	//�C���X�^���X�̍폜
	delete instance_;
	instance_ = nullptr;
}