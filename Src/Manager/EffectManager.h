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
	};

	//�G�t�F�N�g�̌ŗL�̃f�[�^
	struct EFFDATA
	{
		//�G�t�F�N�g�̃��\�[�X�n���h��ID
		int effectResId_ = -1;

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
	void EffectPlay(const EFF_TYPE _effect, const VECTOR _pos, const VECTOR _scale, const VECTOR _rot);

	/// <summary>
	/// �G�t�F�N�g�̒�~
	/// </summary>
	/// <param name="_effect">�~�߂����G�t�F�N�g</param>
	void EffectStop(const EFF_TYPE _effect);

	/// <summary>
	/// �G�t�F�N�g�̍��W�ω�
	/// </summary>
	/// <param name="_effect">�G�t�F�N�g</param>
	/// <param name="_pos">�ω���̍��W</param>
	void EffectChangePos(const EFF_TYPE _effect, const VECTOR _pos);

	/// <summary>
	/// �G�t�F�N�g�̑傫���ω�
	/// </summary>
	/// <param name="_effect">�G�t�F�N�g</param>
	/// <param name="_scl">�ω���̑傫��</param>
	void EffectChangeScl(const EFF_TYPE _effect, const VECTOR _scl);
	
	/// <summary>
	/// �G�t�F�N�g�̉�]�ω�
	/// </summary>
	/// <param name="_effect">�G�t�F�N�g</param>
	/// <param name="_rot">�ω���̉�]</param>
	void EffectChangeRot(const EFF_TYPE _effect, const VECTOR _rot);

private:

	//�G�t�F�N�g�f�[�^�̊i�[�̈�
	std::map<EFF_TYPE, EFFDATA> effDatas_;

	//�G�t�F�N�g�̃��[�h
	void EffectLoad(void);
};

