#pragma once

#include<map>
#include<string>

class SoundManager
{
public:

	enum class BGM_TYPE
	{
		TITLE,					//�^�C�g��
		SELECT,					//�Z���N�g
		BATTLE_FIRST_HALF,		//�o�g���O��
		BATTLE_SECOND_HALF,		//�o�g���㔼
		RESULT,					//���U���g
	};

	enum class SE_TYPE
	{
		CLICK,		//����
		CANCEL,		//�L�����Z��
		FALL,		//����
		ATTACK,		//�U��
		TILEBREAK,	//���j��
		CURTAIN,	//���䖋
	};

	//����
	static constexpr int MAX_PERCENT = 100;		//�ő偓
	static constexpr int MAX_VOLUME = 255;		//�ő剹��

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
	void BGMLoad(const BGM_TYPE _bgm);

	//SE�̃p�X�̏�����
	void SEInit(void);

	/// <summary>
	/// SE�̃��[�h	
	/// </summary>
	/// <param name="_bgm">SE�̎��</param>
	void SELoad(const SE_TYPE _se);

	/// <summary>
	/// BGM�̍Đ�
	/// </summary>
	/// <param name="_bgm">BGM�̎��</param>
	/// <param name="_playType">�Đ��`��</param>
	/// <param name="_volumePar">����(%�\�L)</param>
	/// <param name="_topPositionFlag">�ŏ�����Đ����n�߂邩 (true:�ŏ�����)</param>
	void PlayBGM(const BGM_TYPE _bgm, const int _playType, const int _volumePar = MAX_PERCENT, const bool _topPositionFlag = true);

	/// <summary>
	/// SE�̍Đ�
	/// </summary>
	/// <param name="_se">SE�̎��</param>
	/// <param name="_playType">�Đ��`��</param>
	/// <param name="_volumePar">����(%�\�L)</param>
	/// <param name="_topPositionFlag">�ŏ�����Đ����n�߂邩 (true:�ŏ�����)</param>
	void PlaySE(const SE_TYPE _se, const int _playType, const int _volumePar = MAX_PERCENT, const bool _topPositionFlag = true);

	/// <summary>
	/// BGM���Đ������ǂ�����Ԃ�
	/// </summary>
	/// <param name="_bgm">BGM�̎��</param>
	/// <returns>true:�Đ���</returns>
	bool CheckBGMPlay(const BGM_TYPE _bgm);

	/// <summary>
	/// SE���Đ������ǂ�����Ԃ�
	/// </summary>
	/// <param name="_se">SE�̎��</param>
	/// <returns>true:�Đ���</returns>
	bool CheckSEPlay(const SE_TYPE _se);

	/// <summary>
	/// BGM���~�߂�
	/// </summary>
	/// <param name="_bgm">�~�߂�BGM</param>
	void StopBGM(const BGM_TYPE _bgm);

	/// <summary>
	/// SE���~�߂�
	/// </summary>
	/// <param name="_se">�~�߂�SE</param>
	void StopSE(const SE_TYPE _se);

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

