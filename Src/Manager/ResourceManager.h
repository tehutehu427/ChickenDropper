#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{
		//************************************************************
		// �����ɕۑ����Ă��������摜�⃂�f���A�����Ȃǂ̖��O���L��
		//************************************************************
		
		//�^�C�g��
		TITLE_BACK,		//�^�C�g���w�i
		CLOUD,			//�^�C�g���_
		TITLE_LOGO,		//�^�C�g�����S

		//�Z���N�g
		SELECT_BACK,		//�Z���N�g�w�i
		SELECT_SKY,			//�Z���N�g��
		SELECT_CLOUD,		//�Z���N�g�p�_
		BATTLE_P1C1,		//�v���C���[1�l CPU1�l
		BATTLE_P1C2,		//�v���C���[1�l CPU2�l
		BATTLE_P1C3,		//�v���C���[1�l CPU3�l
		BATTLE_P2,			//�v���C���[2�l
		BATTLE_P2C1,		//�v���C���[2�l CPU1�l
		BATTLE_P2C2,		//�v���C���[2�l CPU2�l
		BATTLE_P3,			//�v���C���[3�l
		BATTLE_P3C1,		//�v���C���[3�l CPU1�l
		BATTLE_P4,			//�v���C���[4�l
		ARROW,				//���
		PAD_NUM,			//�p�b�h���m�F
		EASY,				//��Փx �ȒP
		NORMAL,				//��Փx ����
		HARD,				//��Փx ���
		RULE_SCORE,			//�X�R�A��
		RULE_LIFE,			//���C�t��
		RULE_TILE,			//�^�C���j��
		SCORE_EXPLAIN,		//�X�R�A������
		LIFE_EXPLAIN,		//���C�t������
		TILE_EXPLAIN,		//�^�C���j�󐧐���

		//���[������
		STATE_BLUE,			//�X�e�[�^�X����
		STATE_RED,			//�X�e�[�^�X����
		STATE_GREEN,		//�X�e�[�^�X����
		STATE_YELLOW,		//�X�e�[�^�X����
		NARROW_INFO,		//�k���x��

		//�X�R�A���[��
		KILL_SCORE,			//���Đ�

		//�̗̓��[��
		LIFE_BLUE,			//�̗͐�
		LIFE_RED,			//�̗͐�
		LIFE_GREEN,			//�̗͗�
		LIFE_YELLOW,		//�̗͉�

		//���j�󐔃��[��
		TILE_NUM,			//����

		//���U���g
		RESULT_BACK,		//���U���g�w�i
		CURTAIN_BACK,		//���䖋�w�i
		CURTAIN,			//���䖋
		RANK_1ST,			//1��
		RANK_2ND,			//2��
		RANK_3RD,			//3��
		RANK_4TH,			//4��

		//�v���C���[
		CHICKEN_BLUE,		//�j���g����
		CHICKEN_RED,		//�j���g����
		CHICKEN_GREEN,		//�j���g����
		CHICKEN_YELLOW,		//�j���g����
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);
	//�^�C�g���Ŏg�����\�[�X������
	void InitTitle(void);
	//�Q�[���I�[�o�[�Ŏg�����\�[�X������
	void InitSelect(void);
	//�Q�[���Ŏg�����\�[�X������
	void InitGame(void);
	//�Q�[���N���A�Ŏg�����\�[�X������
	void InitResult(void);

	//�v���C���[���g�����\�[�X
	void ResourcePlayer(void);

	//�X�e�[�W���g�����\�[�X
	void ResourceStage(void);

	//���[�����ʂŎg�����\�[�X
	void ResourceRule(void);

	//�X�R�A���[���Ŏg�����\�[�X
	void ResourceScore(void);

	//�̗̓��[���Ŏg�����\�[�X
	void ResourceLife(void);

	//���j�󐔃��[���Ŏg�����\�[�X
	void ResourceTileBraek(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	Resource Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource*> loadedMap_;

	//

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);

	// �f�X�g���N�^�����l
	~ResourceManager(void) = default;

	// �������[�h
	Resource* _Load(SRC src);

};

