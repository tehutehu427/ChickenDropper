#pragma once

#include<vector>
#include<map>

class EffectManager
{
public:

	//�G�t�F�N�g�̎��
	enum class EFF_TYPE
	{
		START
		,REVIVAL
		,ATTACK
	};

	//�G�t�F�N�g�̌ŗL�̃f�[�^
	struct EFFDATA
	{
		//�G�t�F�N�g�̃��\�[�X�n���h��ID
		int effectResId_;

		//�G�t�F�N�g�̍Đ��n���h��ID
		std::vector<int> effectPlayId_;
	};


	//�G�t�F�N�V�A�̏�����
	void EffectInit(void);

	//�G�t�F�N�V�A�̏I��
	void EffectEnd(void);

	/// <summary>
	/// �G�t�F�N�g�̍Đ�
	/// </summary>
	/// <param name="_effect">�Đ�����G�t�F�N�g�̎��</param>
	/// <param name="_pos">�Đ�������W</param>
	/// <param name="_scale">�Đ����鎞�̑傫��</param>
	/// <param name="_rot">�Đ����鎞�̉�]</param>
	void EffectPlay(EFF_TYPE _effect, VECTOR _pos, VECTOR _scale,VECTOR _rot);

	//�G�t�F�N�g�̒�~
	void EffectStop(EFF_TYPE _effect);

	//�G�t�F�N�g�̍��W�ω�
	void EffectChangePos(EFF_TYPE _effect, VECTOR _pos);

	//�G�t�F�N�g�̑傫���ω�
	void EffectChangeScl(EFF_TYPE _effect, VECTOR _scl);
	
	//�G�t�F�N�g�̉�]�ω�
	void EffectChangeRot(EFF_TYPE _effect, VECTOR _rot);

private:

	//�G�t�F�N�g�f�[�^�̊i�[�̈�
	std::map<EFF_TYPE, EFFDATA> effDatas_;

	//�G�t�F�N�g�̃��[�h
	void EffectLoad(void);
};

