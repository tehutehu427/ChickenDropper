#pragma once

#include<vector>
#include"../Common/CommonData.h"
#include"../Utility/Utility.h"
#include"SceneBase.h"

//�N���X�̑O���錾
class EffectManager;
class SoundManager;
class Stage;
class RuleBase;
class Player;
class ItemBase;

class SceneGame : public SceneBase
{
public:
	//�񋓌^
	//--------------------

	//�Q�[���̏��
	enum class GAME_STATE
	{
		PRE_START	//�Q�[���J�n����
		,START		//�Q�[���J�n
		,GAME		//�Q�[����
		,END		//�Q�[���I��
	};

	//�����o�[�֐�
	//-------------------

	//����������
	bool Init(void) override;		
	//�X�V����
	void Update(void) override;		
	//�`�揈��
	void Draw(void) override;		
	//�������
	bool Release(void) override;	

	//�P���C���̃^�C������Ԃ�
	int GetTileNum(void);		
	//�P���C�����̃^�C������Ԃ�
	int GetTileHNum(void);		

	//�^�C����X�T�C�Y��Ԃ�
	float GetTileSizeX(void);	
	//�^�C����Z�T�C�Y��Ԃ�
	float GetTileSizeZ(void);	

	//�X�e�[�W��X�T�C�Y�̔�����Ԃ�
	float GetStageHSizeX(void);	
	//�X�e�[�W��Z�T�C�Y�̔�����Ԃ�
	float GetStageHSizeZ(void);	

	//�Q�[���̏�Ԃ�Ԃ�
	GAME_STATE GetGameState(void);

	/// <summary>
	/// �Q�[���̏�Ԃ�ύX����
	/// </summary>
	/// <param name="_state">�ύX����Q�[���̏��</param>
	void ChangeGameState(GAME_STATE _state);

	/// <summary>
	/// �U�����Ԃ�Ԃ�
	/// </summary>
	/// <param name="ix">X�̃^�C���ԍ�</param>
	/// <param name="iz">Z�̃^�C���ԍ�</param>
	/// <returns>�Ԃ��U������ ��Stage��atkCnt_��Ԃ�</returns>
	float GetAtkCnt(int ix, int iz);				

	/// <summary>
	/// �U�����Ԃ�^����
	/// </summary>
	/// <param name="ix">X�̃^�C���ԍ�</param>
	/// <param name="iz">Z�̃^�C���ԍ�</param>
	/// <param name="_cnt">�^���鎞��</param>
	void SetAtkCnt(int ix, int iz, float _cnt);		

	/// <summary>
	/// �X�e�[�W�[�Ƃ̏Փ˔���
	/// </summary>
	/// <param name="_pos">���ׂ����v���C���[�̍��W</param>
	/// <param name="_dir">���ׂ����v���C���[�̕���</param>
	/// <returns>�Փ˂�����true</returns>
	bool IsCollisionStageEnd(VECTOR _pos, Utility::DIR_3D _dir);

	/// <summary>
	/// �ׂ̃^�C�������Ă��邩���m���߂�
	/// </summary>
	/// <param name="_pos">�Q�Ƃ���v���C���[��pos</param>
	/// <param name="_dir">�Q�Ƃ���v���C���[��dir</param>
	/// <returns>���Ă�����true</returns>
	bool IsNextTileBroken(VECTOR _pos, Utility::DIR_3D _dir);

	/// <summary>
	/// �ׂ̃^�C�����U����Ԃ����m���߂�
	/// </summary>
	/// <param name="_pos">�Q�Ƃ���v���C���[��pos</param>
	/// <param name="_dir">�Q�Ƃ���v���C���[��dir</param>
	/// <returns>�U����ԂȂ�true</returns>
	bool IsNextTileAttack(VECTOR _pos, Utility::DIR_3D _dir);

	/// <summary>
	/// �������Ă���̃^�C�����U����Ԃ����m���߂�
	/// </summary>
	/// <param name="_pos">�Q�Ƃ���v���C���[��pos</param>
	/// <returns>���Ă�����true</returns>
	bool IsThisTileAttack(VECTOR _pos);

	/// <summary>
	/// �ׂ̃^�C�����U����Ԃ����m���߂�
	/// </summary>
	/// <param name="_pos">�Q�Ƃ���v���C���[��pos</param>
	/// <param name="_dir">�Q�Ƃ���v���C���[��dir</param>
	/// <returns>�U����ԂȂ�true</returns>
	bool IsNextTilePreEnd(VECTOR _pos, Utility::DIR_3D _dir);

	/// <summary>
	/// �������Ă���̃^�C�����U����Ԃ����m���߂�
	/// </summary>
	/// <param name="_pos">�Q�Ƃ���v���C���[��pos</param>
	/// <returns>���Ă�����true</returns>
	bool IsThisTilePreEnd(VECTOR _pos);

	/// <summary>
	/// �ׂ̃^�C�����U����Ԃ����m���߂�
	/// </summary>
	/// <param name="_pos">�Q�Ƃ���v���C���[��pos</param>
	/// <param name="_dir">�Q�Ƃ���v���C���[��dir</param>
	/// <returns>�U����ԂȂ�true</returns>
	bool IsNextTileEnd(VECTOR _pos, Utility::DIR_3D _dir);
	
	//�v���C���[�̃|�C���^��Ԃ�
	std::vector<Player*> GetPlayer(void);

	/// <summary>
	/// �v���C���[�̓��_��Ԃ�
	/// </summary>
	/// <param name="_player">�Q���v���C���[</param>
	/// <returns>�Q�ƃv���C���[�̓��_</returns>
	int GetPlayerScore(Player* _player);

	/// <summary>
	/// �v���C���[�̉��_����
	/// </summary>
	/// <param name="_charaNum">���_����v���C���[</param>
	void TakeScore(CommonData::TYPE _charaNum);

	/// <summary>
	/// �v���C���[�̗̑͂�Ԃ�
	/// </summary>
	/// <param name="_player">�Q�ƃv���C���[</param>
	/// <returns>�Q�ƃv���C���[�̗̑�</returns>
	int GetLife(Player* _player);

	/// <summary>
	/// �v���C���[�̉󂵂��^�C���̐���Ԃ�
	/// </summary>
	/// <param name="_player">�Q�ƃv���C���[</param>
	/// <returns>�Q�ƃv���C���[�̉󂵂��^�C���̐�</returns>
	int GetBrokeTileNum(Player* _player);

	/// <summary>
	/// �v���C���[�̉󂵂��^�C���̐��𑝉�������
	/// </summary>
	/// <param name="_charaNum">���𑝉�������v���C���[�̔ԍ�</param>
	void IncreaseBrokeTileNum(CommonData::TYPE _charaNum);

	/// <summary>
	/// �v���C���[�ԍ���Ԃ�
	/// </summary>
	/// <param name="_player">�Q�ƃv���C���[</param>
	/// <returns>�Q�ƃv���C���[�̃v���C���[�ԍ�</returns>
	CommonData::TYPE GetCharaNum(Player* _player);

	/// <summary>
	///�v���C���[�̕�������
	/// </summary>
	/// <param name="_player">����������v���C���[</param>
	void ReviavalPlayer(Player* _player);

	/// <summary>
	/// �v���C���[���m�̓����蔻��
	/// </summary>
	/// <param name="_pos">�Q�ƃv���C���[��pos</param>
	/// <param name="_dir">�Q�ƃv���C���[��dir</param>
	/// <param name="_num">�Q�ƃv���C���[��charaNum</param>
	/// <returns>�Փ˂�����true</returns>
	bool HitP2P(VECTOR _pos, Utility::DIR_3D _dir, CommonData::TYPE _num);

	/// <summary>
	/// �_���v���C���[�����߂�(CPU�p)
	/// </summary>
	/// <param name="_pos">�Q�ƃv���C���[��pos</param>
	/// <param name="_num">�Q�ƃv���C���[��charaNum</param>
	/// <returns>�_���v���C���[�ւ̕���</returns>
	Utility::DIR_3D AimPlayer(VECTOR _pos ,CommonData::TYPE _num);
	
	/// <summary>
	/// �_�����v���C���[���U���ł��邩�����߂�(CPU�p)
	/// </summary>
	/// <param name="_pos">�Q�ƃv���C���[��pos</param>
	/// <param name="_num">�Q�ƃv���C���[��charaNum</param>
	/// <param name="_dir">�Q�ƃv���C���[��dir</param>
	/// <returns>�U���ł���Ȃ�true</returns>
	bool AimPlayerAttack(VECTOR _pos, CommonData::TYPE _num, Utility::DIR_3D _dir);

	/// <summary>
	/// ����v���C���[�̃^�C���������ɉ󂷏���
	/// </summary>
	/// <param name="_charaNum">�󂵂����v���C���[�^�C��</param>
	void TileNowBreak(CommonData::TYPE _charaNum);

	//�X�e�[�W�k���̃t���O
	bool IsNarrowStage(void);

	/// <summary>
	/// ���W����^�C����X�z��ԍ���Ԃ�
	/// </summary>
	/// <param name="_posX">�Q��X���W</param>
	/// <returns>�^�C����X�z��ԍ�</returns>
	int Pos2TileAddX(float _posX);

	/// <summary>
	/// ���W����^�C����Z�z��ԍ���Ԃ�
	/// </summary>
	/// <param name="_posZ">�Q��Z���W</param>
	/// <returns>�^�C����Z�z��ԍ�</returns>
	int Pos2TileAddZ(float _posZ);

	/// <summary>
	/// �^�C����X�z��ԍ�������W��Ԃ�
	/// </summary>
	/// <param name="_tileAddX">�Q�Ƃ���^�C����X�z��ԍ�</param>
	/// <returns>�^�C���̂���X���W</returns>
	float TileAdd2PosX(int _tileAddX);

	/// <summary>
	/// �^�C����Z�z��ԍ�������W��Ԃ�
	/// </summary>
	/// <param name="_tileAddZ">�Q�Ƃ���^�C����Z�z��ԍ�</param>
	/// <returns>�^�C���̂���Z���W</returns>
	float TileAdd2PosZ(int _tileAddZ);

	/// <summary>
	/// �Q�[���̏I������
	/// </summary>
	/// <param name=""></param>
	/// <returns>�Q�[���I��</returns>
	void EndGame(void);

private:
	//�����o�[�ϐ�
	//--------------------------
	//�C���X�^���X�p

	//�G�t�F�N�g�̃C���X�^���X�p
	EffectManager* effect_;

	//�T�E���h�̃C���X�^���X�p
	SoundManager* sound_;

	//�X�e�[�W�̃C���X�^���X�p
	Stage* stage_;					

	//���[���̃C���X�^���X�p
	RuleBase* rule_;					

	//�v���C���[�̃C���X�^���X�p
	std::vector<Player*> player_;	

	//�A�C�e���̃C���X�^���X�p
	std::vector<ItemBase*>item_;	

	//�Q�[���̏��
	GAME_STATE gameState_;

	//�A�C�e���̏o���Ԋu
	float itemInterval_;

	//�����o�[�֐�
	//--------------------------

	//���[���ݒ�
	void SetRule(void);
	
	//�v���C���[�̏���
	void ProcessPlayer(void);			

	//�A�C�e���̏o������
	void AppearItem(void);

	/// <summary>
	/// �L���ȃA�C�e���z��ԍ���Ԃ�
	/// </summary>
	/// <param name=""></param>
	/// <returns>�g�p�\�ȃA�C�e���̃|�C���^</returns>
	ItemBase* GetValidItem(void);

	/// <summary>
	/// �A�C�e���𐶐�����
	/// </summary>
	/// <param name="_item">��������A�C�e���̃|�C���^</param>
	void GenerateItem(ItemBase* _item);

	/// <summary>
	/// �v���C���[�̃A�C�e���擾����
	/// </summary>
	/// <param name="_player">�擾�����v���C���[</param>
	/// <param name="_item">�擾�����A�C�e��</param>
	void PlayerGotItem(Player* _player, ItemBase* _item);
};