#pragma once
#include "stdafx.h"
#include <algorithm>
#include "afxdialogex.h"
#include "Command.h"
#include "menber.h"

//�������
class AddCommand : public Command
{
private:

	CListBox* m_AllListBox;
	CListBox* m_CurListBox;
	CString m_name;

	bool m_Undo;//�Ƿ񳷻�
	stMember* m_NewMember;//��ӵĳ�Ա ������صĻ� Ҫ�Լ�delete��

	vector<stMember*>* m_AllMenberVect;
	vector<stMember*>* m_CurMenberVect;
	vector<stMember*>* m_DoveMenberVect;
	vector<stMember*> m_AllMenberVect_Undo_Temp;
	vector<stMember*> m_CurMenberVect_Undo_Temp;
	vector<stMember*> m_DoveMenberVect_Undo_Temp;
	vector<stMember*> m_AllMenberVect_Redo_Temp;
	vector<stMember*> m_CurMenberVect_Redo_Temp;
	vector<stMember*> m_DoveMenberVect_Redo_Temp;
public:

	AddCommand(CListBox* all, CListBox* cur,
			vector<stMember*>* allVect,
			vector<stMember*>* curVect,
			vector<stMember*>* doveVect,
			CString name)
		:
		m_AllMenberVect(allVect),
		m_CurMenberVect(curVect),
		m_DoveMenberVect(doveVect),
		m_AllListBox(all),
		m_CurListBox(cur),
		m_name(name),
		m_NewMember(NULL),
		m_Undo(false)
	{
		m_AllMenberVect_Undo_Temp = *m_AllMenberVect;
		m_CurMenberVect_Undo_Temp = *m_CurMenberVect;
		m_DoveMenberVect_Undo_Temp = *m_DoveMenberVect;
	}

	virtual	~AddCommand()
	{
		if (m_Undo)
			delete m_NewMember;
	}

	//�ж�stMember�Ƿ���vect����
	stMember* IsMemberInVect(stMember m, vector<stMember*>& ls)
	{
		for (vector<stMember*>::iterator it = ls.begin(); it != ls.end(); ++it)
		{
			if (m.name == (*it)->name)
				return *it;
		}

		return NULL;
	}

	//�ж�stMember�Ƿ���vect����
	int IsMemberInVect_index(stMember m, vector<stMember*>& ls)
	{
		for (int i = 0; i < ls.size(); ++i)
		{
			if (m.name == ls[i]->name)
				return i;
		}

		return -1;
	}

	//ִ������
	virtual void execute()
	{
		// TODO: �ڴ���ӿؼ�֪ͨ����������
		stMember* m = new stMember;
		m->money = 0;
		m->name = m_name;
		
		if (m->name == _TEXT(""))
		{
			delete m;
			return;
		}

		//�жϵ�ǰ��Ա�Ƿ���ȫ������б���
		stMember* p = IsMemberInVect(*m, *m_AllMenberVect);

		//û���ظ������
		if (NULL == p)
		{
			m_CurListBox->AddString(m->name);
			m_AllListBox->AddString(m->name + _TEXT("  +0"));
			m_CurMenberVect->push_back(m);
			m_AllMenberVect->push_back(m);
		}
		else
		{
			if (NULL == IsMemberInVect(*p, *m_CurMenberVect))
			{
				m_CurMenberVect->push_back(p);
				m_CurListBox->AddString(p->name);
				for (int i = 0; i < m_DoveMenberVect->size(); ++i)
				{
					if (m->name == (*m_DoveMenberVect)[i]->name)
					{
						m_DoveMenberVect->erase(m_DoveMenberVect->begin() + i);
						break;
					}
				}
			}
			else
			{
				MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd, _T("�ظ����"), _T("��ʾ"), MB_OK);
			}
			delete m;
		}

		m_AllMenberVect_Redo_Temp = *m_AllMenberVect;
		m_CurMenberVect_Redo_Temp = *m_CurMenberVect;
		m_DoveMenberVect_Redo_Temp = *m_DoveMenberVect;
		m_NewMember = m;//��¼��ǰ��Ա
	}

	//����
	virtual void redo()
	{
		m_Undo = false;
		*m_AllMenberVect = m_AllMenberVect_Redo_Temp;
		*m_CurMenberVect = m_CurMenberVect_Redo_Temp;
		*m_DoveMenberVect = m_DoveMenberVect_Redo_Temp;

		m_CurListBox->ResetContent();
		m_AllListBox->ResetContent();
		for (int i = 0; i < (int)m_CurMenberVect->size(); ++i)
			m_CurListBox->AddString((*m_CurMenberVect)[i]->name);
		for (int i = 0; i < (int)m_AllMenberVect->size(); ++i)
			m_AllListBox->AddString((*m_AllMenberVect)[i]->name + _T("  +0"));
	}

	//����
	virtual void undo()
	{
		m_Undo = true;
		*m_AllMenberVect = m_AllMenberVect_Undo_Temp;
		*m_CurMenberVect = m_CurMenberVect_Undo_Temp;
		*m_DoveMenberVect = m_DoveMenberVect_Undo_Temp;

		m_CurListBox->ResetContent();
		m_AllListBox->ResetContent();
		for (int i = 0; i < (int)m_CurMenberVect->size(); ++i)
			m_CurListBox->AddString((*m_CurMenberVect)[i]->name);
		for (int i = 0; i < (int)m_AllMenberVect->size(); ++i)
			m_AllListBox->AddString((*m_AllMenberVect)[i]->name + _T("  +0"));
	}
};

//ɾ������
class EraseCommand : public Command
{
private:

	int m_EraseIndex;
	CListBox* m_CurListBox;
	vector<stMember*>* m_CurMenberVect;
	vector<stMember*>* m_DoveMenberVect;
	vector<stMember*> m_CurMenberVect_Undo_Temp;
	vector<stMember*> m_DoveMenberVect_Undo_Temp;
	vector<stMember*> m_CurMenberVect_Redo_Temp;
	vector<stMember*> m_DoveMenberVect_Redo_Temp;
public:

	EraseCommand(CListBox* cur,
				vector<stMember*>* curVect,
				vector<stMember*>* doveVect,
				int eraseIdx)
		:
		m_CurMenberVect(curVect),
		m_DoveMenberVect(doveVect),
		m_CurListBox(cur),
		m_EraseIndex(eraseIdx)
	{
		m_CurMenberVect_Undo_Temp = *m_CurMenberVect;
		m_DoveMenberVect_Undo_Temp = *m_DoveMenberVect;
	}

	virtual	~EraseCommand()
	{

	}

	//ִ������
	virtual void execute()
	{
		CString s;
		m_CurListBox->GetText(m_EraseIndex, s);
		m_CurListBox->DeleteString(m_EraseIndex);

		for (int i = 0; i < m_CurMenberVect->size(); ++i)
		{
			if ((*m_CurMenberVect)[i]->name == s)
			{
				m_DoveMenberVect->push_back((*m_CurMenberVect)[i]);
				m_CurMenberVect->erase(m_CurMenberVect->begin() + i);
				break;
			}
		}

		m_CurMenberVect_Redo_Temp = *m_CurMenberVect;
		m_DoveMenberVect_Redo_Temp = *m_DoveMenberVect;
	}

	//����
	virtual void redo()
	{
		*m_CurMenberVect = m_CurMenberVect_Redo_Temp;
		*m_DoveMenberVect = m_DoveMenberVect_Redo_Temp;

		m_CurListBox->ResetContent();
		for (int i = 0; i < (int)m_CurMenberVect->size(); ++i)
			m_CurListBox->AddString((*m_CurMenberVect)[i]->name);
	}

	//����
	virtual void undo()
	{
		*m_CurMenberVect = m_CurMenberVect_Undo_Temp;
		*m_DoveMenberVect = m_DoveMenberVect_Undo_Temp;

		m_CurListBox->ResetContent();
		for (int i = 0; i < (int)m_CurMenberVect->size(); ++i)
			m_CurListBox->AddString((*m_CurMenberVect)[i]->name);
	}
};

//��������
class GroupingCommand : public Command
{
private:

	CListBox* m_RedListBox;
	CListBox* m_BuleListBox;

	vector<stData>* m_Data;
	vector<stMember*>* m_CurMenberVect;
	vector<stMember*>* m_RedMenberVect;
	vector<stMember*>* m_BuleMenberVect;

	vector<stMember*> m_CurMenberVect_Undo_Temp;
	vector<stMember*> m_BuleMenberVect_Undo_Temp;
	vector<stMember*> m_RedMenberVect_Undo_Temp;
	vector<stData>   m_Data_Undo_Temp;

	vector<stMember*> m_CurMenberVect_Redo_Temp;
	vector<stMember*> m_BuleMenberVect_Redo_Temp;
	vector<stMember*> m_RedMenberVect_Redo_Temp;
	vector<stData>   m_Data_Redo_Temp;

public:

	GroupingCommand(CListBox* redListBox,
					CListBox* BuleListBox,
					vector<stMember*>* curVect,
					vector<stMember*>* redVect,
					vector<stMember*>* buleVect,
					vector<stData>* data)
		:
		m_CurMenberVect(curVect),
		m_RedMenberVect(redVect),
		m_BuleMenberVect(buleVect),
		m_RedListBox(redListBox),
		m_BuleListBox(BuleListBox),
		m_Data(data)
	{
		m_CurMenberVect_Undo_Temp = *m_CurMenberVect;
		m_BuleMenberVect_Undo_Temp = *m_BuleMenberVect;
		m_RedMenberVect_Undo_Temp = *m_RedMenberVect;
		m_Data_Undo_Temp = *m_Data;
	}

	virtual	~GroupingCommand()
	{

	}

	//ִ������
	virtual void execute()
	{
		//����
		vector<stMember*> temp = *m_CurMenberVect;
		m_RedListBox->ResetContent();
		m_BuleListBox->ResetContent();

		m_RedMenberVect->clear();
		m_BuleMenberVect->clear();

		bool flag = false;

		while (temp.size() != 0)
		{
			int rd = rand() % temp.size();

			if (flag)
			{
				m_RedMenberVect->push_back(temp[rd]);
				m_RedListBox->AddString(temp[rd]->name);
				flag = false;
			}
			else
			{
				m_BuleMenberVect->push_back(temp[rd]);
				m_BuleListBox->AddString(temp[rd]->name);
				flag = true;
			}
			temp.erase(temp.begin() + rd);
		}

		CTime time = CTime::GetCurrentTime(); ///����CTime����  
		int m_nYear = time.GetYear(); ///��  
		int m_nMonth = time.GetMonth(); ///��  
		int m_nDay = time.GetDay(); ///��  
		int m_nHour = time.GetHour(); ///Сʱ  
		int m_nMinute = time.GetMinute(); ///����  
		int m_nSecond = time.GetSecond(); ///��  

		char buf[64];
		sprintf_s(buf, "%d��%d��%d��%dʱ%d��%d��", m_nYear, m_nMonth, m_nDay, m_nHour, m_nMinute, m_nSecond);

		stData data;
		data.fenZutime = buf;
		data.redLose = 2;
		m_Data->push_back(data);

		//��¼��ǰ��Ϣ
		m_CurMenberVect_Redo_Temp = *m_CurMenberVect;
		m_RedMenberVect_Redo_Temp = *m_RedMenberVect;
		m_BuleMenberVect_Redo_Temp = *m_BuleMenberVect;
		m_Data_Redo_Temp = *m_Data;
	}

	//����
	virtual void redo()
	{
		*m_CurMenberVect = m_CurMenberVect_Redo_Temp;
		*m_RedMenberVect = m_RedMenberVect_Redo_Temp;
		*m_BuleMenberVect = m_BuleMenberVect_Redo_Temp;
		*m_Data = m_Data_Redo_Temp;

		m_BuleListBox->ResetContent();
		m_RedListBox->ResetContent();

		for (auto it = m_RedMenberVect->begin(); it != m_RedMenberVect->end(); ++it)
			m_RedListBox->AddString((*it)->name);

		for (auto it = m_BuleMenberVect->begin(); it != m_BuleMenberVect->end(); ++it)
			m_BuleListBox->AddString((*it)->name);
	}

	//����
	virtual void undo()
	{
		*m_CurMenberVect = m_CurMenberVect_Undo_Temp;
		*m_RedMenberVect = m_RedMenberVect_Undo_Temp;
		*m_BuleMenberVect = m_BuleMenberVect_Undo_Temp;
		*m_Data = m_Data_Undo_Temp;

		m_BuleListBox->ResetContent();
		m_RedListBox->ResetContent();

		for (auto it = m_RedMenberVect->begin(); it != m_RedMenberVect->end(); ++it)
			m_RedListBox->AddString((*it)->name);

		for (auto it = m_BuleMenberVect->begin(); it != m_BuleMenberVect->end(); ++it)
			m_BuleListBox->AddString((*it)->name);
	}
};

//ʤ������
class WinCommand : public Command
{
private:

	int m_Sum;//��ע
	bool m_BuleWin;//ʤ������

	CListBox* m_RedListBox;
	CListBox* m_BuleListBox;
	CListBox* m_HistoryListBox;
	CListBox* m_AllListBox;

	vector<stData>* m_Data;
	vector<stMember*>* m_AllMenberVect;
	vector<stMember*>* m_RedMenberVect;
	vector<stMember*>* m_BuleMenberVect;
	vector<CString>* m_PaySchemeString;
	
	vector<CString>  m_PaySchemeString_Undo_Temp;
	vector<stMember> m_AllMenberVect_Undo_Temp;
	vector<stMember*> m_BuleMenberVect_Undo_Temp;
	vector<stMember*> m_RedMenberVect_Undo_Temp;
	vector<stData>    m_Data_Undo_Temp;

	vector<CString>  m_PaySchemeString_Redo_Temp;
	vector<stMember> m_AllMenberVect_Redo_Temp;
	vector<stMember*> m_BuleMenberVect_Redo_Temp;
	vector<stMember*> m_RedMenberVect_Redo_Temp;
	vector<stData>    m_Data_Redo_Temp;

public:

	WinCommand(CListBox* redListBox,
		CListBox* BuleListBox,
		CListBox* allListBox,
		CListBox* HistoryListBox,
		vector<stMember*>* allVect,
		vector<stMember*>* redVect,
		vector<stMember*>* buleVect,
		vector<CString>* payScheme,
		vector<stData>* data,
		int sum,
		bool redWin)
		:
		m_AllMenberVect(allVect),
		m_RedMenberVect(redVect),
		m_BuleMenberVect(buleVect),
		m_RedListBox(redListBox),
		m_BuleListBox(BuleListBox),
		m_AllListBox(allListBox),
		m_HistoryListBox(HistoryListBox),
		m_Data(data),
		m_Sum(sum),
		m_BuleWin(redWin),
		m_PaySchemeString(payScheme)
	{

		for (int i = 0; i < (int)m_AllMenberVect->size(); ++i)
		{
			m_AllMenberVect_Undo_Temp.push_back(*(*m_AllMenberVect)[i]);
		}

		m_BuleMenberVect_Undo_Temp = *m_BuleMenberVect;
		m_RedMenberVect_Undo_Temp = *m_RedMenberVect;
		m_Data_Undo_Temp = *m_Data;
		m_PaySchemeString_Undo_Temp = *m_PaySchemeString;
	}

	virtual	~WinCommand()
	{

	}

	//���֧������
	void PayScheme()
	{
		m_PaySchemeString->clear();
		char buf[32];

		//��ӮǮ�ĺ���Ǯ�ķֿ�
		vector<stMember> lose, vectory;
		for (int i = 0; i < m_AllMenberVect->size(); ++i)
		{
			if ((*m_AllMenberVect)[i]->money < 0)
				lose.push_back(*(*m_AllMenberVect)[i]);
			else if ((*m_AllMenberVect)[i]->money > 0)
				vectory.push_back(*(*m_AllMenberVect)[i]);
		}

		vector<stMember>::iterator loseIt = lose.begin();
		while (loseIt != lose.end())
		{
			bool flag = true;
			for (vector<stMember>::iterator it = vectory.begin(); it != vectory.end();)
			{
				if (-((*loseIt).money) == (*it).money)
				{
					_itoa_s((*it).money * m_Sum, buf, 10);
					CString s = (*loseIt).name + _TEXT("  ��  ") + (*it).name + _TEXT("  ") + buf;
					m_PaySchemeString->push_back(s);

					loseIt = lose.erase(loseIt);
					it = vectory.erase(it);
					flag = false;
					break;
				}
				else
					++it;

			}

			if (flag)
				loseIt++;
		}

		loseIt = lose.begin();
		while (loseIt != lose.end())
		{
			bool flag = true;
			for (vector<stMember>::iterator it = vectory.begin(); it != vectory.end();)
			{
				if (-((*loseIt).money) == (*it).money)
				{
					_itoa_s((*it).money * m_Sum, buf, 10);
					CString s = (*loseIt).name + _TEXT("  ��  ") + (*it).name + _TEXT("  ") + buf;
					m_PaySchemeString->push_back(s);

					loseIt = lose.erase(loseIt);
					it = vectory.erase(it);
					flag = false;
					break;
				}
				else if (-((*loseIt).money) < (*it).money)
				{
					_itoa_s(-(*loseIt).money * m_Sum, buf, 10);
					CString s = (*loseIt).name + _TEXT("  ��  ") + (*it).name + _TEXT("  ") + buf;
					m_PaySchemeString->push_back(s);

					(*it).money += (*loseIt).money;
					loseIt = lose.erase(loseIt);
					flag = false;

					if ((*it).money <= 0)
						it = vectory.erase(it);
					break;
				}
				else
				{
					_itoa_s((*it).money * m_Sum, buf, 10);
					CString s = (*loseIt).name + _TEXT("  ��  ") + (*it).name + _TEXT("  ") + buf;
					m_PaySchemeString->push_back(s);

					((*loseIt).money) += (*it).money;
					it = vectory.erase(it);

					if (((*loseIt).money) >= 0)
					{
						loseIt = lose.erase(loseIt);
						flag = false;
						break;
					}
				}
			}

			if (flag)
				loseIt++;
		}

		for (auto it = lose.begin(); it != lose.end(); ++it)
		{
			_itoa_s(abs((*it).money * m_Sum), buf, 10);
			CString s = (*it).name + _TEXT("  ��  ") + _T("[???]") + _TEXT("  ") + buf;
			m_PaySchemeString->push_back(s);
		}

		for (auto it = vectory.begin(); it != vectory.end(); ++it)
		{
			_itoa_s(abs((*it).money * m_Sum), buf, 10);
			CString s = _T("[???]  ��  ") + (*it).name + +_TEXT("  ") + buf;
			m_PaySchemeString->push_back(s);
		}

		m_HistoryListBox->ResetContent();

		for (int i = 0; i < m_PaySchemeString->size(); ++i)
			m_HistoryListBox->InsertString(0, (*m_PaySchemeString)[i]);
	}

	//ִ������
	virtual void execute()
	{
		for (int i = 0; i < (int)m_BuleMenberVect->size(); ++i)
			(*m_BuleMenberVect)[i]->money += (m_BuleWin ? -1 : 1);

		for (int i = 0; i < (int)m_RedMenberVect->size(); ++i)
			(*m_RedMenberVect)[i]->money += (m_BuleWin ? 1 : -1);

		m_RedListBox->ResetContent();
		m_BuleListBox->ResetContent();
		m_AllListBox->ResetContent();

		for (int i = 0; i < (int)m_AllMenberVect->size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = (*m_AllMenberVect)[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s((*m_AllMenberVect)[i]->money * m_Sum, buf, 10);
			m_AllListBox->InsertString(0, (*m_AllMenberVect)[i]->name + _TEXT("  ") + s + CString(buf));
		}

		PayScheme();
		
		if (m_Data->size() > 0)
		{
			CTime time = CTime::GetCurrentTime(); ///����CTime����  
			int m_nYear = time.GetYear(); ///��  
			int m_nMonth = time.GetMonth(); ///��  
			int m_nDay = time.GetDay(); ///��  
			int m_nHour = time.GetHour(); ///Сʱ  
			int m_nMinute = time.GetMinute(); ///����  
			int m_nSecond = time.GetSecond(); ///��  

			char buf[64];
			sprintf_s(buf, "%d��%d��%d��%dʱ%d��%d��", m_nYear, m_nMonth, m_nDay, m_nHour, m_nMinute, m_nSecond);
			(*m_Data)[m_Data->size() - 1].vectoryTmie = buf;
			(*m_Data)[m_Data->size() - 1].redLose = 1;

			for (int i = 0; i < m_RedMenberVect->size(); ++i)
				(*m_Data)[m_Data->size() - 1].red.push_back(*(*m_RedMenberVect)[i]);

			for (int i = 0; i < m_BuleMenberVect->size(); ++i)
				(*m_Data)[m_Data->size() - 1].bule.push_back(*(*m_BuleMenberVect)[i]);
		}

		m_BuleMenberVect->clear();
		m_RedMenberVect->clear();

		//������Ϣ ��������
		m_AllMenberVect_Redo_Temp.clear();
		for (int i = 0; i < (int)m_AllMenberVect->size(); ++i)
		{
			m_AllMenberVect_Redo_Temp.push_back(*(*m_AllMenberVect)[i]);
		}

		m_RedMenberVect_Redo_Temp = *m_RedMenberVect;
		m_BuleMenberVect_Redo_Temp = *m_BuleMenberVect;
		m_PaySchemeString_Redo_Temp = *m_PaySchemeString;
		m_Data_Redo_Temp = *m_Data;
	}

	//����
	virtual void redo()
	{
		for (int i = 0; i < (int)m_AllMenberVect_Redo_Temp.size(); ++i)
		{
			(*m_AllMenberVect)[i]->money = m_AllMenberVect_Redo_Temp[i].money;
			(*m_AllMenberVect)[i]->name = m_AllMenberVect_Redo_Temp[i].name;
		}

		*m_RedMenberVect = m_RedMenberVect_Redo_Temp;
		*m_BuleMenberVect = m_BuleMenberVect_Redo_Temp;
		*m_Data = m_Data_Redo_Temp;
		*m_PaySchemeString = m_PaySchemeString_Redo_Temp;

		m_BuleListBox->ResetContent();
		m_RedListBox->ResetContent();
		m_HistoryListBox->ResetContent();
		m_AllListBox->ResetContent();

		//���֧������
		PayScheme();

		for (auto it = m_RedMenberVect->begin(); it != m_RedMenberVect->end(); ++it)
			m_RedListBox->AddString((*it)->name);

		for (auto it = m_BuleMenberVect->begin(); it != m_BuleMenberVect->end(); ++it)
			m_BuleListBox->AddString((*it)->name);

		for (int i = 0; i < (int)m_AllMenberVect->size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = (*m_AllMenberVect)[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s((*m_AllMenberVect)[i]->money * m_Sum, buf, 10);
			m_AllListBox->InsertString(0, (*m_AllMenberVect)[i]->name + _TEXT("  ") + s + CString(buf));
		}
	}

	//����
	virtual void undo()
	{
		for (int i = 0; i < (int)m_AllMenberVect_Undo_Temp.size(); ++i)
		{
			(*m_AllMenberVect)[i]->money = m_AllMenberVect_Undo_Temp[i].money;
			(*m_AllMenberVect)[i]->name = m_AllMenberVect_Undo_Temp[i].name;
		}

		*m_RedMenberVect = m_RedMenberVect_Undo_Temp;
		*m_BuleMenberVect = m_BuleMenberVect_Undo_Temp;
		*m_Data = m_Data_Undo_Temp;
		*m_PaySchemeString = m_PaySchemeString_Undo_Temp;

		m_BuleListBox->ResetContent();
		m_RedListBox->ResetContent();
		m_HistoryListBox->ResetContent();
		m_AllListBox->ResetContent();

		//���֧������
		PayScheme();

		for (auto it = m_RedMenberVect->begin(); it != m_RedMenberVect->end(); ++it)
			m_RedListBox->AddString((*it)->name);

		for (auto it = m_BuleMenberVect->begin(); it != m_BuleMenberVect->end(); ++it)
			m_BuleListBox->AddString((*it)->name);

		for (int i = 0; i < (int)m_AllMenberVect->size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = (*m_AllMenberVect)[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s((*m_AllMenberVect)[i]->money * m_Sum, buf, 10);
			m_AllListBox->InsertString(0, (*m_AllMenberVect)[i]->name + _TEXT("  ") + s + CString(buf));
		}
	}
};

//ʤ������
class ChangedMoneyCommand : public Command
{
private:

	int* m_Sum;//��ע
	int m_TempSum_Undo;
	int m_TempSum_Redo;

	CListBox* m_HistoryListBox;
	CListBox* m_AllListBox;
	CEdit* m_Edit;

	vector<stMember*>* m_AllMenberVect;
	vector<CString>* m_PaySchemeString;

	vector<CString>  m_PaySchemeString_Undo_Temp;
	vector<stMember> m_AllMenberVect_Undo_Temp;

	vector<CString>  m_PaySchemeString_Redo_Temp;
	vector<stMember> m_AllMenberVect_Redo_Temp;

public:

	ChangedMoneyCommand(CListBox* allListBox,
					CListBox* HistoryListBox,
					CEdit* edit,
					vector<stMember*>* allVect,
					vector<CString>* payScheme,
					int* sum)
		:
		m_Edit(edit),
		m_AllMenberVect(allVect),
		m_AllListBox(allListBox),
		m_HistoryListBox(HistoryListBox),
		m_Sum(sum),
		m_PaySchemeString(payScheme),
		m_TempSum_Undo(*sum)
	{

		for (int i = 0; i < (int)m_AllMenberVect->size(); ++i)
		{
			m_AllMenberVect_Undo_Temp.push_back(*(*m_AllMenberVect)[i]);
		}

		m_PaySchemeString_Undo_Temp = *m_PaySchemeString;
	}

	virtual	~ChangedMoneyCommand()
	{

	}

	//���֧������
	void PayScheme()
	{
		m_PaySchemeString->clear();
		char buf[32];

		//��ӮǮ�ĺ���Ǯ�ķֿ�
		vector<stMember> lose, vectory;
		for (int i = 0; i < m_AllMenberVect->size(); ++i)
		{
			if ((*m_AllMenberVect)[i]->money < 0)
				lose.push_back(*(*m_AllMenberVect)[i]);
			else if ((*m_AllMenberVect)[i]->money > 0)
				vectory.push_back(*(*m_AllMenberVect)[i]);
		}

		vector<stMember>::iterator loseIt = lose.begin();
		while (loseIt != lose.end())
		{
			bool flag = true;
			for (vector<stMember>::iterator it = vectory.begin(); it != vectory.end();)
			{
				if (-((*loseIt).money) == (*it).money)
				{
					_itoa_s((*it).money * (*m_Sum), buf, 10);
					CString s = (*loseIt).name + _TEXT("  ��  ") + (*it).name + _TEXT("  ") + buf;
					m_PaySchemeString->push_back(s);

					loseIt = lose.erase(loseIt);
					it = vectory.erase(it);
					flag = false;
					break;
				}
				else
					++it;

			}

			if (flag)
				loseIt++;
		}

		loseIt = lose.begin();
		while (loseIt != lose.end())
		{
			bool flag = true;
			for (vector<stMember>::iterator it = vectory.begin(); it != vectory.end();)
			{
				if (-((*loseIt).money) == (*it).money)
				{
					_itoa_s((*it).money * (*m_Sum), buf, 10);
					CString s = (*loseIt).name + _TEXT("  ��  ") + (*it).name + _TEXT("  ") + buf;
					m_PaySchemeString->push_back(s);

					loseIt = lose.erase(loseIt);
					it = vectory.erase(it);
					flag = false;
					break;
				}
				else if (-((*loseIt).money) < (*it).money)
				{
					_itoa_s(-(*loseIt).money * (*m_Sum), buf, 10);
					CString s = (*loseIt).name + _TEXT("  ��  ") + (*it).name + _TEXT("  ") + buf;
					m_PaySchemeString->push_back(s);

					(*it).money += (*loseIt).money;
					loseIt = lose.erase(loseIt);
					flag = false;

					if ((*it).money <= 0)
						it = vectory.erase(it);
					break;
				}
				else
				{
					_itoa_s((*it).money * (*m_Sum), buf, 10);
					CString s = (*loseIt).name + _TEXT("  ��  ") + (*it).name + _TEXT("  ") + buf;
					m_PaySchemeString->push_back(s);

					((*loseIt).money) += (*it).money;
					it = vectory.erase(it);

					if (((*loseIt).money) >= 0)
					{
						loseIt = lose.erase(loseIt);
						flag = false;
						break;
					}
				}
			}

			if (flag)
				loseIt++;
		}

		for (auto it = lose.begin(); it != lose.end(); ++it)
		{
			_itoa_s(abs((*it).money * (*m_Sum)), buf, 10);
			CString s = (*it).name + _TEXT("  ��  ") + _T("[???]") + _TEXT("  ") + buf;
			m_PaySchemeString->push_back(s);
		}

		for (auto it = vectory.begin(); it != vectory.end(); ++it)
		{
			_itoa_s(abs((*it).money * (*m_Sum)), buf, 10);
			CString s = _T("[???]  ��  ") + (*it).name + +_TEXT("  ") + buf;
			m_PaySchemeString->push_back(s);
		}

		m_HistoryListBox->ResetContent();

		for (int i = 0; i < m_PaySchemeString->size(); ++i)
			m_HistoryListBox->InsertString(0, (*m_PaySchemeString)[i]);
	}

	//ִ������
	virtual void execute()
	{
		CString s;
		m_Edit->GetWindowText(s);
		*m_Sum = _ttoi(s);

		m_AllListBox->ResetContent();

		for (int i = 0; i < (int)m_AllMenberVect->size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = (*m_AllMenberVect)[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s((*m_AllMenberVect)[i]->money * (*m_Sum), buf, 10);
			m_AllListBox->InsertString(0, (*m_AllMenberVect)[i]->name + _TEXT("  ") + s + CString(buf));
		}

		PayScheme();

		//������Ϣ ��������
		m_AllMenberVect_Redo_Temp.clear();
		for (int i = 0; i < (int)m_AllMenberVect->size(); ++i)
			m_AllMenberVect_Redo_Temp.push_back(*(*m_AllMenberVect)[i]);
		m_PaySchemeString_Redo_Temp = *m_PaySchemeString;
		m_TempSum_Redo = *m_Sum;
	}

	//����
	virtual void redo()
	{
		for (int i = 0; i < (int)m_AllMenberVect_Redo_Temp.size(); ++i)
		{
			(*m_AllMenberVect)[i]->money = m_AllMenberVect_Redo_Temp[i].money;
			(*m_AllMenberVect)[i]->name = m_AllMenberVect_Redo_Temp[i].name;
		}

		*m_PaySchemeString = m_PaySchemeString_Redo_Temp;
		*m_Sum = m_TempSum_Redo;
		char buf[32];
		_itoa_s(*m_Sum, buf,32, 10);
		m_Edit->SetWindowText(_T(buf));

		m_HistoryListBox->ResetContent();
		m_AllListBox->ResetContent();

		//���֧������
		PayScheme();

		for (int i = 0; i < (int)m_AllMenberVect->size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = (*m_AllMenberVect)[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s((*m_AllMenberVect)[i]->money * (*m_Sum), buf, 10);
			m_AllListBox->InsertString(0, (*m_AllMenberVect)[i]->name + _TEXT("  ") + s + CString(buf));
		}
	}

	//����
	virtual void undo()
	{
		for (int i = 0; i < (int)m_AllMenberVect_Undo_Temp.size(); ++i)
		{
			(*m_AllMenberVect)[i]->money = m_AllMenberVect_Undo_Temp[i].money;
			(*m_AllMenberVect)[i]->name = m_AllMenberVect_Undo_Temp[i].name;
		}

		*m_PaySchemeString = m_PaySchemeString_Undo_Temp;
		*m_Sum = m_TempSum_Undo;
		char buf[32];
		_itoa_s(*m_Sum, buf, 32, 10);
		m_Edit->SetWindowText(_T(buf));

		m_HistoryListBox->ResetContent();
		m_AllListBox->ResetContent();

		//���֧������
		PayScheme();

		for (int i = 0; i < (int)m_AllMenberVect->size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = (*m_AllMenberVect)[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s((*m_AllMenberVect)[i]->money * (*m_Sum), buf, 10);
			m_AllListBox->InsertString(0, (*m_AllMenberVect)[i]->name + _TEXT("  ") + s + CString(buf));
		}
	}
};
