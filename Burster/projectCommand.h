#pragma once
#include "stdafx.h"
#include "BursterDlg.h"
#include <algorithm>
#include "afxdialogex.h"
#include "Command.h"
#include "member.h"
#include "Config.h"

#define MD m_MainDlg


//添加命令
class AddCommand : public Command
{
private:
	CBursterDlg* m_MainDlg;//主窗口类
	CListBox* m_AllListBox;
	CListBox* m_CurListBox;
	CString m_name;

	bool m_Undo;//是否撤回
	stMember* m_NewMember;//添加的成员 如果撤回的话 要自己delete掉

	vector<stMember*> m_AllMemberVect_Undo_Temp;
	vector<stMember*> m_CurMemberVect_Undo_Temp;
	vector<stMember*> m_DoveMemberVect_Undo_Temp;
	vector<stMember*> m_AllMemberVect_Redo_Temp;
	vector<stMember*> m_CurMemberVect_Redo_Temp;
	vector<stMember*> m_DoveMemberVect_Redo_Temp;
public:

	AddCommand(CListBox* all, CListBox* cur, CString name, CBursterDlg* pDlg)
		:
		m_AllListBox(all),
		m_CurListBox(cur),
		m_name(name),
		m_NewMember(NULL),
		m_Undo(false),
		m_MainDlg(pDlg)
	{
		m_AllMemberVect_Undo_Temp = MD->m_AllMemberVect;
		m_CurMemberVect_Undo_Temp = MD->m_CurMemberVect;
		m_DoveMemberVect_Undo_Temp = MD->m_DoveMemberVect;
	}

	virtual	~AddCommand()
	{
		if (m_Undo)
			delete m_NewMember;
	}


	//执行命令
	virtual void execute()
	{
		// TODO: 在此添加控件通知处理程序代码
		stMember* m = new stMember;
		m->money = 0;
		m->name = m_name;
		
		if (m->name == _TEXT(""))
		{
			delete m;
			return;
		}

		//判断当前成员是否在全部玩家列表中
		stMember* p = IsMemberInVect(*m, MD->m_AllMemberVect);

		//没有重复才添加
		if (NULL == p)
		{
			m_CurListBox->AddString(m->name);
			m_AllListBox->AddString(m->name + _TEXT("  +0"));
			MD->m_CurMemberVect.push_back(m);
			MD->m_AllMemberVect.push_back(m);
		}
		else
		{
			if (NULL == IsMemberInVect(*p, MD->m_CurMemberVect))
			{
				MD->m_CurMemberVect.push_back(p);
				m_CurListBox->AddString(p->name);
				for (int i = 0; i < MD->m_DoveMemberVect.size(); ++i)
				{
					if (m->name == (MD->m_DoveMemberVect)[i]->name)
					{
						MD->m_DoveMemberVect.erase(MD->m_DoveMemberVect.begin() + i);
						break;
					}
				}
			}
			else
			{
				MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd, _T("重复添加"), _T("提示"), MB_OK);
			}
			delete m;
		}

		m_AllMemberVect_Redo_Temp = MD->m_AllMemberVect;
		m_CurMemberVect_Redo_Temp = MD->m_CurMemberVect;
		m_DoveMemberVect_Redo_Temp = MD->m_DoveMemberVect;
		m_NewMember = m;//记录当前成员
	}

	//重做
	virtual void redo()
	{
		m_Undo = false;
		MD->m_AllMemberVect = m_AllMemberVect_Redo_Temp;
		MD->m_CurMemberVect = m_CurMemberVect_Redo_Temp;
		MD->m_DoveMemberVect = m_DoveMemberVect_Redo_Temp;

		m_CurListBox->ResetContent();
		m_AllListBox->ResetContent();
		for (int i = 0; i < (int)MD->m_CurMemberVect.size(); ++i)
			m_CurListBox->AddString((MD->m_CurMemberVect)[i]->name);
		for (int i = 0; i < (int)MD->m_AllMemberVect.size(); ++i)
			m_AllListBox->AddString((MD->m_AllMemberVect)[i]->name + _T("  +0"));
	}

	//撤销
	virtual void undo()
	{
		m_Undo = true;
		MD->m_AllMemberVect = m_AllMemberVect_Undo_Temp;
		MD->m_CurMemberVect = m_CurMemberVect_Undo_Temp;
		MD->m_DoveMemberVect = m_DoveMemberVect_Undo_Temp;

		m_CurListBox->ResetContent();
		m_AllListBox->ResetContent();
		for (int i = 0; i < (int)MD->m_CurMemberVect.size(); ++i)
			m_CurListBox->AddString((MD->m_CurMemberVect)[i]->name);
		for (int i = 0; i < (int)MD->m_AllMemberVect.size(); ++i)
			m_AllListBox->AddString((MD->m_AllMemberVect)[i]->name + _T("  +0"));
	}
};

//删除命令
class EraseCommand : public Command
{
private:

	int m_EraseIndex;
	CBursterDlg* m_MainDlg;//主窗口类
	CListBox* m_CurListBox;

	vector<stMember*> m_CurMemberVect_Undo_Temp;
	vector<stMember*> m_DoveMemberVect_Undo_Temp;

	vector<stMember*> m_CurMemberVect_Redo_Temp;
	vector<stMember*> m_DoveMemberVect_Redo_Temp;
public:

	EraseCommand(CListBox* cur, int eraseIdx, CBursterDlg* pDlg)
		:
		m_CurListBox(cur),
		m_EraseIndex(eraseIdx),
		m_MainDlg(pDlg)
	{
		m_CurMemberVect_Undo_Temp = MD->m_CurMemberVect;
		m_DoveMemberVect_Undo_Temp = MD->m_DoveMemberVect;
	}

	virtual	~EraseCommand()
	{

	}

	//执行命令
	virtual void execute()
	{
		CString s;
		m_CurListBox->GetText(m_EraseIndex, s);
		m_CurListBox->DeleteString(m_EraseIndex);

		for (int i = 0; i < MD->m_CurMemberVect.size(); ++i)
		{
			if ((MD->m_CurMemberVect)[i]->name == s)
			{
				MD->m_DoveMemberVect.push_back((MD->m_CurMemberVect)[i]);
				MD->m_CurMemberVect.erase(MD->m_CurMemberVect.begin() + i);
				break;
			}
		}

		m_CurMemberVect_Redo_Temp = MD->m_CurMemberVect;
		m_DoveMemberVect_Redo_Temp = MD->m_DoveMemberVect;
	}

	//重做
	virtual void redo()
	{
		MD->m_CurMemberVect = m_CurMemberVect_Redo_Temp;
		MD->m_DoveMemberVect = m_DoveMemberVect_Redo_Temp;

		m_CurListBox->ResetContent();
		for (int i = 0; i < (int)MD->m_CurMemberVect.size(); ++i)
			m_CurListBox->AddString((MD->m_CurMemberVect)[i]->name);
	}

	//撤销
	virtual void undo()
	{
		MD->m_CurMemberVect = m_CurMemberVect_Undo_Temp;
		MD->m_DoveMemberVect = m_DoveMemberVect_Undo_Temp;

		m_CurListBox->ResetContent();
		for (int i = 0; i < (int)MD->m_CurMemberVect.size(); ++i)
			m_CurListBox->AddString((MD->m_CurMemberVect)[i]->name);
	}
};

//分组命令
class GroupingCommand : public Command
{
private:

	CBursterDlg* m_MainDlg;//主窗口类
	CListBox* m_RedListBox;
	CListBox* m_BlueListBox;

	//执行命令前的数据
	vector<stMember*> m_CurMemberVect_Undo_Temp;
	vector<stMember*> m_BlueMemberVect_Undo_Temp;
	vector<stMember*> m_RedMemberVect_Undo_Temp;
	vector<stData>   m_Data_Undo_Temp;

	//执行命令后的数据
	vector<stMember*> m_CurMemberVect_Redo_Temp;
	vector<stMember*> m_BlueMemberVect_Redo_Temp;
	vector<stMember*> m_RedMemberVect_Redo_Temp;
	vector<stData>   m_Data_Redo_Temp;

public:

	GroupingCommand(CListBox* redListBox,
					CListBox* BlueListBox,
					CBursterDlg* pDlg)
		:
		m_RedListBox(redListBox),
		m_BlueListBox(BlueListBox),
		m_MainDlg(pDlg)
	{
		m_CurMemberVect_Undo_Temp = MD->m_CurMemberVect;
		m_BlueMemberVect_Undo_Temp = MD->m_BlueMemberVect;
		m_RedMemberVect_Undo_Temp = MD->m_RedMemberVect;
		m_Data_Undo_Temp = MD->m_Data;
	}

	virtual	~GroupingCommand()
	{

	}

	//执行命令
	virtual void execute()
	{
		//分组
		vector<stMember*> temp = MD->m_CurMemberVect;
		m_RedListBox->ResetContent();
		m_BlueListBox->ResetContent();

		MD->m_RedMemberVect.clear();
		MD->m_BlueMemberVect.clear();

		bool flag = false;

		while (temp.size() != 0)
		{
			int rd = rand() % temp.size();

			if (flag)
			{
				MD->m_RedMemberVect.push_back(temp[rd]);
				m_RedListBox->AddString(temp[rd]->name);
				flag = false;
			}
			else
			{
				MD->m_BlueMemberVect.push_back(temp[rd]);
				m_BlueListBox->AddString(temp[rd]->name);
				flag = true;
			}
			temp.erase(temp.begin() + rd);
		}

		CTime time = CTime::GetCurrentTime(); ///构造CTime对象  
		int m_nYear = time.GetYear(); ///年  
		int m_nMonth = time.GetMonth(); ///月  
		int m_nDay = time.GetDay(); ///日  
		int m_nHour = time.GetHour(); ///小时  
		int m_nMinute = time.GetMinute(); ///分钟  
		int m_nSecond = time.GetSecond(); ///秒  

		char buf[64];
		sprintf_s(buf, "%d年%d月%d日%d时%d分%d秒", m_nYear, m_nMonth, m_nDay, m_nHour, m_nMinute, m_nSecond);

		stData data;
		data.fenZutime = buf;
		data.redLose = 2;
		MD->m_Data.push_back(data);

		//记录当前信息
		m_CurMemberVect_Redo_Temp = MD->m_CurMemberVect;
		m_RedMemberVect_Redo_Temp = MD->m_RedMemberVect;
		m_BlueMemberVect_Redo_Temp = MD->m_BlueMemberVect;
		m_Data_Redo_Temp = MD->m_Data;
	}

	//重做
	virtual void redo()
	{
		MD->m_CurMemberVect = m_CurMemberVect_Redo_Temp;
		MD->m_RedMemberVect = m_RedMemberVect_Redo_Temp;
		MD->m_BlueMemberVect = m_BlueMemberVect_Redo_Temp;
		MD->m_Data = m_Data_Redo_Temp;

		m_BlueListBox->ResetContent();
		m_RedListBox->ResetContent();

		for (auto it = MD->m_RedMemberVect.begin(); it != MD->m_RedMemberVect.end(); ++it)
			m_RedListBox->AddString((*it)->name);

		for (auto it = MD->m_BlueMemberVect.begin(); it != MD->m_BlueMemberVect.end(); ++it)
			m_BlueListBox->AddString((*it)->name);
	}

	//撤销
	virtual void undo()
	{
		MD->m_CurMemberVect = m_CurMemberVect_Undo_Temp;
		MD->m_RedMemberVect = m_RedMemberVect_Undo_Temp;
		MD->m_BlueMemberVect = m_BlueMemberVect_Undo_Temp;
		MD->m_Data = m_Data_Undo_Temp;

		m_BlueListBox->ResetContent();
		m_RedListBox->ResetContent();

		for (auto it = MD->m_RedMemberVect.begin(); it != MD->m_RedMemberVect.end(); ++it)
			m_RedListBox->AddString((*it)->name);

		for (auto it = MD->m_BlueMemberVect.begin(); it != MD->m_BlueMemberVect.end(); ++it)
			m_BlueListBox->AddString((*it)->name);
	}
};

//胜利命令
class WinCommand : public Command
{
private:

	bool m_BlueWin;//胜利队伍
	CBursterDlg* m_MainDlg;//主窗口类

	CListBox* m_RedListBox;
	CListBox* m_BlueListBox;
	CListBox* m_HistoryListBox;
	CListBox* m_AllListBox;

	//执行命令前的数据
	vector<CString>  m_PaySchemeString_Undo_Temp;
	vector<stMember> m_AllMemberVect_Undo_Temp;
	vector<stMember*> m_BlueMemberVect_Undo_Temp;
	vector<stMember*> m_RedMemberVect_Undo_Temp;
	vector<stData>    m_Data_Undo_Temp;

	//执行命令后的数据
	vector<CString>  m_PaySchemeString_Redo_Temp;
	vector<stMember> m_AllMemberVect_Redo_Temp;
	vector<stMember*> m_BlueMemberVect_Redo_Temp;
	vector<stMember*> m_RedMemberVect_Redo_Temp;
	vector<stData>    m_Data_Redo_Temp;

public:

	WinCommand(CListBox* redListBox,
		CListBox* BlueListBox,
		CListBox* allListBox,
		CListBox* HistoryListBox,
		bool redWin,
		CBursterDlg* pDlg)
		:

		m_RedListBox(redListBox),
		m_BlueListBox(BlueListBox),
		m_AllListBox(allListBox),
		m_HistoryListBox(HistoryListBox),
		m_BlueWin(redWin),
		m_MainDlg(pDlg)
	{

		for (int i = 0; i < (int)MD->m_AllMemberVect.size(); ++i)
		{
			m_AllMemberVect_Undo_Temp.push_back(*(MD->m_AllMemberVect)[i]);
		}

		m_BlueMemberVect_Undo_Temp = MD->m_BlueMemberVect;
		m_RedMemberVect_Undo_Temp = MD->m_RedMemberVect;
		m_Data_Undo_Temp = MD->m_Data;
		m_PaySchemeString_Undo_Temp = MD->m_PaySchemeString;
	}

	virtual	~WinCommand()
	{

	}


	//执行命令
	virtual void execute()
	{
		for (int i = 0; i < (int)MD->m_BlueMemberVect.size(); ++i)
			(MD->m_BlueMemberVect)[i]->money += (m_BlueWin ? -1 : 1);

		for (int i = 0; i < (int)MD->m_RedMemberVect.size(); ++i)
			(MD->m_RedMemberVect)[i]->money += (m_BlueWin ? 1 : -1);

		m_RedListBox->ResetContent();
		m_BlueListBox->ResetContent();
		m_AllListBox->ResetContent();

		for (int i = 0; i < (int)MD->m_AllMemberVect.size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = (MD->m_AllMemberVect)[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s((MD->m_AllMemberVect)[i]->money * MD->m_Sum, buf, 10);
			m_AllListBox->InsertString(0, (MD->m_AllMemberVect)[i]->name + _TEXT("  ") + s + CString(buf));
		}

		//最佳支付方案
		PayScheme_g(&MD->m_PaySchemeString, &MD->m_AllMemberVect, MD->m_Sum, m_HistoryListBox);
		
		if (MD->m_Data.size() > 0)
		{
			CTime time = CTime::GetCurrentTime(); ///构造CTime对象  
			int m_nYear = time.GetYear(); ///年  
			int m_nMonth = time.GetMonth(); ///月  
			int m_nDay = time.GetDay(); ///日  
			int m_nHour = time.GetHour(); ///小时  
			int m_nMinute = time.GetMinute(); ///分钟  
			int m_nSecond = time.GetSecond(); ///秒  

			char buf[64];
			sprintf_s(buf, "%d年%d月%d日%d时%d分%d秒", m_nYear, m_nMonth, m_nDay, m_nHour, m_nMinute, m_nSecond);
			(MD->m_Data)[MD->m_Data.size() - 1].vectoryTmie = buf;
			(MD->m_Data)[MD->m_Data.size() - 1].redLose = 1;

			for (int i = 0; i < MD->m_RedMemberVect.size(); ++i)
				(MD->m_Data)[MD->m_Data.size() - 1].red.push_back(*(MD->m_RedMemberVect)[i]);

			for (int i = 0; i < MD->m_BlueMemberVect.size(); ++i)
				(MD->m_Data)[MD->m_Data.size() - 1].blue.push_back(*(MD->m_BlueMemberVect)[i]);
		}

		MD->m_BlueMemberVect.clear();
		MD->m_RedMemberVect.clear();

		//保存信息 用于重做
		m_AllMemberVect_Redo_Temp.clear();
		for (int i = 0; i < (int)MD->m_AllMemberVect.size(); ++i)
		{
			m_AllMemberVect_Redo_Temp.push_back(*(MD->m_AllMemberVect)[i]);
		}

		m_RedMemberVect_Redo_Temp = MD->m_RedMemberVect;
		m_BlueMemberVect_Redo_Temp = MD->m_BlueMemberVect;
		m_PaySchemeString_Redo_Temp = MD->m_PaySchemeString;
		m_Data_Redo_Temp = MD->m_Data;
	}

	//重做
	virtual void redo()
	{
		for (int i = 0; i < (int)m_AllMemberVect_Redo_Temp.size(); ++i)
		{
			(MD->m_AllMemberVect)[i]->money = m_AllMemberVect_Redo_Temp[i].money;
			(MD->m_AllMemberVect)[i]->name = m_AllMemberVect_Redo_Temp[i].name;
		}

		MD->m_RedMemberVect = m_RedMemberVect_Redo_Temp;
		MD->m_BlueMemberVect = m_BlueMemberVect_Redo_Temp;
		MD->m_Data = m_Data_Redo_Temp;
		MD->m_PaySchemeString = m_PaySchemeString_Redo_Temp;

		m_BlueListBox->ResetContent();
		m_RedListBox->ResetContent();
		m_HistoryListBox->ResetContent();
		m_AllListBox->ResetContent();

		//最佳支付方案
		PayScheme_g(&MD->m_PaySchemeString, &MD->m_AllMemberVect, MD->m_Sum, m_HistoryListBox);

		for (auto it = MD->m_RedMemberVect.begin(); it != MD->m_RedMemberVect.end(); ++it)
			m_RedListBox->AddString((*it)->name);

		for (auto it = MD->m_BlueMemberVect.begin(); it != MD->m_BlueMemberVect.end(); ++it)
			m_BlueListBox->AddString((*it)->name);

		for (int i = 0; i < (int)MD->m_AllMemberVect.size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = (MD->m_AllMemberVect)[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s((MD->m_AllMemberVect)[i]->money * MD->m_Sum, buf, 10);
			m_AllListBox->InsertString(0, (MD->m_AllMemberVect)[i]->name + _TEXT("  ") + s + CString(buf));
		}
	}

	//撤销
	virtual void undo()
	{
		for (int i = 0; i < (int)m_AllMemberVect_Undo_Temp.size(); ++i)
		{
			(MD->m_AllMemberVect)[i]->money = m_AllMemberVect_Undo_Temp[i].money;
			(MD->m_AllMemberVect)[i]->name = m_AllMemberVect_Undo_Temp[i].name;
		}

		MD->m_RedMemberVect = m_RedMemberVect_Undo_Temp;
		MD->m_BlueMemberVect = m_BlueMemberVect_Undo_Temp;
		MD->m_Data = m_Data_Undo_Temp;
		MD->m_PaySchemeString = m_PaySchemeString_Undo_Temp;

		m_BlueListBox->ResetContent();
		m_RedListBox->ResetContent();
		m_HistoryListBox->ResetContent();
		m_AllListBox->ResetContent();

		//最佳支付方案
		PayScheme_g(&MD->m_PaySchemeString, &MD->m_AllMemberVect, MD->m_Sum, m_HistoryListBox);

		for (auto it = MD->m_RedMemberVect.begin(); it != MD->m_RedMemberVect.end(); ++it)
			m_RedListBox->AddString((*it)->name);

		for (auto it = MD->m_BlueMemberVect.begin(); it != MD->m_BlueMemberVect.end(); ++it)
			m_BlueListBox->AddString((*it)->name);

		for (int i = 0; i < (int)MD->m_AllMemberVect.size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = (MD->m_AllMemberVect)[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s((MD->m_AllMemberVect)[i]->money * MD->m_Sum, buf, 10);
			m_AllListBox->InsertString(0, (MD->m_AllMemberVect)[i]->name + _TEXT("  ") + s + CString(buf));
		}
	}
};

//改变底注命令
class ChangedMoneyCommand : public Command
{
private:

	int m_TempSum_Undo;
	int m_TempSum_Redo;

	CBursterDlg* m_MainDlg;//主窗口类

	CListBox* m_HistoryListBox;
	CListBox* m_AllListBox;
	CEdit* m_Edit;

	vector<CString>  m_PaySchemeString_Undo_Temp;
	vector<stMember> m_AllMemberVect_Undo_Temp;

	vector<CString>  m_PaySchemeString_Redo_Temp;
	vector<stMember> m_AllMemberVect_Redo_Temp;

public:

	ChangedMoneyCommand(CListBox* allListBox,
					CListBox* HistoryListBox,
					CEdit* edit,
					CBursterDlg* pDlg)
		:
		m_Edit(edit),
		m_AllListBox(allListBox),
		m_HistoryListBox(HistoryListBox),
		m_MainDlg(pDlg),
		m_TempSum_Undo(pDlg->m_Sum)
	{

		for (int i = 0; i < (int)MD->m_AllMemberVect.size(); ++i)
		{
			m_AllMemberVect_Undo_Temp.push_back(*(MD->m_AllMemberVect)[i]);
		}

		m_PaySchemeString_Undo_Temp = MD->m_PaySchemeString;
	}

	virtual	~ChangedMoneyCommand()
	{

	}

	//执行命令
	virtual void execute()
	{
		CString s;
		m_Edit->GetWindowText(s);
		MD->m_Sum = _ttoi(s);

		m_AllListBox->ResetContent();

		for (int i = 0; i < (int)MD->m_AllMemberVect.size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = (MD->m_AllMemberVect)[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s((MD->m_AllMemberVect)[i]->money * (MD->m_Sum), buf, 10);
			m_AllListBox->InsertString(0, (MD->m_AllMemberVect)[i]->name + _TEXT("  ") + s + CString(buf));
		}

		//最佳支付方案
		PayScheme_g(&MD->m_PaySchemeString, &MD->m_AllMemberVect, MD->m_Sum, m_HistoryListBox);

		//保存信息 用于重做
		m_AllMemberVect_Redo_Temp.clear();
		for (int i = 0; i < (int)MD->m_AllMemberVect.size(); ++i)
			m_AllMemberVect_Redo_Temp.push_back(*(MD->m_AllMemberVect)[i]);
		m_PaySchemeString_Redo_Temp = MD->m_PaySchemeString;
		m_TempSum_Redo = MD->m_Sum;
	}

	//重做
	virtual void redo()
	{
		for (int i = 0; i < (int)m_AllMemberVect_Redo_Temp.size(); ++i)
		{
			(MD->m_AllMemberVect)[i]->money = m_AllMemberVect_Redo_Temp[i].money;
			(MD->m_AllMemberVect)[i]->name = m_AllMemberVect_Redo_Temp[i].name;
		}

		MD->m_PaySchemeString = m_PaySchemeString_Redo_Temp;
		MD->m_Sum = m_TempSum_Redo;
		char buf[32];
		_itoa_s(MD->m_Sum, buf,32, 10);
		m_Edit->SetWindowText(_T(buf));

		m_HistoryListBox->ResetContent();
		m_AllListBox->ResetContent();

		//最佳支付方案
		PayScheme_g(&MD->m_PaySchemeString, &MD->m_AllMemberVect, MD->m_Sum, m_HistoryListBox);

		for (int i = 0; i < (int)MD->m_AllMemberVect.size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = (MD->m_AllMemberVect)[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s((MD->m_AllMemberVect)[i]->money * (MD->m_Sum), buf, 10);
			m_AllListBox->InsertString(0, (MD->m_AllMemberVect)[i]->name + _TEXT("  ") + s + CString(buf));
		}
	}

	//撤销
	virtual void undo()
	{
		for (int i = 0; i < (int)m_AllMemberVect_Undo_Temp.size(); ++i)
		{
			(MD->m_AllMemberVect)[i]->money = m_AllMemberVect_Undo_Temp[i].money;
			(MD->m_AllMemberVect)[i]->name = m_AllMemberVect_Undo_Temp[i].name;
		}

		MD->m_PaySchemeString = m_PaySchemeString_Undo_Temp;
		MD->m_Sum = m_TempSum_Undo;
		char buf[32];
		_itoa_s(MD->m_Sum, buf, 32, 10);
		m_Edit->SetWindowText(_T(buf));

		m_HistoryListBox->ResetContent();
		m_AllListBox->ResetContent();

		//最佳支付方案
		PayScheme_g(&MD->m_PaySchemeString, &MD->m_AllMemberVect, MD->m_Sum, m_HistoryListBox);

		for (int i = 0; i < (int)MD->m_AllMemberVect.size(); ++i)
		{
			char buf[32];
			CString s = _TEXT("");
			s = (MD->m_AllMemberVect)[i]->money >= 0 ? _TEXT("+") : s;
			_itoa_s((MD->m_AllMemberVect)[i]->money * (MD->m_Sum), buf, 10);
			m_AllListBox->InsertString(0, (MD->m_AllMemberVect)[i]->name + _TEXT("  ") + s + CString(buf));
		}
	}
};

///配置命令
class ConfigCommand : public Command
{
private:

	//外部数据指针
	CListBox* m_CurListBox;
	CListBox* m_AllListBox;
	CListBox* m_BlueListBox;
	CListBox* m_RedListBox;
	CListBox* m_HistoryListBox;

	CBursterDlg* m_MainDlg;//主窗口类

	//执行前的数据
	vector<stData> m_Data_Undo_Temp;
	vector<stMember> m_AllMemberVect_Undo_Temp;
	vector<stMember> m_CurMemberVect_Undo_Temp;
	vector<stMember> m_RedMemberVect_Undo_Temp;
	vector<stMember> m_BlueMemberVect_Undo_Temp;

	//执行后的数据
	vector<stData> m_Data_Redo_Temp;
	vector<stMember> m_AllMemberVect_Redo_Temp;
	vector<stMember> m_CurMemberVect_Redo_Temp;
	vector<stMember> m_BlueMemberVect_Redo_Temp;
	vector<stMember> m_RedMemberVect_Redo_Temp;

public:

	ConfigCommand(CListBox* CurListBox,
				CListBox* AllListBox,
				CListBox* BlueListBox,
				CListBox* RedListBox,
				CListBox* historyListBox,
				CBursterDlg* pDlg)
		:
		m_CurListBox(CurListBox),
		m_AllListBox(AllListBox),
		m_BlueListBox(BlueListBox),
		m_HistoryListBox(historyListBox),
		m_RedListBox(RedListBox),
		m_MainDlg(pDlg)
	{
		MemberCopy(m_AllMemberVect_Undo_Temp, MD->m_AllMemberVect);
		MemberCopy(m_CurMemberVect_Undo_Temp, MD->m_CurMemberVect);
		MemberCopy(m_BlueMemberVect_Undo_Temp, MD->m_BlueMemberVect);
		MemberCopy(m_RedMemberVect_Undo_Temp, MD->m_RedMemberVect);
	}

	virtual	~ConfigCommand()
	{
	}

	//执行命令
	virtual void execute()
	{
		// TODO: 在此添加控件通知处理程序代码
		bool save = false;
		CConfig dlg(m_AllListBox,
					&MD->m_CurMemberVect,
					&MD->m_AllMemberVect,
					&MD->m_RedMemberVect,
					&MD->m_BlueMemberVect,
					MD->m_Sum,
					save);

		dlg.DoModal();

		//修改
		if (save)
		{
			//重新刷新列表框数据
			SetListBoxData();

			//重新计算最佳支付方案
			vector<CString> pay;
			PayScheme_g(&pay, &MD->m_AllMemberVect, MD->m_Sum, m_HistoryListBox);
		}

		//记录执行命令之后的数据，用于重做
		MemberCopy(m_AllMemberVect_Redo_Temp, MD->m_AllMemberVect);
		MemberCopy(m_CurMemberVect_Redo_Temp, MD->m_CurMemberVect);
		MemberCopy(m_BlueMemberVect_Redo_Temp, MD->m_BlueMemberVect);
		MemberCopy(m_RedMemberVect_Redo_Temp, MD->m_RedMemberVect);
	}

	//设置各个列表框数据
	void SetListBoxData()
	{
		CListBox* list[] =
		{
			m_CurListBox,
			m_RedListBox,
			m_BlueListBox,
			m_AllListBox
		};

		vector<stMember*>* vect[] =
		{
			&MD->m_CurMemberVect,
			&MD->m_RedMemberVect,
			&MD->m_BlueMemberVect,
			&MD->m_AllMemberVect
		};

		for (int i = 0; i < 4; ++i)
		{
			list[i]->ResetContent();
			for (auto it = vect[i]->begin(); it != vect[i]->end(); ++it)
			{
				CString str = (*it)->name;
				CString money;
				money.Format("%d", (*it)->money * MD->m_Sum);
				if ((*it)->money >= 0)
					money = _T("+") + money;
				if (i == 3)
					str = str + _T("  ") + money;
				list[i]->AddString(str);
			}
		}
	}

	//重做
	virtual void redo()
	{
		//还原执行命令前的数据
		MemberCopy(MD->m_AllMemberVect, m_AllMemberVect_Redo_Temp);

		//还原红队列表
		MD->m_RedMemberVect.clear();
		for (int i = 0; i < m_RedMemberVect_Redo_Temp.size(); ++i)
		{
			int index = IsMemberInVect_index(m_RedMemberVect_Redo_Temp[i], MD->m_AllMemberVect);
			if (index != -1)
				MD->m_RedMemberVect.push_back((MD->m_AllMemberVect)[index]);
		}

		//还原蓝队队列表
		MD->m_BlueMemberVect.clear();
		for (int i = 0; i < m_BlueMemberVect_Redo_Temp.size(); ++i)
		{
			int index = IsMemberInVect_index(m_BlueMemberVect_Redo_Temp[i], MD->m_AllMemberVect);
			if (index != -1)
				MD->m_BlueMemberVect.push_back((MD->m_AllMemberVect)[index]);
		}

		//重新刷新列表框数据
		SetListBoxData();

		//重新计算最佳支付方案
		vector<CString> pay;
		PayScheme_g(&pay, &MD->m_AllMemberVect, MD->m_Sum, m_HistoryListBox);
	}

	//撤销
	virtual void undo()
	{
		//还原执行命令前的数据
		MemberCopy(MD->m_AllMemberVect, m_AllMemberVect_Undo_Temp);

		//还原红队列表
		MD->m_RedMemberVect.clear();
		for (int i = 0; i < m_RedMemberVect_Undo_Temp.size(); ++i)
		{
			int index = IsMemberInVect_index(m_RedMemberVect_Undo_Temp[i], MD->m_AllMemberVect);
			if (index != -1)
				MD->m_RedMemberVect.push_back((MD->m_AllMemberVect)[index]);
		}

		//还原蓝队队列表
		MD->m_BlueMemberVect.clear();
		for (int i = 0; i < m_BlueMemberVect_Undo_Temp.size(); ++i)
		{
			int index = IsMemberInVect_index(m_BlueMemberVect_Undo_Temp[i], MD->m_AllMemberVect);
			if (index != -1)
				MD->m_BlueMemberVect.push_back((MD->m_AllMemberVect)[index]);
		}

		//重新刷新列表框数据
		SetListBoxData();

		//重新计算最佳支付方案
		vector<CString> pay;
		PayScheme_g(&pay, &MD->m_AllMemberVect, MD->m_Sum, m_HistoryListBox);
	}
};
