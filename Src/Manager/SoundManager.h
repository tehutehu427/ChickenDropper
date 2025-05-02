#pragma once

#include<map>
#include<string>

class SoundManager
{
public:

	enum class BGM_TYPE
	{
		TITLE					//�^�C�g��
		,SELECT					//�Z���N�g
		,BATTLE_FIRST_HALF		//�o�g���O��
		,BATTLE_SECOND_HALF		//�o�g���㔼
		,RESULT					//���U���g
		,MAX
	};

	enum class SE_TYPE
	{
		CLICK		//����
		,CANCEL		//�L�����Z��
		,FALL		//����
		,ATTACK		//�U��
		,TILEBREAK	//���j��
		,CURTAIN	//���䖋
		,MAX
	};

	//�����o�[�֐�
	//-------------------------

	//�T�E���h�̏�����
	void SoundInit(void);

	//BGM�̃p�X�̏�����
	void BGMInit(void);

	/// <summary>
	/// BGM�̃��[�h
	/// </summary>
	/// <param name="_bgm">BGM�̎��</param>
	void BGMLoad(BGM_TYPE _bgm);

	//SE�̃p�X�̏�����
	void SEInit(void);

	/// <summary>
	/// SE�̃��[�h	
	/// </summary>
	/// <param name="_bgm">SE�̎��</param>
	void SELoad(SE_TYPE _se);

	/// <summary>
	/// BGM�̍Đ�
	/// </summary>
	/// <param name="_bgm">BGM�̎��</param>
	/// <param name="_playType">�Đ��`��</param>
	/// <param name="_volumePar">����(%�\�L)</param>
	/// <param name="_topPositionFlag">�ŏ�����Đ����n�߂邩 (true:�ŏ�����)</param>
	void PlayBGM(BGM_TYPE _bgm, int _playType, int _volumePar = 100, bool _topPositionFlag = true);

	/// <summary>
	/// SE�̍Đ�
	/// </summary>
	/// <param name="_se">SE�̎��</param>
	/// <param name="_playType">�Đ��`��</param>
	/// <param name="_volumePar">����(%�\�L)</param>
	/// <param name="_topPositionFlag">�ŏ�����Đ����n�߂邩 (true:�ŏ�����)</param>
	void PlaySE(SE_TYPE _se, int _playType, int _volumePar = 100, bool _topPositionFlag = true);

	/// <summary>
	/// BGM���Đ������ǂ�����Ԃ�
	/// </summary>
	/// <param name="_bgm">BGM�̎��</param>
	/// <returns>true:�Đ���</returns>
	bool CheckBGMPlay(BGM_TYPE _bgm);

	/// <summary>
	/// SE���Đ������ǂ�����Ԃ�
	/// </summary>
	/// <param name="_se">SE�̎��</param>
	/// <returns>true:�Đ���</returns>
	bool CheckSEPlay(SE_TYPE _se);

	/// <summary>
	/// BGM���~�߂�
	/// </summary>
	/// <param name="_bgm">�~�߂�BGM</param>
	void StopBGM(BGM_TYPE _bgm);

	/// <summary>
	/// SE���~�߂�
	/// </summary>
	/// <param name="_se">�~�߂�SE</param>
	void StopSE(SE_TYPE _se);

	//�T�E���h�̉��
	void SoundRelease(void);

private:

	//�����o�[�ϐ�
	//--------------------------

	//�T�E���h�̃p�X
	std::map<BGM_TYPE, std::string> bgmPass_;	//BGM�̃p�X

	std::map<SE_TYPE, std::string> sePass_;		//SE�̃p�X

	//�T�E���h�̃n���h��
	std::map<BGM_TYPE,int> bgm_;	//BGM�n���h��

	std::map<SE_TYPE, int> se_;		//SE�n���h��
};

