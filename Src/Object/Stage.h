//#pragma once

#include"../Utility/Utility.h"
#include"../Common/Vector2F.h"
#include"../Common/CommonData.h"

class SoundManager;
class SceneGame;

class Stage
{
public:
	//�񋓌^
	//-------------------
	enum class TILE_STATE
	{
		NORMAL		//�ʏ�
		,ATTACK		//�U��
		,BROKEN		//�j��
		,PREEND		//�I���O
		,END		//�I��
		,MAX
	};

	enum class TILE_TYPE
	{
		PAT_1_1			//�p�^�[��1-1
		,PAT_1_2		//�p�^�[��1-2
		, P1_PAT_1_1	//�v���C���[1�̃p�^�[��1-1
		, P1_PAT_1_2	//�v���C���[1�̃p�^�[��1-2
		, P2_PAT_1_1	//�v���C���[2�̃p�^�[��1-1
		, P2_PAT_1_2	//�v���C���[2�̃p�^�[��1-2
		, P3_PAT_1_1	//�v���C���[3�̃p�^�[��1-1
		, P3_PAT_1_2	//�v���C���[3�̃p�^�[��1-2
		, P4_PAT_1_1	//�v���C���[4�̃p�^�[��1-1
		, P4_PAT_1_2	//�v���C���[4�̃p�^�[��1-2
		, PAT_1_1_END	//�p�^�[��1-1��END
		, PAT_1_2_END	//�p�^�[��1-2��END
		,MAX
	};

	//�萔
	//-------------------
	//�^�C��
	static constexpr float TILE_SIZE_X = 64.0f;				//�^�C���̉��T�C�Y
	static constexpr float TILE_SIZE_Y = 20.0f;				//�^�C���̏c�T�C�Y
	static constexpr float TILE_SIZE_Z = 64.0f;				//�^�C���̉��s�T�C�Y

	static constexpr float TILE_HSIZE_X = TILE_SIZE_X / 2;	//�^�C���̉��T�C�Y�̔���
	static constexpr float TILE_HSIZE_Y = TILE_SIZE_Y / 2;	//�^�C���̏c�T�C�Y�̔���
	static constexpr float TILE_HSIZE_Z = TILE_SIZE_Z / 2;	//�^�C���̉��s�T�C�Y�̔���

	static constexpr int TILE_NUM = 9;						//�P���C���̃^�C����

	static constexpr int TILE_HNUM = TILE_NUM / 2;			//�P���C���̃^�C�����̔���

	static constexpr float TILE_BREAK_TIME = 5.0f;				//�^�C��������܂ł̎���
	static constexpr float TILE_REPAIR_TIME = 90.0f;			//�^�C���C���܂ł̎���
	static constexpr float TILE_PREEND_TIME = 5.0f;				//�^�C���I���O�܂ł̎���
	static constexpr float TILE_END_TIME = 60.0f;				//�^�C���I���܂ł̎���

	//�X�e�[�W
	static constexpr float STAGE_SIZE_X = TILE_SIZE_X * TILE_NUM;	//�X�e�[�W�̉��T�C�Y
	static constexpr float STAGE_SIZE_Z = TILE_SIZE_Z * TILE_NUM;	//�X�e�[�W�̏c�T�C�Y

	static constexpr float STAGE_HSIZE_X = STAGE_SIZE_X / 2;		//�X�e�[�W�̉��T�C�Y�̔���
	static constexpr float STAGE_HSIZE_Z = STAGE_SIZE_Z / 2;		//�X�e�[�W�̏c�T�C�Y�̔���
	
	static constexpr int STAGE_NARROW_RANGE = 2;														//�X�e�[�W�k���͈�
	static constexpr Utility::DIR_3D STAGE_NARROW_DIR = Utility::DIR_3D::RIGHT;					//�X�e�[�W�k���J�n����

	//�����o�[�֐�
	//-------------------
	
	//�f�t�H���g�R���X�g���N�^
	Stage(void);					
	//�f�X�g���N�^
	~Stage(void);					

	//����������
	bool Init(SceneGame* parent);		
	//�X�V����
	void Update(void);					
	//�`�揈��
	void Draw(void);					
	//�������
	bool Release(void);					

	/// <summary>
	/// �^�C���̏�ԑJ�ڏ���
	/// </summary>
	/// <param name="ix">X�̃^�C���ԍ�</param>
	/// <param name="iz">Z�̃^�C���ԍ�</param>
	/// <param name="_stateSwap">�ω���������</param>
	void TileSwap(int ix,int iz , TILE_STATE _stateSwap);

	/// <summary>
	/// �U�������v���C���[�̔ԍ����^�C���ɗ^����
	/// </summary>
	/// <param name="ix">X�̃^�C���ԍ�</param>
	/// <param name="iz">Z�̃^�C���ԍ�</param>
	/// <param name="_charaNum">�U�������v���C���[</param>
	void TileAttackPlayer(int ix, int iz, CommonData::TYPE _charaNum);

	/// <summary>
	/// ���̃^�C�����U�������v���C���[�̔ԍ���Ԃ�
	/// </summary>
	/// <param name="ix">X�̃^�C���ԍ�</param>
	/// <param name="iz">Z�̃^�C���ԍ�</param>
	/// <returns>�U�������v���C���[�̔ԍ�</returns>
	CommonData::TYPE GetTileAttackPlayer(int ix, int iz);

	/// <summary>
	/// �U�����Ԃ�Ԃ�
	/// </summary>
	/// <param name="ix">X�̃^�C���ԍ�</param>
	/// <param name="iz">Z�̃^�C���ԍ�</param>
	/// <returns></returns>
	float GetAtkCnt(int ix, int iz);					

	/// <summary>
	/// �U�����Ԃ�^����
	/// </summary>
	/// <param name="ix">X�̃^�C���ԍ�</param>
	/// <param name="iz">Z�̃^�C���ԍ�</param>
	/// <param name="_cnt">�^���鎞��</param>
	void SetAtkCnt(int ix, int iz, float _cnt);			
	
	//1���C���̃^�C������Ԃ�
	int GetTileHNum(void);				

	/// <summary>
	/// �^�C���̍��W��Ԃ�
	/// </summary>
	/// <param name="ix">X�̃^�C���ԍ�</param>
	/// <param name="iz">Z�̃^�C���ԍ�</param>
	/// <returns></returns>
	VECTOR GetPos(int ix, int iz);		
	
	/// <summary>
	/// �^�C���̏�Ԃ�Ԃ�
	/// </summary>
	/// <param name="ix">X�̃^�C���ԍ�</param>
	/// <param name="iz">Z�̃^�C���ԍ�</param>
	/// <returns></returns>
	TILE_STATE GetTileState(int ix, int iz);		

	/// <summary>
	/// �^�C���̔Ԓn��bool�ŕԂ�
	/// </summary>
	/// <param name="ix">X�̃^�C���ԍ�</param>
	/// <param name="iz">Z�̃^�C���ԍ�</param>
	/// <returns>���̔Ԓn��true�ŕԂ�</returns>
	bool GetTileAdd(int ix, int iz);

private:
	//�|�C���^
	//-------------------
	SceneGame* sceneGame_;		//�Q�[���V�[���̃|�C���^

	SoundManager* se_;			//SE�̃|�C���^

	//�����o�[�ϐ�
	//--------------------------
	int backImage_;				//�w�i�摜�̊i�[�̈�
	int cloudImage_;			//�w�i�̉_�摜�̊i�[�̈�
	Vector2F cloudPos_;			//�_�摜�̍��W

	int model_[static_cast<int>(TILE_TYPE::MAX)];		//�X�e�[�W�̃��f���i�[�̈�
	int map_[TILE_NUM][TILE_NUM][static_cast<int>(TILE_TYPE::MAX)];						//�X�e�[�W�̊i�[�̈�
	
	VECTOR pos_[TILE_NUM][TILE_NUM];		//�^�C���̍��W
	VECTOR scale_;							//�^�C���̑傫��	
	VECTOR rot_;							//�^�C���̉�]

	TILE_STATE tileState_[TILE_NUM][TILE_NUM];		//�^�C���̏�Ԋi�[�̈�

	CommonData::TYPE attackPlayer_[TILE_NUM][TILE_NUM];	//�^�C���ɍU�������v���C���[

	float tileSwapTime_[TILE_NUM][TILE_NUM];		//�^�C���̏�ԑJ�ڃJ�E���g
	float swapTimeCompare_[TILE_NUM][TILE_NUM];		//��ԑJ�ڃJ�E���g�̎��Ԓ����p

	float atkCnt_[TILE_NUM][TILE_NUM];				//�U������U���I���܂ł̌v������

	int endCnt_;						//END��Ԃɂ��邽�߂̃J�E���^

	int endTileNumX_;					//END��Ԃɂ���^�C����X�ԍ�
	int endTileNumZ_;					//END��Ԃɂ���^�C����Z�ԍ�
	Utility::DIR_3D endMoveDir_;	//END��Ԃɂ��Ă�������

	int endTileNum_;					//�������^�C���̐�
	int stageNarrowRange_;				//�X�e�[�W�k���J�E���^

	//�����o�[�֐�
	//--------------------------

	//�X�e�[�W�����߂�
	void NarrowStage(void);
};