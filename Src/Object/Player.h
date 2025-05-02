#pragma once

#include<DxLib.h>
#include"../Common/CommonData.h"
#include"../Utility/NodyUtility.h"

class SceneGame;
class EffectManager;
class SoundManager;
class Attack;

class Player
{
public:
	//�񋓌^
	//-------------------
	//�v���C���[��CPU���̔��f
	enum class CHARA_JUDGE
	{
		PLAYER		//�v���C���[
		,CPU		//CPU
		,MAX
	};

	//�L�����̏�Ԕ���
	enum class STATE
	{
		NORMAL		//�ʏ���
		,ATTACK		//�U�����
		,DEAD		//������
		,MAX
	};

	//�A�j���[�V�����ԍ�
	enum class ANIM_NUM
	{
		NONE
		,IDLE		//�ҋ@
		,WALK		//����
		,ATTACK		//�U��
		,FALL		//����
		,RANK1ST	//1��
		,RANK2ND	//2��
		,RANK3RD	//3��
		,RANK4TH	//4��
	};

	//�萔
	//-------------------
	static constexpr float RADIUS = 24.0;				//�v���C���[�̔��a

	static constexpr int MOVE_KEY_KEEP_TIME = 8;		//�ړ��ɕK�v�ȃL�[�������ݎ���(�t���[��)				

	static constexpr float SPEED_NORMAL = 4.0f;			//�v���C���[�̒ʏ푬�x

	static constexpr int MAX_LIFE = 5;					//�v���C���[�̍ő�̗͐�

	static constexpr int DAMAGE = 1;					//��{�_���[�W
	static constexpr int SELF_SUB_SCORE = 2;			//���œ_

	static constexpr int SUB_TILE_BROKE_SCORE = 5;		//������^�C���X�R�A�̊���

	static constexpr float FALL_SPEED = 5.0f;			//�������x

	static constexpr float INVINCIBLE = 100.0f;			//���G����

	static constexpr float SPEED_ANIM = 60.0f;			//�A�j���[�V�������x

	static constexpr float ATK_COOLDOWN_CPU_EASY = 180.0f;		//EASY��CPU�p�̍U���̍Ĕ�������
	static constexpr float ATK_COOLDOWN_CPU_NORMAL = 100.0f;	//NORMAL��CPU�p�̍U���̍Ĕ�������
	static constexpr float ATK_COOLDOWN_CPU_HARD = 60.0f;		//HARD��CPU�p�̍U���̍Ĕ�������

	static constexpr float CPU_MOVE_INTERVAL_EASY = 60.0f;		//EASY��CPU�̈ړ��Ԋu
	static constexpr float CPU_MOVE_INTERVAL_NORMAL = 40.0f;	//NORMAL��CPU�̈ړ��Ԋu
	static constexpr float CPU_MOVE_INTERVAL_HARD = 20.0f;		//HARD��CPU�̈ړ��Ԋu

	static constexpr int CPU_MOVE_INTERVAL_RAND_EASY = 21;		//EASY��CPU�̈ړ��Ԋu�����炷�p
	static constexpr int CPU_MOVE_INTERVAL_RAND_NORMAL = 10;	//NORMAL��CPU�̈ړ��Ԋu�����炷�p
	static constexpr int CPU_MOVE_INTERVAL_RAND_HARD = 7;		//HARD��CPU�̈ړ��Ԋu�����炷�p

	static constexpr int NUM_SIZE = 70;				//�ԍ��̑傫��
	static constexpr int NUM_DIS = 35;				//�ԍ��ƃv���C���[�̋���

	static constexpr float NUM_VIEW_TIME = 180.0f;	//�ԍ��̕\������
	
	//�����o�[�֐�
	//-------------------
	
	//�f�t�H���g�R���X�g���N�^
	Player(void);					
	//�f�X�g���N�^
	~Player(void);					

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="parent">�e</param>
	/// <param name="_pos">�����ʒu</param>
	/// <param name="_charaNum">�v���C���[�ԍ�</param>
	/// <returns></returns>
	bool Init(SceneGame* parent, VECTOR _pos, CommonData::TYPE _charaNum);	

	//�X�V����
	void Update(void);			

	//�`�揈��
	void Draw(void);														
	//�������
	bool Release(void);														

	//�ړ�����(�v���C���[)
	void MovePlayer(void);
	//�ړ�����(CPU)
	void MoveCPU(void);

	//��]����
	void ProcessRot(void);

	/// <summary>
	/// �_���[�W�����̏���
	/// </summary>
	/// <param name="_charaNum">�U�����Ă����v���C���[�̔ԍ�</param>
	void PreDamage(CommonData::TYPE _charaNum);

	/// <summary>
	/// �_���[�W����
	/// </summary>
	/// <param name="_damage">�^����_���[�W���͉�����X�R�A�̗�</param>
	void Damage(int _damage);

	/// <summary>
	/// ���_�̏���
	/// </summary>
	/// <param name="_score">���_����X�R�A</param>
	void TakeScore(int _score);

	//���݂̓��_��Ԃ�
	int GetScore(void);

	//�U���̐���(�v���C���[)
	void ProcessAttackPlayer(void);
	//�U���̐���(CPU)
	void ProcessAttackCPU(void);
	//�U���̐���
	void GenerateAttack(void);
	//�L���ȍU�����擾
	Attack* GetValidAttack(void);		

	//�v���C���[�̏�Ԃ�Ԃ�
	STATE GetState(void);

	/// <summary>
	/// �v���C���[�̏�ԕύX
	/// </summary>
	/// <param name="_state">�ύX���������</param>
	void ChangeState(STATE _state);

	//�v���C���[�̌��ݍ��W��Ԃ�
	VECTOR GetPos(void);				

	/// <summary>
	/// �v���C���[�̍��W��^����
	/// </summary>
	/// <param name="_setPos">�^������pos</param>
	void SetPos(VECTOR _setPos);		

	//�v���C���[�̕�����Ԃ�
	NodyUtility::DIR_3D GetDir(void);	

	//�v���C���[�������Ă��邩��Ԃ�
	bool GetIsWalk(void);

	//�v���C���[�̗̑͂�Ԃ�
	int GetLife(void);

	//�v���C���[�̂��ꂽ���Ԃ�Ԃ�
	int GetDeadTime(void);

	//�v���C���[�̂��ꂽ���Ԃ�^����(GetNowCount��^����)
	void SetDeadTime(void);

	//�v���C���[�̉󂵂��^�C���̐���Ԃ�
	int GetBrokeTileNum(void);

	//�v���C���[�̉󂵂��^�C���̐��𑝂₷
	void IncreaseBrokeTileNum(void);

	//�v���C���[�̏��ʂ�Ԃ�
	int GetRank(void);

	//�v���C���[�̏��ʂ����߂�
	void SetRank(int _rank);

	/// <summary>
	/// �v���C���[�̃A�j���[�V�����ԍ���Ԃ�
	/// </summary>
	/// <returns>�Q�ƃv���C���[�̃A�j���[�V�����ԍ�</returns>
	ANIM_NUM GetAnimNum(void);

	//�A�j���[�V�����ԍ��ύX
	void ChangeAnim(ANIM_NUM _animNo);

	//�v���C���[�̖��G���Ԃ�Ԃ�
	float GetInvincible(void);			

	//�v���C���[���G����Ԃ�
	CHARA_JUDGE GetCharaJudge(void);

	/// <summary>
	/// �v���C���[���G����^����
	/// </summary>
	/// <param name="_type">�^����^�C�v</param>
	void SetCharaJudge(CHARA_JUDGE _type);	

	//�v���C���[�ԍ���Ԃ�
	CommonData::TYPE GetCharaNum(void);

	//�U���̃|�C���^��Ԃ�
	std::vector<Attack*> GetAttack(void);	

	/// <summary>
	/// ��Փx��ݒ�
	/// </summary>
	/// <param name="_diff">�ݒ肷���Փx</param>
	void SetDifficulty(CommonData::DIFFICULTY _diff);

	/// <summary>
	/// �v���C���[�̑��x�����߂�
	/// </summary>
	/// <param name="_speed">���x</param>
	void SetSpeed(float _speed);

	void RunMoveInterval(void);

private:
	//�|�C���^
	//-------------------
	SceneGame* sceneGame_;		//�Q�[���V�[���̃|�C���^

	EffectManager* effect_;		//�G�t�F�N�g�̃C���X�^���X�p

	SoundManager* se_;			//SE�̃C���X�^���X�p

	std::vector<Attack*> attack_;	//�U���̃C���X�^���X�p

	//�����o�ϐ�
	//-------------------
	int model_;							//�v���C���[�̉摜�i�[�̈�

	VECTOR pos_;						//�v���C���[�̍��W
	VECTOR scale_;						//�v���C���[�̑傫��
	VECTOR rot_;						//�v���C���[�̉�]

	NodyUtility::DIR_3D dir_;			//�v���C���[�̌���

	STATE state_;						//�v���C���[�̏��

	float speed_;						//�v���C���[�̈ړ���

	bool isWalk_;						//�����Ă��邩(true:�����Ă���)

	int killScore_;						//�����|������

	int life_;							//�v���C���[�̗̑�

	CommonData::TYPE attackedPlayer_;	//�U�����Ă����v���C���[

	int deadTime_;						//�v���C���[�̂��ꂽ����

	int brokeTileNum_;					//�v���C���[�̉󂵂��^�C���̐�

	int rank_;							//�v���C���[�̏���

	int animAttachNo_;					//�A�j���[�V�����A�^�b�`�ԍ�

	ANIM_NUM animNum_;					//�A�j���[�V�����̔ԍ�

	float animTotalTime_;				//�A�j���[�V�����̑��Đ�����

	float stepAnim_;					//�Đ����̃A�j���[�V��������

	float speedAnim_;					//�A�j���[�V�����̑��x

	float atkCoolDown_;					//CPU�p�̍U���̍Ĕ������Ԃ̃J�E���g�p

	float isEndAtkCoolDownCPU_;			//CPU�p�̍U���̍Ĕ������Ԃ̃J�E���g�̔���(true:������I����)

	float invincible_;					//���G���ԗp

	CHARA_JUDGE charaJudge_;			//�v���C���[��CPU���̔��f�p

	CommonData::TYPE charaNum_;			//�v���C���[�ԍ�

	int  numImage_;						//�v���C���[�ԍ��̕\���p
	float numViewTime_;					//�v���C���[�ԍ��̕\������

	int padNum_;						//�p�b�h�ԍ�

	CommonData::DIFFICULTY difficulty_;		//CPU�̓�Փx

	float CPUMoveInterVal_;					//CPU�̈ړ��Ԋu�J�E���^
	bool randInterval_;						//CPU�̈ړ��Ԋu���炵�p
	bool isEndMoveInterval_;				//CPU�̈ړ��Ԋu�J�E���^���I������(true:�I����)

	bool isMoveFront_;						//�O�ɓ����邩(true:������) CPU�p
	bool isMoveRight_;						//�E�ɓ����邩(true:������) CPU�p
	bool isMoveBack_;						//���ɓ����邩(true:������) CPU�p
	bool isMoveLeft_;						//���ɓ����邩(true:������) CPU�p		

	//�����o�[�֐�
	//------------------------

	/// <summary>
	/// CPU�̈ړ����f����
	/// </summary>
	/// <param name="_dir">�ŏ��̎Q�ƕ���</param>
	/// <returns>�ړ��������</returns>
	NodyUtility::DIR_3D CPUMoveChack(NodyUtility::DIR_3D _dir);

	//��Ԃ��Ƃ̍X�V����
	void UpdateState(void);

	//�A�j���[�V�����̍X�V����
	void UpdateAnimation(void);

	//�����A�j���[�V����
	void FallAnimation(void);

	//�A�j���[�V����
	void Animation(void);

	//�X�V����(�v���C���[)
	void UpdatePlayer(void);

	//�X�V����(CPU)
	void UpdateCPU(void);
};