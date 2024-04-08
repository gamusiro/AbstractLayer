/*===================================================================================
*    Date : 2023/01/14(�y)
*        Author		: Gakuto.S
*        File		: Object_Pins.h
*        Detail		:
===================================================================================*/
#pragma once
#include "IF_Object.h"
#include "Object_Pin.h"

class IObjectGLTF;
class ObjectTable;
class ObjectPin;

class ObjectPins : public IObject3D
{
private:
	// �J���[�̎���
	std::map<int, ObjectPin::COLOR> k_COLORLIST
	{
		{0, ObjectPin::COLOR::BLUE},
		{1, ObjectPin::COLOR::RED},
		{2, ObjectPin::COLOR::YELLOW},
		{3, ObjectPin::COLOR::GREEN},
		{4, ObjectPin::COLOR::PINK},
		{5, ObjectPin::COLOR::WHITE},
	};

public:
	//**************************************************
	/// \brief �R���X�g���N�^
	/// 
	/// \return �Ȃ�
	//**************************************************
	ObjectPins(ObjectTable* table);

	//**************************************************
	/// \brief ����������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Init() override;

	//**************************************************
	/// \brief �I������
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Uninit() override;

	//**************************************************
	/// \brief �`�揈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void Draw() override;

public:
	//**************************************************
	/// \brief �s���̐ݒ菈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void RegisterPins(int pinWholeIndex, int colorIndex);

	//**************************************************
	/// \brief �s���̐ݒ菈��
	/// 
	/// \return �Ȃ�
	//**************************************************
	void UnRegisterPins(int pinWholeIndex);

	//**************************************************
	/// \brief �m��\���ǂ����`�F�b�N���鏈��
	/// 
	/// \return 4�z�[�����܂��Ă����true
	//**************************************************
	bool GetPinWholeStateOfAColumn(int currentlyColumnIndex);

	//**************************************************
	/// \brief �q�b�g�A�u���[�̃`�F�b�N����
	/// 
	/// \return true �Ȃ�Q�[���I��
	//**************************************************
	bool CheckHitOrBrow(int currentlyColumn, int& hit, int& brow);

	//**************************************************
	/// \brief �𓚃J���[��ݒ肷��
	/// 
	/// \return �Ȃ�
	//**************************************************
	bool SetAnswerColor(int answerRowIndex, int finishColumn);

	//**************************************************
	/// \brief �I�����C���p�A���ʉ𓚂̍쐬����
	/// 
	/// \return �Ȃ�
	//**************************************************
	void SetCommonAnswer(std::array<int, 4> ans);

public:
	static constexpr int k_COLOR_NUM = 6;
	static constexpr DirectX::XMFLOAT3 k_PIN_SCALE = { 0.8f, 0.8f, 0.8f };

private:
	std::map<int, IObjectGLTF*>	m_hitBrowPins;
	std::map<int, ObjectPin*>	m_answerPins;
	std::map<int, ObjectPin*>	m_pins;
	std::map<int, IObjectGLTF*>	m_colorPins;

	ObjectTable* m_table;
};

