//#pragma once

#include<string>

class Application
{
public:
	//�萔
	//-------------------------

	//��ʃT�C�Y

	//�g�嗦
	static constexpr float MAGNIFICATION = 4.0f / 5.0f;
	
	//�t���X�N���[�� 
	static constexpr int SCREEN_SIZE_X = 1920 * MAGNIFICATION;
	static constexpr int SCREEN_SIZE_Y = 1080 * MAGNIFICATION;

	//static constexpr int SCREEN_SIZE_X = 1200;
	//static constexpr int SCREEN_SIZE_Y = 800;

	//�t���[�����[�g�֌W
	static constexpr int ONE_SEC_TO_FRAME = 1000;		//1�b���~���b�ϊ�
	static constexpr int DRAW_FPS_SHIFT_POS_X = -110;	//�t���[�����[�g�`��p

	//�f�[�^�p�X
	static const std::string PATH_IMAGE;	//�摜�̃p�X
	static const std::string PATH_MODEL;	//���f���̃p�X
	static const std::string PATH_EFFECT;	//�G�t�F�N�g�̃p�X
	static const std::string PATH_BGM;		//BGM�̃p�X
	static const std::string PATH_SE;		//SE�̃p�X


	//�����o�[�֐�
	//-----------------------------
	
	//����������
	bool Init(void);	
	//�Q�[�����[�v����
	void Run(void);		
	//�������
	bool Release(void);	

	//�V���O���g����
	//--------------------------
	//�O������ÓI�ɃC���X�^���X�̐���������
	static void CreateInstance(void);

	//�O���ŃC���X�^���X�𗘗p�\�Ƃ��邽�߂ɁA
	//�C���X�^���X��Ԃ��֐������
	static Application& GetInstance(void);

private:
	//�V���O���g����
	//---------------------------
	
	//�R���X�g���N�^
	Application(void);		
	//�f�X�g���N�^
	~Application(void);		

	//�R�s�[�R���X�g���N�^�𗘗p�ł��Ȃ��悤�ɂ���
	Application(const Application& ins);

	//�C���X�^���X�̔j��
	void Destroy(void);

	//�����o�[�ϐ�
	//--------------------------
	//�ÓI�ȃC���X�^���X�i�[�̈�
	static Application* instance_;

	//�t���[�����[�g�֘A
	//---------------------------------
	int currentTime_ = 0;			//���ݎ���
	int lastFrameTime_ = 0;			//�O��̃t���[�����s���̎���

	int frameCnt_ = 0;				//�t���[���J�E���g�p
	int updateFrameRateTime_ = 0;	//�t���[�����[�g���X�V��������

	float frameRate_ = 0.f;			//�t���[�����[�g(�\���p)

	bool isDrawFrameRate_ = false;	//�t���[�����[�g�̕\���t���O(true:�\��)

	//�t���[�����[�g�v�Z
	void CalcFrameRate();

	//�t���[�����[�g�\��(�f�o�b�O�p)
	void DrawFrameRate();
};