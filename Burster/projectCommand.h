#pragma once
#include "stdafx.h"
#include "BursterDlg.h"
#include "Command.h"
#include "member.h"
#include "Config.h"

#define _MD m_MainDlg

//�������
class AddCommand : public Command
{
private:
	CBursterDlg* m_MainDlg;//��������
	CString m_AddName;

	bool m_Undo;//�Ƿ񳷻�
	stMember* m_NewMember;//��ӵĳ�Ա ������صĻ� Ҫ�Լ�delete��

	vector<stMember*> m_AllMemberVect_Undo_Temp;
	vector<stMember*> m_CurMemberVect_Undo_Temp;
	vector<stMember*> m_DoveMemberVect_Undo_Temp;

	vector<stMember*> m_AllMemberVect_Redo_Temp;
	vector<stMember*> m_CurMemberVect_Redo_Temp;
	vector<stMember*> m_DoveMemberVect_Redo_Temp;
public:

	AddCommand(CString name, CBursterDlg* pDlg)
		:
		m_AddName(name),
		m_NewMember(NULL),
		m_Undo(false),
		m_MainDlg(pDlg)
	{
		m_AllMemberVect_Undo_Temp = _MD->m_AllMemberVect;
		m_CurMemberVect_Undo_Temp = _MD->m_CurMemberVect;
		m_DoveMemberVect_Undo_Temp = _MD->m_DoveMemberVect;
	}

	virtual	~AddCommand()
	{
		if (m_Undo)
			delete m_NewMember;
	}

	//ִ������
	virtual void execute()
	{
		// TODO: �ڴ���ӿؼ�֪ͨ����������
		stMember* m = new stMember;
		m->money = 0;
		m->name = m_AddName;
		
		if (m->name == _TEXT(""))
		{
			delete m;
			return;
		}

		//�жϵ�ǰ��Ա�Ƿ���ȫ������б���
		stMember* p = IsMemberInVect(*m, _MD->m_AllMemberVect);

		//û���ظ������
		if (NULL == p)
		{
			_MD->m_CurListBox->AddString(m->name);
			_MD->m_AllListBox->AddString(m->name + _TEXT("  +0"));
			_MD->m_CurMemberVect.push_back(m);
			_MD->m_AllMemberVect.push_back(m);
		}
		else
		{
			if (NULL == IsMemberInVect(*p, _MD->m_CurMemberVect))
			{
				_MD->m_CurMemberVect.push_back(p);
				_MD->m_CurListBox->AddString(p->name);
				for (int i = 0; i < (int)_MD->m_DoveMemberVect.size(); ++i)
				{
					if (m->name == _MD->m_DoveMemberVect[i]->name)
					{
						_MD->m_DoveMemberVect.erase(_MD->m_DoveMemberVect.begin() + i);
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

		m_AllMemberVect_Redo_Temp = _MD->m_AllMemberVect;
		m_CurMemberVect_Redo_Temp = _MD->m_CurMemberVect;
		m_DoveMemberVect_Redo_Temp = _MD->m_DoveMemberVect;
		m_NewMember = m;//��¼��ǰ��Ա
	}

	//����
	virtual void redo()
	{
		m_Undo = false;
		_MD->m_AllMemberVect = m_AllMemberVect_Redo_Temp;
		_MD->m_CurMemberVect = m_CurMemberVect_Redo_Temp;
		_MD->m_DoveMemberVect = m_DoveMemberVect_Redo_Temp;

		_MD->m_CurListBox->ResetContent();
		_MD->m_AllListBox->ResetContent();
		for (int i = 0; i < (int)_MD->m_CurMemberVect.size(); ++i)
			_MD->m_CurListBox->AddString((_MD->m_CurMemberVect)[i]->name);
		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
		{
			CString money, flag("");
			money.Format("%d", _MD->m_AllMemberVect[i]->money * _MD->m_Sum);
			if (_MD->m_AllMemberVect[i]->money >= 0)
				flag = "+";
			_MD->m_AllListBox->AddString(_MD->m_AllMemberVect[i]->name + _T("  ") + flag + money);
		}
	}

	//����
	virtual void undo()
	{
		m_Undo = true;
		_MD->m_AllMemberVect = m_AllMemberVect_Undo_Temp;
		_MD->m_CurMemberVect = m_CurMemberVect_Undo_Temp;
		_MD->m_DoveMemberVect = m_DoveMemberVect_Undo_Temp;

		_MD->m_CurListBox->ResetContent();
		_MD->m_AllListBox->ResetContent();
		for (int i = 0; i < (int)_MD->m_CurMemberVect.size(); ++i)
			_MD->m_CurListBox->AddString((_MD->m_CurMemberVect)[i]->name);
		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
		{
			CString money, flag("");
			money.Format("%d", _MD->m_AllMemberVect[i]->money * _MD->m_Sum);
			if (_MD->m_AllMemberVect[i]->money >= 0)
				flag = "+";
			_MD->m_AllListBox->AddString(_MD->m_AllMemberVect[i]->name + _T("  ") + flag + money);
		}
	}
};

//ɾ������
class EraseCommand : public Command
{
private:

	int m_EraseIndex;
	CBursterDlg* m_MainDlg;//��������

	vector<stMember*> m_CurMemberVect_Undo_Temp;
	vector<stMember*> m_DoveMemberVect_Undo_Temp;

	vector<stMember*> m_CurMemberVect_Redo_Temp;
	vector<stMember*> m_DoveMemberVect_Redo_Temp;
public:

	EraseCommand(int eraseIdx, CBursterDlg* pDlg)
		:
		m_EraseIndex(eraseIdx),
		m_MainDlg(pDlg)
	{
		m_CurMemberVect_Undo_Temp = _MD->m_CurMemberVect;
		m_DoveMemberVect_Undo_Temp = _MD->m_DoveMemberVect;
	}

	virtual	~EraseCommand()
	{

	}

	//ִ������
	virtual void execute()
	{
		CString s;
		_MD->m_CurListBox->GetText(m_EraseIndex, s);
		_MD->m_CurListBox->DeleteString(m_EraseIndex);

		for (int i = 0; i < (int)_MD->m_CurMemberVect.size(); ++i)
		{
			if (_MD->m_CurMemberVect[i]->name == s)
			{
				_MD->m_DoveMemberVect.push_back(_MD->m_CurMemberVect[i]);
				_MD->m_CurMemberVect.erase(_MD->m_CurMemberVect.begin() + i);
				break;
			}
		}

		m_CurMemberVect_Redo_Temp = _MD->m_CurMemberVect;
		m_DoveMemberVect_Redo_Temp = _MD->m_DoveMemberVect;
	}

	//����
	virtual void redo()
	{
		_MD->m_CurMemberVect = m_CurMemberVect_Redo_Temp;
		_MD->m_DoveMemberVect = m_DoveMemberVect_Redo_Temp;

		_MD->m_CurListBox->ResetContent();
		for (int i = 0; i < (int)_MD->m_CurMemberVect.size(); ++i)
			_MD->m_CurListBox->AddString(_MD->m_CurMemberVect[i]->name);
	}

	//����
	virtual void undo()
	{
		_MD->m_CurMemberVect = m_CurMemberVect_Undo_Temp;
		_MD->m_DoveMemberVect = m_DoveMemberVect_Undo_Temp;

		_MD->m_CurListBox->ResetContent();
		for (int i = 0; i < (int)_MD->m_CurMemberVect.size(); ++i)
			_MD->m_CurListBox->AddString(_MD->m_CurMemberVect[i]->name);
	}
};

//��������
class GroupingCommand : public Command
{
private:

	CBursterDlg* m_MainDlg;//��������

	//ִ������ǰ������
	vector<stMember*> m_CurMemberVect_Undo_Temp;
	vector<stMember*> m_BlueMemberVect_Undo_Temp;
	vector<stMember*> m_RedMemberVect_Undo_Temp;
	vector<stData>   m_Data_Undo_Temp;

	//ִ������������
	vector<stMember*> m_CurMemberVect_Redo_Temp;
	vector<stMember*> m_BlueMemberVect_Redo_Temp;
	vector<stMember*> m_RedMemberVect_Redo_Temp;
	vector<stData>   m_Data_Redo_Temp;

public:

	GroupingCommand(CBursterDlg* pDlg)
		:
		m_MainDlg(pDlg)
	{
		m_CurMemberVect_Undo_Temp = _MD->m_CurMemberVect;
		m_BlueMemberVect_Undo_Temp = _MD->m_BlueMemberVect;
		m_RedMemberVect_Undo_Temp = _MD->m_RedMemberVect;
		m_Data_Undo_Temp = _MD->m_Data;
	}

	virtual	~GroupingCommand()
	{

	}

	//ִ������
	virtual void execute()
	{
		//����
		vector<stMember*> temp = _MD->m_CurMemberVect;
		_MD->m_RedListBox->ResetContent();
		_MD->m_BlueListBox->ResetContent();

		_MD->m_RedMemberVect.clear();
		_MD->m_BlueMemberVect.clear();

		bool flag = false;

		while (temp.size() != 0)
		{
			int rd = rand() % temp.size();

			if (flag)
			{
				_MD->m_RedMemberVect.push_back(temp[rd]);
				_MD->m_RedListBox->AddString(temp[rd]->name);
				flag = false;
			}
			else
			{
				_MD->m_BlueMemberVect.push_back(temp[rd]);
				_MD->m_BlueListBox->AddString(temp[rd]->name);
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
		_MD->m_Data.push_back(data);

		//��¼��ǰ��Ϣ
		m_CurMemberVect_Redo_Temp = _MD->m_CurMemberVect;
		m_RedMemberVect_Redo_Temp = _MD->m_RedMemberVect;
		m_BlueMemberVect_Redo_Temp = _MD->m_BlueMemberVect;
		m_Data_Redo_Temp = _MD->m_Data;
	}

	//����
	virtual void redo()
	{
		_MD->m_CurMemberVect = m_CurMemberVect_Redo_Temp;
		_MD->m_RedMemberVect = m_RedMemberVect_Redo_Temp;
		_MD->m_BlueMemberVect = m_BlueMemberVect_Redo_Temp;
		_MD->m_Data = m_Data_Redo_Temp;

		_MD->m_BlueListBox->ResetContent();
		_MD->m_RedListBox->ResetContent();

		for (auto it = _MD->m_RedMemberVect.begin(); it != _MD->m_RedMemberVect.end(); ++it)
			_MD->m_RedListBox->AddString((*it)->name);

		for (auto it = _MD->m_BlueMemberVect.begin(); it != _MD->m_BlueMemberVect.end(); ++it)
			_MD->m_BlueListBox->AddString((*it)->name);
	}

	//����
	virtual void undo()
	{
		_MD->m_CurMemberVect = m_CurMemberVect_Undo_Temp;
		_MD->m_RedMemberVect = m_RedMemberVect_Undo_Temp;
		_MD->m_BlueMemberVect = m_BlueMemberVect_Undo_Temp;
		_MD->m_Data = m_Data_Undo_Temp;

		_MD->m_BlueListBox->ResetContent();
		_MD->m_RedListBox->ResetContent();

		for (auto it = _MD->m_RedMemberVect.begin(); it != _MD->m_RedMemberVect.end(); ++it)
			_MD->m_RedListBox->AddString((*it)->name);

		for (auto it = _MD->m_BlueMemberVect.begin(); it != _MD->m_BlueMemberVect.end(); ++it)
			_MD->m_BlueListBox->AddString((*it)->name);
	}
};

//ʤ������
class WinCommand : public Command
{
private:

	bool m_BlueWin;//ʤ������
	CBursterDlg* m_MainDlg;//��������

	//ִ������ǰ������
	vector<CString>  m_PaySchemeString_Undo_Temp;
	vector<stMember> m_AllMemberVect_Undo_Temp;
	vector<stMember*> m_BlueMemberVect_Undo_Temp;
	vector<stMember*> m_RedMemberVect_Undo_Temp;
	vector<stData>    m_Data_Undo_Temp;

	//ִ������������
	vector<CString>  m_PaySchemeString_Redo_Temp;
	vector<stMember> m_AllMemberVect_Redo_Temp;
	vector<stMember*> m_BlueMemberVect_Redo_Temp;
	vector<stMember*> m_RedMemberVect_Redo_Temp;
	vector<stData>    m_Data_Redo_Temp;

public:

	WinCommand(bool redWin, CBursterDlg* pDlg)
		:
		m_BlueWin(redWin),
		m_MainDlg(pDlg)
	{

		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
		{
			m_AllMemberVect_Undo_Temp.push_back(*_MD->m_AllMemberVect[i]);
		}

		m_BlueMemberVect_Undo_Temp = _MD->m_BlueMemberVect;
		m_RedMemberVect_Undo_Temp = _MD->m_RedMemberVect;
		m_Data_Undo_Temp = _MD->m_Data;
		m_PaySchemeString_Undo_Temp = _MD->m_PaySchemeString;
	}

	virtual	~WinCommand()
	{

	}

	//ִ������
	virtual void execute()
	{
		for (int i = 0; i < (int)_MD->m_BlueMemberVect.size(); ++i)
			_MD->m_BlueMemberVect[i]->money += (m_BlueWin ? -1 : 1);

		for (int i = 0; i < (int)_MD->m_RedMemberVect.size(); ++i)
			_MD->m_RedMemberVect[i]->money += (m_BlueWin ? 1 : -1);

		_MD->m_RedListBox->ResetContent();
		_MD->m_BlueListBox->ResetContent();
		_MD->m_AllListBox->ResetContent();

		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = _MD->m_AllMemberVect[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s(_MD->m_AllMemberVect[i]->money * _MD->m_Sum, buf, 10);
			_MD->m_AllListBox->AddString(_MD->m_AllMemberVect[i]->name + _TEXT("  ") + s + CString(buf));
		}

		//���֧������
		PayScheme(&_MD->m_PaySchemeString, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);
		
		if (_MD->m_Data.size() > 0)
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
			_MD->m_Data[_MD->m_Data.size() - 1].vectoryTmie = buf;
			_MD->m_Data[_MD->m_Data.size() - 1].redLose = 1;

			for (int i = 0; i < (int)_MD->m_RedMemberVect.size(); ++i)
				(_MD->m_Data)[_MD->m_Data.size() - 1].red.push_back(*_MD->m_RedMemberVect[i]);

			for (int i = 0; i < (int)_MD->m_BlueMemberVect.size(); ++i)
				(_MD->m_Data)[_MD->m_Data.size() - 1].blue.push_back(*_MD->m_BlueMemberVect[i]);
		}

		_MD->m_BlueMemberVect.clear();
		_MD->m_RedMemberVect.clear();

		//������Ϣ ��������
		m_AllMemberVect_Redo_Temp.clear();
		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
			m_AllMemberVect_Redo_Temp.push_back(*_MD->m_AllMemberVect[i]);

		m_RedMemberVect_Redo_Temp = _MD->m_RedMemberVect;
		m_BlueMemberVect_Redo_Temp = _MD->m_BlueMemberVect;
		m_PaySchemeString_Redo_Temp = _MD->m_PaySchemeString;
		m_Data_Redo_Temp = _MD->m_Data;
	}

	//����
	virtual void redo()
	{
		for (int i = 0; i < (int)m_AllMemberVect_Redo_Temp.size(); ++i)
		{
			_MD->m_AllMemberVect[i]->money = m_AllMemberVect_Redo_Temp[i].money;
			_MD->m_AllMemberVect[i]->name = m_AllMemberVect_Redo_Temp[i].name;
		}

		_MD->m_RedMemberVect = m_RedMemberVect_Redo_Temp;
		_MD->m_BlueMemberVect = m_BlueMemberVect_Redo_Temp;
		_MD->m_Data = m_Data_Redo_Temp;
		_MD->m_PaySchemeString = m_PaySchemeString_Redo_Temp;

		_MD->m_BlueListBox->ResetContent();
		_MD->m_RedListBox->ResetContent();
		_MD->m_PaySchemeListBox->ResetContent();
		_MD->m_AllListBox->ResetContent();

		//���֧������
		PayScheme(&_MD->m_PaySchemeString, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);

		for (auto it = _MD->m_RedMemberVect.begin(); it != _MD->m_RedMemberVect.end(); ++it)
			_MD->m_RedListBox->AddString((*it)->name);

		for (auto it = _MD->m_BlueMemberVect.begin(); it != _MD->m_BlueMemberVect.end(); ++it)
			_MD->m_BlueListBox->AddString((*it)->name);

		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = _MD->m_AllMemberVect[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s(_MD->m_AllMemberVect[i]->money * _MD->m_Sum, buf, 10);
			_MD->m_AllListBox->AddString(_MD->m_AllMemberVect[i]->name + _TEXT("  ") + s + CString(buf));
		}
	}

	//����
	virtual void undo()
	{
		for (int i = 0; i < (int)m_AllMemberVect_Undo_Temp.size(); ++i)
		{
			_MD->m_AllMemberVect[i]->money = m_AllMemberVect_Undo_Temp[i].money;
			_MD->m_AllMemberVect[i]->name = m_AllMemberVect_Undo_Temp[i].name;
		}

		_MD->m_RedMemberVect = m_RedMemberVect_Undo_Temp;
		_MD->m_BlueMemberVect = m_BlueMemberVect_Undo_Temp;
		_MD->m_Data = m_Data_Undo_Temp;
		_MD->m_PaySchemeString = m_PaySchemeString_Undo_Temp;

		_MD->m_BlueListBox->ResetContent();
		_MD->m_RedListBox->ResetContent();
		_MD->m_PaySchemeListBox->ResetContent();
		_MD->m_AllListBox->ResetContent();

		//���֧������
		PayScheme(&_MD->m_PaySchemeString, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);

		for (auto it = _MD->m_RedMemberVect.begin(); it != _MD->m_RedMemberVect.end(); ++it)
			_MD->m_RedListBox->AddString((*it)->name);

		for (auto it = _MD->m_BlueMemberVect.begin(); it != _MD->m_BlueMemberVect.end(); ++it)
			_MD->m_BlueListBox->AddString((*it)->name);

		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = _MD->m_AllMemberVect[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s(_MD->m_AllMemberVect[i]->money * _MD->m_Sum, buf, 10);
			_MD->m_AllListBox->AddString(_MD->m_AllMemberVect[i]->name + _TEXT("  ") + s + CString(buf));
		}
	}
};

//�ı��ע����
class ChangedMoneyCommand : public Command
{
private:

	int m_TempSum_Undo;
	int m_TempSum_Redo;

	CBursterDlg* m_MainDlg;//��������

	vector<CString>  m_PaySchemeString_Undo_Temp;
	vector<stMember> m_AllMemberVect_Undo_Temp;

	vector<CString>  m_PaySchemeString_Redo_Temp;
	vector<stMember> m_AllMemberVect_Redo_Temp;

public:

	ChangedMoneyCommand(CBursterDlg* pDlg)
		:
		m_MainDlg(pDlg),
		m_TempSum_Undo(pDlg->m_Sum)
	{

		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
			m_AllMemberVect_Undo_Temp.push_back(*_MD->m_AllMemberVect[i]);

		m_PaySchemeString_Undo_Temp = _MD->m_PaySchemeString;
	}

	virtual	~ChangedMoneyCommand()
	{

	}

	//ִ������
	virtual void execute()
	{
		CString s;
		_MD->m_SumEdit->GetWindowText(s);
		_MD->m_Sum = _ttoi(s);

		_MD->m_AllListBox->ResetContent();

		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = _MD->m_AllMemberVect[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s(_MD->m_AllMemberVect[i]->money * (_MD->m_Sum), buf, 10);
			_MD->m_AllListBox->AddString(_MD->m_AllMemberVect[i]->name + _TEXT("  ") + s + CString(buf));
		}

		//���֧������
		PayScheme(&_MD->m_PaySchemeString, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);

		//������Ϣ ��������
		m_AllMemberVect_Redo_Temp.clear();
		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
			m_AllMemberVect_Redo_Temp.push_back(*_MD->m_AllMemberVect[i]);

		m_PaySchemeString_Redo_Temp = _MD->m_PaySchemeString;
		m_TempSum_Redo = _MD->m_Sum;
	}

	//����
	virtual void redo()
	{
		for (int i = 0; i < (int)m_AllMemberVect_Redo_Temp.size(); ++i)
		{
			_MD->m_AllMemberVect[i]->money = m_AllMemberVect_Redo_Temp[i].money;
			_MD->m_AllMemberVect[i]->name = m_AllMemberVect_Redo_Temp[i].name;
		}

		_MD->m_PaySchemeString = m_PaySchemeString_Redo_Temp;
		_MD->m_Sum = m_TempSum_Redo;
		char buf[32];
		_itoa_s(_MD->m_Sum, buf,32, 10);
		_MD->m_SumEdit->SetWindowText(_T(buf));

		_MD->m_PaySchemeListBox->ResetContent();
		_MD->m_AllListBox->ResetContent();

		//���֧������
		PayScheme(&_MD->m_PaySchemeString, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);

		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = _MD->m_AllMemberVect[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s(_MD->m_AllMemberVect[i]->money * (_MD->m_Sum), buf, 10);
			_MD->m_AllListBox->AddString(_MD->m_AllMemberVect[i]->name + _TEXT("  ") + s + CString(buf));
		}
	}

	//����
	virtual void undo()
	{
		for (int i = 0; i < (int)m_AllMemberVect_Undo_Temp.size(); ++i)
		{
			_MD->m_AllMemberVect[i]->money = m_AllMemberVect_Undo_Temp[i].money;
			_MD->m_AllMemberVect[i]->name = m_AllMemberVect_Undo_Temp[i].name;
		}

		_MD->m_PaySchemeString = m_PaySchemeString_Undo_Temp;
		_MD->m_Sum = m_TempSum_Undo;
		char buf[32];
		_itoa_s(_MD->m_Sum, buf, 32, 10);
		_MD->m_SumEdit->SetWindowText(_T(buf));

		_MD->m_PaySchemeListBox->ResetContent();
		_MD->m_AllListBox->ResetContent();

		//���֧������
		PayScheme(&_MD->m_PaySchemeString, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);

		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = (_MD->m_AllMemberVect)[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s((_MD->m_AllMemberVect)[i]->money * (_MD->m_Sum), buf, 10);
			_MD->m_AllListBox->AddString(_MD->m_AllMemberVect[i]->name + _TEXT("  ") + s + CString(buf));
		}
	}
};

///��������
class ConfigCommand : public Command
{
public:

	bool m_Changed;//�Ƿ����Զ�������

private:

	CBursterDlg* m_MainDlg;//��������

	//ִ��ǰ������
	vector<stData> m_Data_Undo_Temp;
	vector<stMember> m_AllMemberVect_Undo_Temp;
	vector<stMember> m_CurMemberVect_Undo_Temp;
	vector<stMember> m_RedMemberVect_Undo_Temp;
	vector<stMember> m_BlueMemberVect_Undo_Temp;

	//ִ�к������
	vector<stData> m_Data_Redo_Temp;
	vector<stMember> m_AllMemberVect_Redo_Temp;
	vector<stMember> m_CurMemberVect_Redo_Temp;
	vector<stMember> m_BlueMemberVect_Redo_Temp;
	vector<stMember> m_RedMemberVect_Redo_Temp;

public:

	ConfigCommand(CBursterDlg* pDlg)
		:
		m_MainDlg(pDlg),
		m_Changed(false)
	{
		MemberCopy(m_AllMemberVect_Undo_Temp, _MD->m_AllMemberVect);
		MemberCopy(m_CurMemberVect_Undo_Temp, _MD->m_CurMemberVect);
		MemberCopy(m_BlueMemberVect_Undo_Temp, _MD->m_BlueMemberVect);
		MemberCopy(m_RedMemberVect_Undo_Temp, _MD->m_RedMemberVect);
	}

	virtual	~ConfigCommand()
	{
	}

	//ִ������
	virtual void execute()
	{
		//�������ô���
		CConfig dlg(m_Changed, m_MainDlg);
		dlg.DoModal();

		//�޸�
		if (m_Changed)
		{
			//����ˢ���б������
			SetListBoxData();

			//���¼������֧������
			vector<CString> pay;
			PayScheme(&pay, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);
		}

		//��¼ִ������֮������ݣ���������
		MemberCopy(m_AllMemberVect_Redo_Temp, _MD->m_AllMemberVect);
		MemberCopy(m_CurMemberVect_Redo_Temp, _MD->m_CurMemberVect);
		MemberCopy(m_BlueMemberVect_Redo_Temp, _MD->m_BlueMemberVect);
		MemberCopy(m_RedMemberVect_Redo_Temp, _MD->m_RedMemberVect);
	}

	//���ø����б������
	void SetListBoxData()
	{
		CListBox* list[] =
		{
			_MD->m_CurListBox,
			_MD->m_RedListBox,
			_MD->m_BlueListBox,
			_MD->m_AllListBox
		};

		vector<stMember*>* vect[] =
		{
			&_MD->m_CurMemberVect,
			&_MD->m_RedMemberVect,
			&_MD->m_BlueMemberVect,
			&_MD->m_AllMemberVect
		};

		for (int i = 0; i < 4; ++i)
		{
			list[i]->ResetContent();
			for (auto it = vect[i]->begin(); it != vect[i]->end(); ++it)
			{
				CString str = (*it)->name;
				CString money;
				money.Format("%d", (*it)->money * _MD->m_Sum);
				if ((*it)->money >= 0)
					money = _T("+") + money;
				if (i == 3)
					str = str + _T("  ") + money;
				list[i]->AddString(str);
			}
		}
	}

	//����
	virtual void redo()
	{
		//��ԭִ������ǰ������
		MemberCopy(_MD->m_AllMemberVect, m_AllMemberVect_Redo_Temp);

		//��ԭ����б�
		_MD->m_RedMemberVect.clear();
		for (int i = 0; i < (int)m_RedMemberVect_Redo_Temp.size(); ++i)
		{
			int index = IsMemberInVect_index(m_RedMemberVect_Redo_Temp[i], _MD->m_AllMemberVect);
			if (index != -1)
				_MD->m_RedMemberVect.push_back(_MD->m_AllMemberVect[index]);
		}

		//��ԭ���Ӷ��б�
		_MD->m_BlueMemberVect.clear();
		for (int i = 0; i < (int)m_BlueMemberVect_Redo_Temp.size(); ++i)
		{
			int index = IsMemberInVect_index(m_BlueMemberVect_Redo_Temp[i], _MD->m_AllMemberVect);
			if (index != -1)
				_MD->m_BlueMemberVect.push_back(_MD->m_AllMemberVect[index]);
		}

		//����ˢ���б������
		SetListBoxData();

		//���¼������֧������
		vector<CString> pay;
		PayScheme(&pay, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);
	}

	//����
	virtual void undo()
	{
		//��ԭִ������ǰ������
		MemberCopy(_MD->m_AllMemberVect, m_AllMemberVect_Undo_Temp);

		//��ԭ����б�
		_MD->m_RedMemberVect.clear();
		for (int i = 0; i < (int)m_RedMemberVect_Undo_Temp.size(); ++i)
		{
			int index = IsMemberInVect_index(m_RedMemberVect_Undo_Temp[i], _MD->m_AllMemberVect);
			if (index != -1)
				_MD->m_RedMemberVect.push_back(_MD->m_AllMemberVect[index]);
		}

		//��ԭ���Ӷ��б�
		_MD->m_BlueMemberVect.clear();
		for (int i = 0; i < (int)m_BlueMemberVect_Undo_Temp.size(); ++i)
		{
			int index = IsMemberInVect_index(m_BlueMemberVect_Undo_Temp[i], _MD->m_AllMemberVect);
			if (index != -1)
				_MD->m_BlueMemberVect.push_back(_MD->m_AllMemberVect[index]);
		}

		//����ˢ���б������
		SetListBoxData();

		//���¼������֧������
		vector<CString> pay;
		PayScheme(&pay, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);
	}
};
