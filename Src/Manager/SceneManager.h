//#pragma once

//�N���X�̑O���錾
class Camera;
class Fader;
class SceneBase;

class SceneManager
{
public:
	//�񋓌^
	//-----------------
	//�V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE
		,TITLE		//�^�C�g��
		,SELECT		//�Z���N�g
		,GAME		//�Q�[��
		,RESULT		//���U���g
	};

	//�萔
	//-------------------

	//�ŏ��̃V�[��
	static constexpr SCENE_ID FIRST_SCENE = SCENE_ID::TITLE;	

	//�����o�[�֐�
	//-------------------
	
	//����������
	bool Init(void);		
	//3D�Ɋւ��鏉���ݒ�
	void Init3D(void);		

	//�X�V����
	void Update(void);		
	//�`�揈��
	void Draw(void);		
	//�������
	bool Release(void);		

	//�V�[���؂�ւ����˗�����֐�
	void ChangeScene(SCENE_ID nextID, bool isToFade);

	//�V���O���g����
	//----------------------------
	
	//�O������ÓI�ɃC���X�^���X�̐���������
	static void CreateInstance(void);

	//�O���ŃC���X�^���X�𗘗p�\�Ƃ��邽�߂�
	//�C���X�^���X��Ԃ��֐������(�Q�ƌ^�ł��A�|�C���^�^�ł��悢)
	static SceneManager& GetInstance(void);

private:
	//�����o�[�ϐ�
	//------------------------
	//�V�[��ID
	SCENE_ID sceneID_;				//���݂̃V�[��ID
	SCENE_ID waitSceneID_;			//���ɑJ�ڂ���ID

	//�V�[���J��
	bool isSceneChanging_;			//�V�[���J�ڂ̃t���O(true:�J�ڒ�)

	//�J����
	Camera* camera_;				//�J�����̃C���X�^���X�p

	//�t�F�[�h
	Fader* fader_;					//�t�F�[�h�̃C���X�^���X�p

	//�V�[���Ǘ�
	SceneBase* scene_;				//�V�[���̃C���X�^���X�Ǘ��p

	//�ÓI�ȃC���X�^���X�i�[�̈�
	static SceneManager* instance_;

	//�����o�[�֐�
	//--------------------------
	//�V�[���؂�ւ���
	void DoChangeScene(void);				
	//�t�F�[�h���{�p�֐�
	void Fade(void);						
	//�w�肵���V�[���̉���p
	void ReleaseScene(SCENE_ID iSceneID);	

	//�V���O���g����
	//----------------------------
	//�f�t�H���g�R���X�g���N�^
	SceneManager(void);

	// �f�X�g���N�^
	~SceneManager(void);

	//�R�s�[�R���X�g���N�^�𗘗p�ł��Ȃ��悤�ɂ���
	SceneManager(const SceneManager& ins);

	//�C���X�^���X�̔j��
	void Destroy(void);


};