//#pragma once

#include"../Common/CommonData.h"
#include"../Utility/Utility.h"

class SceneGame;
class Player;

class Attack
{
public:
	//�萔
	//-------------------
	static constexpr float ATK_SPEED = 10.0;		//�U���̑��x

	//�����o�[�֐�
	//-------------------
	
	//�f�t�H���g�R���X�g���N�^
	Attack(void);			

	//�f�X�g���N�^
	~Attack(void);			
	
	/// <summary>
	/// ���������� 
	/// </summary>
	/// <param name="parent">�V�[���Q�[��</param>
	/// <param name="parPlayer">�e�v���C���[</param>
	/// <param name="_pos">�����ʒu</param>
	/// <param name="_dir">��������</param>
	/// <param name="_CharaNum">�e�v���C���[�̃v���C���[�ԍ�</param>
	/// <returns></returns>
	bool Init(SceneGame* parent
			, Player* parPlayer
			, const VECTOR _pos
			, const Utility::DIR_3D _dir
			, const CommonData::TYPE _charaNum);

	//�X�V����
	void Update(void);							

	//�`�揈��
	void Draw(void);							

	//�������
	bool Release(void);							

	//�U���̌��ݍ��W��Ԃ�
	const VECTOR GetPos(void);
	//�U���̍��W�����߂�
	void SetPos(const VECTOR _setPos);

	//�U���̐��������Ԃ�
	const bool GetIsAlive(void);
	//�U���̐��������ݒ�
	void SetIsAlive(const bool _isAttack);

	//�U���̕�����Ԃ�
	const Utility::DIR_3D GetAttackDir(void);
	//�U���̕����ݒ�
	void SetAttackDir(const Utility::DIR_3D _setDir);

	//�U���̃v���C���[�ԍ���Ԃ�
	const CommonData::TYPE GetCharaNum(void);

	//�U���Œ��̌o�ߎ��Ԃ�Ԃ�
	const float GetAtkCnt(void);

	//�U���̍폜
	void Destroy(void);

private:
	//�|�C���^
	//-------------------
	SceneGame* sceneGame_;		//�Q�[���V�[���̃|�C���^

	Player* player_;			//�v���C���[�̃|�C���^

	//�����o�[�ϐ�
	//-------------------
	int images_;				//�U���̉摜�i�[�̈�
	
	VECTOR pos_;				//�U���̍��W

	Utility::DIR_3D dir_;	//�U���̌���

	CommonData::TYPE charaNum_;	//�e�v���C���[�ԍ�

	bool isAlive_;			//�U���̐�������

	float speed_;			//�U���̑��x

	float atkCnt_;			//�U���Œ��̌o�ߎ��ԃJ�E���g�p

	//�����o�[�֐�
	//-------------------
};