#include<DxLib.h>
#include<sstream>
#include"../Common/Vector2.h"
#include"../Common/Vector2F.h"
#include"NodyUtility.h"

//�����񕪊��̋@�\
std::vector <std::string> NodyUtility::Split(std::string& line, char delimiter)
{
	//���I�z��̖߂�l�i�[�̈�̊m��
	std::vector<std::string> result;

	//�����񑀍�p�̃N���X�ɒu������
	std::istringstream lineStream(line);

	//�f�[�^�̊i�[�̈�
	std::string field;

	//stream�����Ԃɓǂݍ���
	//getline���g��
	//��3�����Ŏw�肵���f���~�^�L���܂ł̃f�[�^��
	//��2�����̕ϐ��Ɋi�[����
	while (getline(lineStream, field, delimiter))
	{
		//���I�z��ɒǉ�����
		result.push_back(field);
	}

	return result;
}

//�l�̌ܓ��p�̊֐�(float)
int NodyUtility::Round(float value)
{
	return static_cast <int>(roundf(value));
}

//�I�[�o�[���[�h�֐�(�����Ɩ߂�l��ύX���Ă���֐�)
//�l�̌ܓ��p�̊֐�(Vector2)
Vector2 NodyUtility::Round(Vector2F value)
{
	Vector2 ret;
	ret.x = static_cast<int>(round(value.x));
	ret.y = static_cast<int>(round(value.y));
	return ret;
}

float NodyUtility::Deg2RadF(const float _deg)
{
	return _deg * DEG2RAD;
}

float NodyUtility::Rad2DegF(const float _rad)
{
	return _rad * RAD2DEG;
}
