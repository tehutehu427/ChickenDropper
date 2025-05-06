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
	const TYPE GetPlayerNum(void)const;

	/// <summary>
	/// �v���C���[�̐������߂�
	/// </summary>
	/// <param name="_pNum">�v���C���[�̐�</param>
	void SetPlayerNum(const TYPE _pNum);

	//CPU�̐���Ԃ�
	const CPU_NUM GetCPUNum(void)const;

	/// <summary>
	/// CPU�̐������߂�
	/// </summary>
	/// <param name="_cNum">CPU�̐�</param>
	void SetCPUNum(const CPU_NUM _cNum);

	//�ΐ�l����Ԃ�
	const BATTLE_PATTERN GetBattlePattern(void)const;

	/// <summary>
	/// �ΐ�l�������߂�
	/// </summary>
	/// <param name="_pattern">�ΐ�l��</param>
	void SetBattlePattern(const BATTLE_PATTERN _pattern);

	//���[���̏���Ԃ�
	const RULE GetRule(void)const;

	/// <summary>
	/// ���[�������߂�
	/// </summary>
	/// <param name="_ruleType">���[��</param>
	void SetRule(const RULE _ruleType);

	//���ʂ����߂��v���C���[�̐���Ԃ�
	const std::vector<int> GetRank(void)const;

	//���ʂ�Ԃ�
	const int GetRank(const int size)const;

	/// <summary>
	/// ���ʂ����߂�(P1���珇�Ԃ�)
	/// </summary>
	/// <param name="_rank">����</param>
	void SetRank(const int _rank);

	//�z����폜
	void DeleteArray(void);

	/// <summary>
	/// ��Փx��Ԃ�
	/// </summary>
	/// <param name="size">�ق���CPU�̔z��ԍ�</param>
	/// <returns>��Փx</returns>
	const DIFFICULTY GetDifficulty(const int size)const;

	//��Փx�z��̃T�C�Y��Ԃ�
	const size_t GetDifficultySize(void)const;

	/// <summary>
	/// ��Փx��z��̍Ōォ�甲���o��
	/// </summary>
	/// <param name=""></param>
	/// <returns>��Փx</returns>
	const DIFFICULTY PopDifficulty(void);

	//��Փx�����߂�(CPU�̔ԍ�����������)
	void SetDifficulty(const DIFFICULTY _difficulty);

	//��Փx�̔z�����폜
	void DeleteDiff(void);

	//��Փx�̔z���S�č폜
	void DeleteDiffAll(void);

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