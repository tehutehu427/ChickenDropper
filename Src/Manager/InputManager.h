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

	/// <summary>
	/// �P���ɉ�����Ă��邩
	/// </summary>
	/// <param name="_keyCode">�m�F����L�[�R�[�h</param>
	/// <returns>true:������Ă���</returns>
	const bool IsKeyDown(const int _keyCode);

	/// <summary>
	/// �������������ꂽ��
	/// </summary>
	/// <param name="_keyCode">�m�F����L�[�R�[�h</param>
	/// <returns>true:�����ꂽ</returns>
	const bool IsKeyPush(const int _keyCode);

	/// <summary>
	/// �����������Ă��邩
	/// </summary>
	/// <param name="_keyCode">�m�F����L�[�R�[�h</param>
	/// <returns>true:�����������Ă���</returns>
	const bool IsKeyKeep(const int _keyCode);

	/// <summary>
	/// �������������ꂽ��
	/// </summary>
	/// <param name="_keyCode">�m�F����L�[�R�[�h</param>
	/// <returns>true:�����ꂽ</returns>
	const bool IsKeyRelease(const int _keyCode);

	//�R���g���[���[
	//---------------------------
	
	//���䏈��(�R���g���[���[)
	void StepPadInput(void);

	/// <summary>
	/// �P���ɉ�����Ă��邩(�R���g���[���[)
	/// </summary>
	/// <param name="_padNum">�p�b�h�ԍ�</param>
	/// <param name="_keyCode">�m�F����L�[�R�[�h</param>
	/// <returns>true:������Ă���</returns>
	const bool IsJoypadKeyDown(const int _padNum, const int _keyCode);

	/// <summary>
	/// �������������ꂽ��(�R���g���[���[)
	/// </summary>
	/// <param name="_padNum">�p�b�h�ԍ�</param>
	/// <param name="_keyCode">�m�F����L�[�R�[�h</param>
	/// <returns>true:�����ꂽ</returns>
	const bool IsJoypadKeyPush(const int _padNum, const int _keyCode);

	/// <summary>
	/// �����������Ă��邩(�R���g���[���[)
	/// </summary>
	/// <param name="_padNum">�p�b�h�ԍ�</param>
	/// <param name="_keyCode">�m�F����L�[�R�[�h</param>
	/// <returns>true:�����������Ă���</returns>
	const bool IsJoypadKeyKeep(const int _padNum, const int _keyCode);

	/// <summary>
	/// �������������ꂽ��(�R���g���[���[)
	/// </summary>
	/// <param name="_padNum">�p�b�h�ԍ�</param>
	/// <param name="_keyCode">�m�F����L�[�R�[�h</param>
	/// <returns></returns>
	const bool IsJoypadKeyRelease(const int _padNum, const int _keyCode);

	/// <summary>
	/// �w��t���[������������ꂽ��(�R���g���[���[)
	/// </summary>
	/// <param name="_padNum">�p�b�h�ԍ�</param>
	/// <param name="_keyCode">�m�F����L�[�R�[�h</param>
	/// <param name="_keepTime">�w�肷��t���[��</param>
	/// <returns>true:�����ꂽ</returns>
	const KEEP_FUNCTION HowLongJoypadKeyKeep(const int _padNum, const int _keyCode, const int _keepTime);

	//�C���X�^���X�̍폜
	void Destroy(void);

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
};