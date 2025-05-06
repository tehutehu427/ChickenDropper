#pragma once

#include<Dxlib.h>
#include<string>
#include<vector>

//�N���X�̑O���錾
class Vector2;
class Vector2F;

class Utility
{
public:
	//�񋓌^
	//---------------------
	//����
	enum class DIR_2D	//2D�p�̕���
	{
		UP			//�����
		,RIGHT		//�E����
		,DOWN		//������
		,LEFT		//������
		,MAX
	};

	enum class DIR_3D	//3D�p�̕���
	{
		FRONT		//�O����
		, RIGHT		//�E����
		, BACK		//������
		, LEFT		//������
		, MAX
	};

	//�萔
	//----------------------------------------------------
	static constexpr float DEG2RAD = DX_PI_F / 180.0f;		//�f�O���[�̃��W�A���ϊ��p
	static constexpr float RAD2DEG = 180.0f / DX_PI_F;		//���W�A���̃f�O���[�ϊ��p

	static constexpr VECTOR VECTOR_ZERO = { 0.0f,0.0f,0.0f };		//VECTOR�^�̂O�̏������p
	static constexpr VECTOR VECTOR_ONE = { 1.0f,1.0f,1.0f };		//VECTOR�^�̂P�̏������p

	//�f���^�^�C��
	static constexpr float DEFAULT_FPS = 60.0f;					//FPS
	static constexpr float DELTA_TIME = 1.0f / DEFAULT_FPS;		//�f���^�^�C��

	//�F
	static constexpr int COLOR_RED = 0xff0000;		//��
	static constexpr int COLOR_GREEN = 0x00ff00;	//��
	static constexpr int COLOR_BLUE = 0x0000ff;		//��
	static constexpr int COLOR_YELLOW = 0xffff00;	//��
	static constexpr int COLOR_WHITE = 0xffffff;	//��
	static constexpr int COLOR_BLACK = 0x000000;	//��
	static constexpr int COLOR_GRAY = 0xaaaaaa;		//�D

	//�֐�
	//----------------------------------------------------

	//�����񕪊��̋@�\
	static std::vector <std::string> Split(const std::string& line, const char delimiter);

	//�l�̌ܓ��p�̊֐�(float)
	static const int Round(const float value);

	//�I�[�o�[���[�h�֐�
	static const Vector2 Round(const Vector2F value);

	//Degree��Radian�ϊ�
	static const float Deg2RadF(const float _deg);

	//Radian��Degree�ϊ�
	static const float Rad2DegF(const float _rad);
};