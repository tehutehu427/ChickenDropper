#pragma once

class InputManager
{
public:
	//�񋓌^
	//-----------------------

	//�L�[�̉���������ꂽ���ǂ����̋@�\
	enum class KEEP_FUNCTION
	{
		NONE			//��������������Ă��Ȃ�
		,KEEP_TRUE		//����������ꂽ
		,KEEP_FALSE		//�r���ŗ����ꂽ
	};

	//�萔
	//-----------------------
	static constexpr int KEY_BUF_LEN = 256;		//�L�[�{�[�h�̃L�[�̐�

	static constexpr int PAD_KEY_BUF_LEN = 32;	//�Q�[���p�b�h�̓��͗̈搔

	static constexpr int PAD_NUM = 4;			//�R���g���[���[�̍ő�ڑ���

	//����������
	void InitInput(void);

	//���䏈��
	void StepInput(void);

	//�P���ɉ�����Ă��邩
	bool IsKeyDown(int _keyCode);

	//�������������ꂽ��
	bool IsKeyPush(int _keyCode);

	//�����������Ă��邩
	bool IsKeyKeep(int _keyCode);

	//�������������ꂽ��
	bool IsKeyRelease(int _keyCode);

	//�R���g���[���[
	//---------------------------
	
	//���䏈��(�R���g���[���[)
	void StepPadInput(void);

	//�P���ɉ�����Ă��邩(�R���g���[���[)
	bool IsJoypadKeyDown(int _padNum, int _keyCode);

	//�������������ꂽ��(�R���g���[���[)
	bool IsJoypadKeyPush(int _padNum, int _keyCode);

	//�����������Ă��邩(�R���g���[���[)
	bool IsJoypadKeyKeep(int _padNum, int _keyCode);

	//�������������ꂽ��(�R���g���[���[)
	bool IsJoypadKeyRelease(int _padNum, int _keyCode);

	//���t���[������������ꂽ��(�R���g���[���[)
	KEEP_FUNCTION HowLongJoypadKeyKeep(int _padNum, int _keyCode, int _keepTime);

	//�������
	void InputRelease(void);

	//�V���O���g����
	//----------------------------
	
	//�O������ÓI�ɃC���X�^���X�̐���������
	static void CreateInstance(void);

	//�O���ŃC���X�^���X�𗘗p�\�Ƃ��邽�߂�
	//�C���X�^���X��Ԃ��֐������(�Q�ƌ^�ł��A�|�C���^�^�ł��悢)
	static InputManager& GetInstance(void);

private:
	//�����o�ϐ�
	//-----------------------
	//���݃t���[���̃L�[���
	char currentKeyBuf_[KEY_BUF_LEN] = { 0 };

	//�O�t���[���̃L�[���
	char preKeyBuf_[KEY_BUF_LEN] = { 0 };

	//���݃t���[���̃L�[���(�R���g���[���[)
	int currentPadKeyBuf_[PAD_NUM];

	//�O�t���[���̃L�[���(�R���g���[���[)
	int prePadKeyBuf_[PAD_NUM];

	//�L�[�̉����ꂽ����(�R���g���[���[)
	int pushPadKeyTime_[PAD_NUM][PAD_KEY_BUF_LEN];

	//�L�[�̉����ꂽ���̃g���K�[(�R���g���[���[)
	bool pushPadKeyTrg_[PAD_NUM][PAD_KEY_BUF_LEN];

	//�ÓI�ȃC���X�^���X�i�[�̈�
	static InputManager* instance_;

	//�V���O���g����
	//--------------------------------

	void Destroy(void);
};