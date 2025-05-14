#pragma once

class Fader
{

public:

	// �t�F�[�h���i�ޑ���
	static constexpr float SPEED_ALPHA = 3.0f;

	//�A���t�@�l�̍ő�
	static constexpr float ALPHA_MAX = 255.0f;

	// ���
	enum class STATE
	{
		NONE,		// �ʏ���
		FADE_OUT,	// ���X�ɈÓ]
		FADE_IN		// ���X�ɖ��]
	};

	// �R���X�g���N�^
	Fader(void);

	// �f�X�g���N�^
	~Fader(void);

	//������
	void Init(void);
	//�X�V
	void Update(void);
	//�`��
	void Draw(void);

	// ��Ԃ̎擾
	const STATE GetState(void);

	// �t�F�[�h�������I�����Ă��邩
	const bool IsEnd(void);

	// �w��t�F�[�h���J�n����
	void SetFade(const STATE state);

private:

	// ���
	STATE state_;

	// �����x
	float alpha_;

	// ���(STATE)��ۂ����܂܏I��������s�����߁A
	// Update->Draw->Update��1�t���[������p
	bool isPreEnd_;

	// �t�F�[�h�����̏I������
	bool isEnd_;

};
