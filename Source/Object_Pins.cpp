/*===================================================================================
*    Date : 2023/01/14(�y)
*        Author		: Gakuto.S
*        File		: Object_Pins.cpp
*        Detail		:
===================================================================================*/
#include "Object_Pins.h"

#include "Application.h"
#include "Summary_Objects.h"
using namespace std;
using namespace DirectX;

ObjectPins::ObjectPins(ObjectTable* table)
	:m_table(table)
{
}

void ObjectPins::Init()
{
	// �𓚂��쐬
	random_device seed_gen;
	mt19937 engine(seed_gen());
	
	int index = ObjectTable::k_COL * ObjectTable::k_ROW;

	for (int i = 0; i < ObjectTable::k_ROW; ++i)
	{
		// �s���I�u�W�F�N�g�̐���
		int random = engine() % k_COLORLIST.size();
		m_answerPins[i] = new ObjectPin(k_COLORLIST[random], false);
		m_answerPins[i]->Init();

		// �s���̃|�W�V�����ݒ�
		m_answerPins[i]->SetPosition(m_table->GetPinWholePinSetPosition(index + i));
		m_answerPins[i]->SetScale(k_PIN_SCALE);
	}

	// �J���[�s���̍쐬
	for (int i = 0; i < ObjectTable::k_COLOR_NUM; ++i)
	{
		// �s���I�u�W�F�N�g�̐���
		m_colorPins[i] = new ObjectPin(k_COLORLIST[i]);
		m_colorPins[i]->Init();

		// �s���̃|�W�V�����ݒ�
		m_colorPins[i]->SetPosition(m_table->GetColorPinWholePinSetPosition(i));
		m_colorPins[i]->SetScale(k_PIN_SCALE);
	}
}

void ObjectPins::Uninit()
{
	for (auto& ansPin : m_answerPins)
	{
		if (ansPin.second)
		{
			ansPin.second->Uninit();
			delete ansPin.second;
		}
	}

	for (auto& HBPin : m_hitBrowPins)
	{
		if (HBPin.second)
		{
			HBPin.second->Uninit();
			delete HBPin.second;
		}
	}

	for (auto& colorPin : m_colorPins)
	{
		if (colorPin.second)
		{
			colorPin.second->Uninit();
			delete colorPin.second;
		}
	}

	for (auto& pin : m_pins)
	{
		if (pin.second)
		{
			pin.second->Uninit();
			delete pin.second;
		}
	}
}

void ObjectPins::Draw()
{
	// �𓚃s���̕`��
	for (auto& ansPin : m_answerPins)
	{
		if (ansPin.second)
			ansPin.second->Draw();
	}

	// �q�b�g�A�u���[�̕\������
	for (auto& HBPin : m_hitBrowPins)
	{
		if (HBPin.second)
			HBPin.second->Draw();
	}

	// �J���[�s���̕\������
	for (auto& colorPin : m_colorPins)
	{
		if (colorPin.second)
			colorPin.second->Draw();
	}

	// �񓚃s���̕\������
	for (auto& pin : m_pins)
	{
		if (pin.second)
			pin.second->Draw();
	}
}

void ObjectPins::RegisterPins(int pinWholeIndex, int colorIndex)
{
	// �f�[�^�̉��
	this->UnRegisterPins(pinWholeIndex);

	// �s���I�u�W�F�N�g�̐���
	m_pins[pinWholeIndex] = new ObjectPin(k_COLORLIST[colorIndex]);
	m_pins[pinWholeIndex]->Init();

	// �s���̃|�W�V�����ݒ�
	m_pins[pinWholeIndex]->SetPosition(m_table->GetPinWholePinSetPosition(pinWholeIndex));
	m_pins[pinWholeIndex]->SetScale(k_PIN_SCALE);
}

void ObjectPins::UnRegisterPins(int pinWholeIndex)
{
	// �f�[�^������ꍇ�̓f�[�^�̍폜
	if (m_pins[pinWholeIndex])
	{
		delete m_pins[pinWholeIndex];
		m_pins[pinWholeIndex] = nullptr;
	}
}

bool ObjectPins::GetPinWholeStateOfAColumn(int currentlyColumnIndex)
{
	for (int i = 0; i < ObjectTable::k_ROW; ++i)
	{
		if (!m_pins[ObjectTable::k_ROW * currentlyColumnIndex + i])
			return false;
	}

	// ���4�̃s�����h�����Ă��邩�ǂ���
	return true;
}

bool ObjectPins::CheckHitOrBrow(int currentlyColumnIndex, int& hit, int& brow)
{
	vector<ObjectPinAns::ANSWER> needPinList;
	map<int, ObjectPinAns::ANSWER> answerState;
	vector<int> noHitPinIndexList;

	// �q�b�g�̃`�F�b�N
	for (int i = 0; i < ObjectTable::k_ROW; ++i)
	{
		int pinIndex = ObjectTable::k_ROW * currentlyColumnIndex + i;
		int answerIndex = i;
		
		// �q�b�g�ł����
		ObjectPinAns::ANSWER ans = {};
		if (m_pins[pinIndex]->GetColor() == m_answerPins[answerIndex]->GetColor())
		{
			needPinList.push_back(ObjectPinAns::ANSWER::HIT);		// �K�v�ȃs���Ƃ��Ēǉ�
			answerState[answerIndex] = ObjectPinAns::ANSWER::HIT;	// �񓚂̏�Ԃ��q�b�g�Ƃ��Đݒ�
			hit++;
		}
		else
			noHitPinIndexList.push_back(pinIndex);					// �q�b�g���Ă��Ȃ��f�[�^�����X�g�A�b�v
	}

	// �u���[�̃`�F�b�N
	for (int i = 0; i < noHitPinIndexList.size(); ++i)
	{
		int pinIndex = noHitPinIndexList[i];	// �q�b�g���Ă��Ȃ��s���C���f�b�N�X���擾

		for (int j = 0; j < ObjectTable::k_ROW; ++j)
		{			
			int answerIndex = j;
			if (answerState[answerIndex] != ObjectPinAns::ANSWER::NONE)
				continue;	// �񓚏�Ԃ��q�b�g��u���[�ł���Ύ��𒲂ׂ�

			if (m_pins[pinIndex]->GetColor() == m_answerPins[answerIndex]->GetColor())
			{
				needPinList.push_back(ObjectPinAns::ANSWER::BROW);		// �K�v�ȃs���Ƃ��Ēǉ�
				answerState[answerIndex] = ObjectPinAns::ANSWER::BROW;	// �񓚂̏�Ԃ��u���[�Ƃ��Đݒ�
				brow++;
				break;
			}
		}
	}

	for (int i = 0; i < int(needPinList.size()); ++i)
	{
		// �q�b�g���̓u���[�s���̃I�u�W�F�N�g�𐶐�����
		int index = ObjectTable::k_ROW * currentlyColumnIndex + i;
		m_hitBrowPins[index] = new ObjectPinAns(needPinList[i]);
		m_hitBrowPins[index]->Init();

		// �|�W�V������ݒ肷��
		m_hitBrowPins[index]->SetPosition(m_table->GetHitBrowPinWholePinSetPosition(index));
	}

	// �Ō�̗�Ő������Ȃ������ꍇ
	bool ret = true;
	if (needPinList.size() < ObjectTable::k_ROW)
		ret = false;

	// �S�ăq�b�g�ł��邩���ׂ�
	if (ret)
	{
		for (size_t i = 0; i < needPinList.size(); ++i)
		{
			if (needPinList[i] != ObjectPinAns::ANSWER::HIT)
			{
				ret = false;
				break;
			}
		}
	}

	// �Q�[���I�[�o�[
	if (!ret && currentlyColumnIndex >= ObjectTable::k_COL -1)
		ret = true;

	return ret;
}

bool ObjectPins::SetAnswerColor(int answerRowIndex, int finishColumn)
{
	// �F�̐ݒ�
	m_answerPins[answerRowIndex]->SetUseMaterial(true);

	// �J���[����v
	if (m_answerPins[answerRowIndex]->GetColor() == m_pins[finishColumn * ObjectTable::k_ROW + answerRowIndex]->GetColor())
		return true;
	else
		return false;
}

void ObjectPins::SetCommonAnswer(std::array<int, 4> ans)
{
	for (int i = 0; i < ObjectTable::k_ROW; ++i)
	{
		ObjectPin* work = new ObjectPin(k_COLORLIST[ans[i]], false);
		work->Init();
		work->SetPosition(m_answerPins[i]->GetPosition());
		work->SetScale(m_answerPins[i]->GetScale());
		m_answerPins[i]->Uninit();
		delete m_answerPins[i];

		m_answerPins[i] = work;
	}
}
