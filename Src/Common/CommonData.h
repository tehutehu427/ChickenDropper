#pragma once

#include<vector>

class CommonData
{
public:
	//�񋓌^
	//--------------------
	//�v���C���[�̎��
	enum class TYPE
	{
		NONE
		, P1		//�v���C���[�P
		, P2		//�v���C���[�Q
		, P3		//�v���C���[�R
		, P4		//�v���C���[�S(�ő吔)
	};

	//CPU��
	enum class CPU_NUM
	{
		NONE	//�Ȃ�
		,C1		//CPU 1�l
		,C2		//CPU 2�l
		,C3		//CPU 3�l
	};

	//�ΐ�`��
	enum class BATTLE_PATTERN
	{
		P1C1		//�v���C���[1�l CPU1�l
		,P1C2		//�v���C���[1�l CPU2�l
		,P1C3		//�v���C���[1�l CPU3�l
		,P2			//�v���C���[2�l
		,P2C1		//�v���C���[2�l CPU1�l
		,P2C2		//�v���C���[2�l CPU2�l
		,P3			//�v���C���[3�l
		,P3C1		//�v���C���[3�l CPU1�l
		,P4			//�v���C���[4�l
		,MAX
	};

	//���[��
	enum class RULE
	{
		NONE
		, TIME			//���Ԑ�
		, LIFE			//���C�t��
		, BREAK_TILE	//���̔j��
	};

	//��Փx
	enum class DIFFICULTY
	{
		EASY		//�ȒP
		,NORMAL		//����
		,HARD		//���
	};

	//�����o�[�֐�
	//--------------------------
	
	//����������
	bool Init(void);	

	//�C���X�^���X�̔j��
	void Destroy(void);

	//�v���C���[�̐���Ԃ�
	TYPE GetPlayerNum(void);						

	/// <summary>
	/// �v���C���[�̐������߂�
	/// </summary>
	/// <param name="_pNum">�v���C���[�̐�</param>
	void SetPlayerNum(TYPE _pNum);					

	//CPU�̐���Ԃ�
	CPU_NUM GetCPUNum(void);						

	/// <summary>
	/// CPU�̐������߂�
	/// </summary>
	/// <param name="_cNum">CPU�̐�</param>
	void SetCPUNum(CPU_NUM _cNum);					

	//�ΐ�l����Ԃ�
	BATTLE_PATTERN GetBattlePattern(void);			

	/// <summary>
	/// �ΐ�l�������߂�
	/// </summary>
	/// <param name="_pattern">�ΐ�l��</param>
	void SetBattlePattern(BATTLE_PATTERN _pattern);	

	//���[���̏���Ԃ�
	RULE GetRule(void);								

	/// <summary>
	/// ���[�������߂�
	/// </summary>
	/// <param name="_ruleType">���[��</param>
	void SetRule(RULE _ruleType);		

	//���ʂ����߂��v���C���[�̐���Ԃ�
	std::vector<int> GetRank(void);

	//���ʂ�Ԃ�
	int GetRank(int size);

	/// <summary>
	/// ���ʂ����߂�(P1���珇�Ԃ�)
	/// </summary>
	/// <param name="_rank">����</param>
	void SetRank(int _rank);

	//�z����폜
	void DeleteArray(void);

	/// <summary>
	/// ��Փx��Ԃ�
	/// </summary>
	/// <param name="size">�ق���CPU�̔z��ԍ�</param>
	/// <returns>��Փx</returns>
	DIFFICULTY GetDifficulty(int size);

	//��Փx�z��̃T�C�Y��Ԃ�
	size_t GetDifficultySize(void);

	/// <summary>
	/// ��Փx��z��̍Ōォ�甲���o��
	/// </summary>
	/// <param name=""></param>
	/// <returns>��Փx</returns>
	DIFFICULTY PopDifficulty(void);

	//��Փx�����߂�(CPU�̔ԍ�����������)
	void SetDifficulty(DIFFICULTY _difficulty);

	//��Փx�̔z�����폜
	void DeleteDiff(void);

	//��Փx�̔z���S�č폜
	void DeleteDiffAll(void);

	//�`���[�g���A���̗L����Ԃ�
	bool GetIsTutorial(void);

	/// <summary>
	/// �`���[�g���A���̗L����ݒ�
	/// </summary>
	/// <param name="_isTutorial">�`���[�g���A���̗L��(true:�`���[�g���A��������)</param>
	void SetIsTutorial(bool _isTutorial);

	//�V���O���g����
	//--------------------------
	
	//�O������ÓI�ɃC���X�^���X�̐���������
	static void CreateInstance(void);

	//�O���ŃC���X�^���X�𗘗p�\�Ƃ��邽�߂ɁA
	//�C���X�^���X��Ԃ��֐������
	static CommonData& GetInstance(void);

private:
	//�����o�[�ϐ�
	//--------------------------
	TYPE playerNum_;				//�v���C���[�̐l��

	CPU_NUM cpuNum_;				//CPU�̐�

	BATTLE_PATTERN battlePattern_;	//�ΐ�l��

	RULE rule_;						//���[��		

	std::vector<int> rank_;			//����

	std::vector<DIFFICULTY> difficulty_;			//��Փx

	bool isTutorial_;				//�`���[�g���A���̗L��

	//�V���O���g����
	//---------------------------
	
	//�R���X�g���N�^
	CommonData(void);		
	//�f�X�g���N�^
	~CommonData(void);		

	//�R�s�[�R���X�g���N�^�𗘗p�ł��Ȃ��悤�ɂ���
	CommonData(const CommonData& ins);

	//�ÓI�ȃC���X�^���X�i�[�̈�
	static CommonData* instance_;

};