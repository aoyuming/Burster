#pragma once
#include "stdafx.h"
#include "BursterDlg.h"
#include "Command.h"
#include "member.h"
#include "Config.h"
#include <vector>

using namespace std;

#define _MD m_MainDlg

//添加命令
class AddCommand : public Command
{
private:
	CBursterDlg* m_MainDlg;//主窗口类
	vector<CString> m_AddName;

	bool m_Undo;//是否撤回
	vector<stMember*> m_NewMember;//添加的成员 如果撤回的话 要自己delete掉

	vector<stMember*> m_AllMemberVect_Undo_Temp;
	vector<stMember*> m_CurMemberVect_Undo_Temp;
	vector<stMember*> m_DoveMemberVect_Undo_Temp;

	vector<stMember*> m_AllMemberVect_Redo_Temp;
	vector<stMember*> m_CurMemberVect_Redo_Temp;
	vector<stMember*> m_DoveMemberVect_Redo_Temp;
public:

	AddCommand(const vector<CString>& name, CBursterDlg* pDlg)
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

	AddCommand(CString name, CBursterDlg* pDlg)
		:
		m_NewMember(NULL),
		m_Undo(false),
		m_MainDlg(pDlg)
	{
		m_AddName.push_back(name);
		m_AllMemberVect_Undo_Temp = _MD->m_AllMemberVect;
		m_CurMemberVect_Undo_Temp = _MD->m_CurMemberVect;
		m_DoveMemberVect_Undo_Temp = _MD->m_DoveMemberVect;
	}

	virtual	~AddCommand()
	{
		if (m_Undo)
		{
			for (int i = 0; i < (int)m_NewMember.size(); ++i)
				delete m_NewMember[i];
			m_NewMember.clear();
		}
	}

	//执行命令
	virtual bool execute()
	{
		// TODO: 在此添加控件通知处理程序代码
		vector<stMember*> mVect;
		for (int i = 0; i < (int)m_AddName.size(); ++i)
		{
			stMember* m = new stMember;
			m->money = 0;
			m->name = m_AddName[i];

			if (m->name == _TEXT(""))
			{
				delete m;
				continue;
			}

			//判断当前成员是否在全部玩家列表中
			stMember* p = IsMemberInVect(*m, _MD->m_AllMemberVect);

			//没有重复才添加
			if (NULL == p)
			{
				_MD->m_CurListBox->AddString(m->name);
				_MD->m_AllListBox->AddString(m->name + _TEXT("  +0"));
				_MD->m_CurMemberVect.push_back(m);
				_MD->m_AllMemberVect.push_back(m);
				mVect.push_back(m);
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

				delete m;
			}
		}

		if (mVect.size() == 0)
			MessageBox(AfxGetApp()->m_pMainWnd->m_hWnd, _T("   重复添加   "), _T("提示"), MB_OK | MB_ICONSTOP);
		
		m_AllMemberVect_Redo_Temp = _MD->m_AllMemberVect;
		m_CurMemberVect_Redo_Temp = _MD->m_CurMemberVect;
		m_DoveMemberVect_Redo_Temp = _MD->m_DoveMemberVect;
		m_NewMember = mVect;//记录当前成员

		return m_NewMember.size() != 0;
	}

	//重做
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

	//撤销
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

//删除命令
class EraseCommand : public Command
{
private:

	int m_EraseIndex;
	CBursterDlg* m_MainDlg;//主窗口类

	vector<stMember*> m_CurMemberVect_Undo_Temp;
	vector<stMember*> m_DoveMemberVect_Undo_Temp;
	vector<stMember*> m_LastRedMemberVect_Undo_Temp;
	vector<stMember*> m_LastBlueMemberVect_Undo_Temp;

	vector<stMember*> m_CurMemberVect_Redo_Temp;
	vector<stMember*> m_DoveMemberVect_Redo_Temp;
	vector<stMember*> m_LastRedMemberVect_Redo_Temp;
	vector<stMember*> m_LastBlueMemberVect_Redo_Temp;
public:

	EraseCommand(int eraseIdx, CBursterDlg* pDlg)
		:
		m_EraseIndex(eraseIdx),
		m_MainDlg(pDlg)
	{
		m_CurMemberVect_Undo_Temp = _MD->m_CurMemberVect;
		m_DoveMemberVect_Undo_Temp = _MD->m_DoveMemberVect;
		m_LastRedMemberVect_Undo_Temp = _MD->m_LastRedMemberVect;
		m_LastBlueMemberVect_Undo_Temp = _MD->m_LastBlueMemberVect;
	}

	virtual	~EraseCommand()
	{

	}

	//执行命令
	virtual bool execute()
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

		_MD->m_LastBlueMemberVect.clear();
		_MD->m_LastRedMemberVect.clear();

		m_CurMemberVect_Redo_Temp = _MD->m_CurMemberVect;
		m_DoveMemberVect_Redo_Temp = _MD->m_DoveMemberVect;
		m_LastRedMemberVect_Redo_Temp = _MD->m_LastRedMemberVect;
		m_LastBlueMemberVect_Redo_Temp = _MD->m_LastBlueMemberVect;
		return true;
	}

	//重做
	virtual void redo()
	{
		_MD->m_CurMemberVect = m_CurMemberVect_Redo_Temp;
		_MD->m_DoveMemberVect = m_DoveMemberVect_Redo_Temp;
		_MD->m_LastRedMemberVect = m_LastRedMemberVect_Redo_Temp;
		_MD->m_LastBlueMemberVect = m_LastBlueMemberVect_Redo_Temp;

		_MD->m_CurListBox->ResetContent();
		for (int i = 0; i < (int)_MD->m_CurMemberVect.size(); ++i)
			_MD->m_CurListBox->AddString(_MD->m_CurMemberVect[i]->name);
	}

	//撤销
	virtual void undo()
	{
		_MD->m_CurMemberVect = m_CurMemberVect_Undo_Temp;
		_MD->m_DoveMemberVect = m_DoveMemberVect_Undo_Temp;
		_MD->m_LastRedMemberVect = m_LastRedMemberVect_Undo_Temp;
		_MD->m_LastBlueMemberVect = m_LastBlueMemberVect_Undo_Temp;

		_MD->m_CurListBox->ResetContent();
		for (int i = 0; i < (int)_MD->m_CurMemberVect.size(); ++i)
			_MD->m_CurListBox->AddString(_MD->m_CurMemberVect[i]->name);
	}
};

//分组命令
class GroupingCommand : public Command
{
public:

	//分组算法
	enum GroupingArith { ENTIRELY, AVERAGE};

private:

	GroupingArith m_Arith;//分组算法
	CBursterDlg* m_MainDlg;//主窗口类

	//执行命令前的数据
	vector<stMember*> m_CurMemberVect_Undo_Temp;
	vector<stMember*> m_BlueMemberVect_Undo_Temp;
	vector<stMember*> m_RedMemberVect_Undo_Temp;
	vector<stMember*> m_LastBlueMemberVect_Undo_Temp;
	vector<stMember*> m_LastRedMemberVect_Undo_Temp;
	vector<stData>   m_Data_Undo_Temp;

	//执行命令后的数据
	vector<stMember*> m_CurMemberVect_Redo_Temp;
	vector<stMember*> m_BlueMemberVect_Redo_Temp;
	vector<stMember*> m_RedMemberVect_Redo_Temp;
	vector<stMember*> m_LastBlueMemberVect_Redo_Temp;
	vector<stMember*> m_LastRedMemberVect_Redo_Temp;
	vector<stData>   m_Data_Redo_Temp;

public:

	GroupingCommand(CBursterDlg* pDlg, GroupingArith arith)
		:
		m_MainDlg(pDlg),
		m_Arith(arith)
	{
		m_CurMemberVect_Undo_Temp = _MD->m_CurMemberVect;
		m_BlueMemberVect_Undo_Temp = _MD->m_BlueMemberVect;
		m_RedMemberVect_Undo_Temp = _MD->m_RedMemberVect;
		m_LastBlueMemberVect_Undo_Temp = _MD->m_LastBlueMemberVect;
		m_LastRedMemberVect_Undo_Temp = _MD->m_LastRedMemberVect;
		m_Data_Undo_Temp = _MD->m_Data;
	}

	virtual	~GroupingCommand()
	{

	}

	//完全随机分组
	void entirelyRandomGrouping()
	{
		//分组
		vector<stMember*> temp = _MD->m_CurMemberVect;
		_MD->m_RedMemberVect.clear();
		_MD->m_BlueMemberVect.clear();
		_MD->m_LastRedMemberVect.clear();
		_MD->m_LastBlueMemberVect.clear();
		bool flag = false;

		while (temp.size() != 0)
		{
			int rd = rand() % temp.size();

			if (flag)
			{
				_MD->m_LastRedMemberVect.push_back(temp[rd]);
				_MD->m_RedMemberVect.push_back(temp[rd]);
				_MD->m_RedListBox->AddString(temp[rd]->name);
				flag = false;
			}
			else
			{
				_MD->m_LastBlueMemberVect.push_back(temp[rd]);
				_MD->m_BlueMemberVect.push_back(temp[rd]);
				_MD->m_BlueListBox->AddString(temp[rd]->name);
				flag = true;
			}
			temp.erase(temp.begin() + rd);
		}
	}

	//平均随机分组
	void averageRandomGrouping()
	{
		if (_MD->m_LastRedMemberVect.size() != _MD->m_LastBlueMemberVect.size() 
			|| _MD->m_LastRedMemberVect.size() == 0
			|| _MD->m_LastBlueMemberVect.size() == 0)
		{
			entirelyRandomGrouping();
			return;
		}
		
		//新增的成员
		vector<stMember*> newMember;
		for (int i = 0; i < (int)_MD->m_CurMemberVect.size(); ++i)
		{
			if (-1 != IsMemberInVect_index(*_MD->m_CurMemberVect[i], _MD->m_LastBlueMemberVect))
				continue;
			if (-1 != IsMemberInVect_index(*_MD->m_CurMemberVect[i], _MD->m_LastRedMemberVect))
				continue;

			newMember.push_back(_MD->m_CurMemberVect[i]);
		}

		//分组
		vector<int> redRandVect, blueRandVect;
		int count = (int)_MD->m_LastRedMemberVect.size() / 2;
		for (int i = 0; i < count; ++i)
		{
		RED:
			int redIndex = rand() % _MD->m_LastRedMemberVect.size();
			for (int j = 0; j < (int)redRandVect.size(); ++j)
			{
				if (redIndex == redRandVect[j])
					goto RED;
			}
		BLUE:
			int blueIndex = rand() % _MD->m_LastBlueMemberVect.size();
			for (int j = 0; j < (int)blueRandVect.size(); ++j)
			{
				if (blueIndex == blueRandVect[j])
					goto BLUE;
			}
			
			stMember* temp = _MD->m_LastRedMemberVect[redIndex];
			_MD->m_LastRedMemberVect[redIndex] = _MD->m_LastBlueMemberVect[blueIndex];
			_MD->m_LastBlueMemberVect[blueIndex] = temp;
			redRandVect.push_back(redIndex);
			blueRandVect.push_back(blueIndex);
		}

		//新增的成员分组
		bool flag = false;
		while (newMember.size() != 0)
		{
			int rd = rand() % newMember.size();

			if (flag)
			{
				_MD->m_LastRedMemberVect.push_back(newMember[rd]);
				flag = false;
			}
			else
			{
				_MD->m_LastBlueMemberVect.push_back(newMember[rd]);
				flag = true;
			}
			newMember.erase(newMember.begin() + rd);
		}

		_MD->m_RedMemberVect = _MD->m_LastRedMemberVect;
		_MD->m_BlueMemberVect = _MD->m_LastBlueMemberVect;
		for (int i = 0; i < (int)_MD->m_RedMemberVect.size(); ++i)
			_MD->m_RedListBox->AddString(_MD->m_RedMemberVect[i]->name);
		for (int i = 0; i < (int)_MD->m_BlueMemberVect.size(); ++i)
			_MD->m_BlueListBox->AddString(_MD->m_BlueMemberVect[i]->name);
	}

	//执行命令
	virtual bool execute()
	{
		_MD->m_RedListBox->ResetContent();
		_MD->m_BlueListBox->ResetContent();

		if (_MD->m_RedMemberVect.size() != 0 && _MD->m_BlueMemberVect.size() != 0)
		{
			_MD->m_LastRedMemberVect = _MD->m_RedMemberVect;
			_MD->m_LastBlueMemberVect = _MD->m_BlueMemberVect;
		}

		//完全随机分组
		if (m_Arith == ENTIRELY)
			entirelyRandomGrouping();
		else if (m_Arith == AVERAGE)//平均随机分组
			averageRandomGrouping();
		
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
		for (int i = 0; i < (int)_MD->m_RedMemberVect.size(); ++i)
			data.red.push_back(*_MD->m_RedMemberVect[i]);

		for (int i = 0; i < (int)_MD->m_BlueMemberVect.size(); ++i)
			data.blue.push_back(*_MD->m_BlueMemberVect[i]);

		_MD->m_Data.push_back(data);

		//记录当前信息
		m_CurMemberVect_Redo_Temp = _MD->m_CurMemberVect;
		m_RedMemberVect_Redo_Temp = _MD->m_RedMemberVect;
		m_BlueMemberVect_Redo_Temp = _MD->m_BlueMemberVect;
		m_LastRedMemberVect_Redo_Temp = _MD->m_LastRedMemberVect;
		m_LastBlueMemberVect_Redo_Temp = _MD->m_LastBlueMemberVect;
		m_Data_Redo_Temp = _MD->m_Data;
		return true;
	}

	//重做
	virtual void redo()
	{
		_MD->m_CurMemberVect = m_CurMemberVect_Redo_Temp;
		_MD->m_RedMemberVect = m_RedMemberVect_Redo_Temp;
		_MD->m_BlueMemberVect = m_BlueMemberVect_Redo_Temp;
		_MD->m_LastRedMemberVect = m_LastRedMemberVect_Redo_Temp;
		_MD->m_LastBlueMemberVect = m_LastBlueMemberVect_Redo_Temp;
		_MD->m_Data = m_Data_Redo_Temp;

		_MD->m_BlueListBox->ResetContent();
		_MD->m_RedListBox->ResetContent();

		for (auto it = _MD->m_RedMemberVect.begin(); it != _MD->m_RedMemberVect.end(); ++it)
			_MD->m_RedListBox->AddString((*it)->name);

		for (auto it = _MD->m_BlueMemberVect.begin(); it != _MD->m_BlueMemberVect.end(); ++it)
			_MD->m_BlueListBox->AddString((*it)->name);
	}

	//撤销
	virtual void undo()
	{
		_MD->m_CurMemberVect = m_CurMemberVect_Undo_Temp;
		_MD->m_RedMemberVect = m_RedMemberVect_Undo_Temp;
		_MD->m_BlueMemberVect = m_BlueMemberVect_Undo_Temp;
		_MD->m_LastRedMemberVect = m_LastRedMemberVect_Undo_Temp;
		_MD->m_LastBlueMemberVect = m_LastBlueMemberVect_Undo_Temp;
		_MD->m_Data = m_Data_Undo_Temp;

		_MD->m_BlueListBox->ResetContent();
		_MD->m_RedListBox->ResetContent();

		for (auto it = _MD->m_RedMemberVect.begin(); it != _MD->m_RedMemberVect.end(); ++it)
			_MD->m_RedListBox->AddString((*it)->name);

		for (auto it = _MD->m_BlueMemberVect.begin(); it != _MD->m_BlueMemberVect.end(); ++it)
			_MD->m_BlueListBox->AddString((*it)->name);
	}
};

//胜利命令
class WinCommand : public Command
{
private:

	bool m_BlueWin;//胜利队伍
	CBursterDlg* m_MainDlg;//主窗口类

	//执行命令前的数据
	vector<CString>  m_PaySchemeString_Undo_Temp;
	vector<stMember> m_AllMemberVect_Undo_Temp;
	vector<stMember*> m_BlueMemberVect_Undo_Temp;
	vector<stMember*> m_RedMemberVect_Undo_Temp;
	vector<stMember*> m_LastBlueMemberVect_Undo_Temp;
	vector<stMember*> m_LastRedMemberVect_Undo_Temp;
	vector<stData>    m_Data_Undo_Temp;

	//执行命令后的数据
	vector<CString>  m_PaySchemeString_Redo_Temp;
	vector<stMember> m_AllMemberVect_Redo_Temp;
	vector<stMember*> m_BlueMemberVect_Redo_Temp;
	vector<stMember*> m_RedMemberVect_Redo_Temp;
	vector<stMember*> m_LastBlueMemberVect_Redo_Temp;
	vector<stMember*> m_LastRedMemberVect_Redo_Temp;
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

		m_LastRedMemberVect_Undo_Temp = _MD->m_LastBlueMemberVect;
		m_LastBlueMemberVect_Undo_Temp = _MD->m_LastRedMemberVect;
		m_BlueMemberVect_Undo_Temp = _MD->m_BlueMemberVect;
		m_RedMemberVect_Undo_Temp = _MD->m_RedMemberVect;
		m_Data_Undo_Temp = _MD->m_Data;
		m_PaySchemeString_Undo_Temp = _MD->m_PaySchemeString;
	}

	virtual	~WinCommand()
	{

	}

	//执行命令
	virtual bool execute()
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

		//最佳支付方案
		PayScheme(&_MD->m_PaySchemeString, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);
		
		if (_MD->m_Data.size() > 0)
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
			_MD->m_Data[_MD->m_Data.size() - 1].vectoryTmie = buf;
			_MD->m_Data[_MD->m_Data.size() - 1].redLose = 1;
			_MD->m_Data[_MD->m_Data.size() - 1].red.clear();
			_MD->m_Data[_MD->m_Data.size() - 1].blue.clear();

			for (int i = 0; i < (int)_MD->m_RedMemberVect.size(); ++i)
				_MD->m_Data[_MD->m_Data.size() - 1].red.push_back(*_MD->m_RedMemberVect[i]);

			for (int i = 0; i < (int)_MD->m_BlueMemberVect.size(); ++i)
				_MD->m_Data[_MD->m_Data.size() - 1].blue.push_back(*_MD->m_BlueMemberVect[i]);
		}

		_MD->m_LastBlueMemberVect = _MD->m_BlueMemberVect;
		_MD->m_LastRedMemberVect = _MD->m_RedMemberVect;
		_MD->m_BlueMemberVect.clear();
		_MD->m_RedMemberVect.clear();

		//保存信息 用于重做
		m_AllMemberVect_Redo_Temp.clear();
		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
			m_AllMemberVect_Redo_Temp.push_back(*_MD->m_AllMemberVect[i]);

		m_RedMemberVect_Redo_Temp = _MD->m_RedMemberVect;
		m_BlueMemberVect_Redo_Temp = _MD->m_BlueMemberVect;
		m_PaySchemeString_Redo_Temp = _MD->m_PaySchemeString;
		m_Data_Redo_Temp = _MD->m_Data;
		m_LastRedMemberVect_Redo_Temp = _MD->m_LastBlueMemberVect;
		m_LastBlueMemberVect_Redo_Temp = _MD->m_LastRedMemberVect;

		return true;
	}

	//重做
	virtual void redo()
	{
		for (int i = 0; i < (int)m_AllMemberVect_Redo_Temp.size(); ++i)
		{
			_MD->m_AllMemberVect[i]->money = m_AllMemberVect_Redo_Temp[i].money;
			_MD->m_AllMemberVect[i]->name = m_AllMemberVect_Redo_Temp[i].name;
		}

		_MD->m_RedMemberVect = m_RedMemberVect_Redo_Temp;
		_MD->m_BlueMemberVect = m_BlueMemberVect_Redo_Temp;
		_MD->m_LastBlueMemberVect = m_LastBlueMemberVect_Redo_Temp;
		_MD->m_LastRedMemberVect = m_LastRedMemberVect_Redo_Temp;
		_MD->m_Data = m_Data_Redo_Temp;
		_MD->m_PaySchemeString = m_PaySchemeString_Redo_Temp;

		_MD->m_BlueListBox->ResetContent();
		_MD->m_RedListBox->ResetContent();
		_MD->m_PaySchemeListBox->ResetContent();
		_MD->m_AllListBox->ResetContent();

		//最佳支付方案
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

	//撤销
	virtual void undo()
	{
		for (int i = 0; i < (int)m_AllMemberVect_Undo_Temp.size(); ++i)
		{
			_MD->m_AllMemberVect[i]->money = m_AllMemberVect_Undo_Temp[i].money;
			_MD->m_AllMemberVect[i]->name = m_AllMemberVect_Undo_Temp[i].name;
		}

		_MD->m_LastRedMemberVect = m_LastRedMemberVect_Undo_Temp;
		_MD->m_LastBlueMemberVect = m_LastBlueMemberVect_Undo_Temp;
		_MD->m_RedMemberVect = m_RedMemberVect_Undo_Temp;
		_MD->m_BlueMemberVect = m_BlueMemberVect_Undo_Temp;
		_MD->m_Data = m_Data_Undo_Temp;
		_MD->m_PaySchemeString = m_PaySchemeString_Undo_Temp;

		_MD->m_BlueListBox->ResetContent();
		_MD->m_RedListBox->ResetContent();
		_MD->m_PaySchemeListBox->ResetContent();
		_MD->m_AllListBox->ResetContent();

		//最佳支付方案
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

//改变底注命令
class ChangedMoneyCommand : public Command
{
private:

	int m_TempSum_Undo;
	int m_TempSum_Redo;

	CBursterDlg* m_MainDlg;//主窗口类

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

	//执行命令
	virtual bool execute()
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

		//最佳支付方案
		PayScheme(&_MD->m_PaySchemeString, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);

		//保存信息 用于重做
		m_AllMemberVect_Redo_Temp.clear();
		for (int i = 0; i < (int)_MD->m_AllMemberVect.size(); ++i)
			m_AllMemberVect_Redo_Temp.push_back(*_MD->m_AllMemberVect[i]);

		m_PaySchemeString_Redo_Temp = _MD->m_PaySchemeString;
		m_TempSum_Redo = _MD->m_Sum;
		return true;
	}

	//重做
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

		//最佳支付方案
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

	//撤销
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

		//最佳支付方案
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

///配置命令
class ConfigCommand : public Command
{
private:

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

	ConfigCommand(CBursterDlg* pDlg)
		:
		m_MainDlg(pDlg)
	{
		MemberCopy(m_AllMemberVect_Undo_Temp, _MD->m_AllMemberVect);
		MemberCopy(m_CurMemberVect_Undo_Temp, _MD->m_CurMemberVect);
		MemberCopy(m_BlueMemberVect_Undo_Temp, _MD->m_BlueMemberVect);
		MemberCopy(m_RedMemberVect_Undo_Temp, _MD->m_RedMemberVect);
	}

	virtual	~ConfigCommand()
	{
	}

	//执行命令
	virtual bool execute()
	{
		//创建配置窗口
		bool changed = false;
		CConfig dlg(changed, m_MainDlg);
		dlg.DoModal();

		//修改
		if (changed)
		{
			//重新刷新列表框数据
			SetListBoxData();

			//重新计算最佳支付方案
			PayScheme(&_MD->m_PaySchemeString, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);
			//记录执行命令之后的数据，用于重做
			MemberCopy(m_AllMemberVect_Redo_Temp, _MD->m_AllMemberVect);
			MemberCopy(m_CurMemberVect_Redo_Temp, _MD->m_CurMemberVect);
			MemberCopy(m_BlueMemberVect_Redo_Temp, _MD->m_BlueMemberVect);
			MemberCopy(m_RedMemberVect_Redo_Temp, _MD->m_RedMemberVect);
		}
		return changed;
	}

	//设置各个列表框数据
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

	//重做
	virtual void redo()
	{
		//还原执行命令前的数据
		MemberCopy(_MD->m_AllMemberVect, m_AllMemberVect_Redo_Temp);

		//还原红队列表
		_MD->m_RedMemberVect.clear();
		for (int i = 0; i < (int)m_RedMemberVect_Redo_Temp.size(); ++i)
		{
			int index = IsMemberInVect_index(m_RedMemberVect_Redo_Temp[i], _MD->m_AllMemberVect);
			if (index != -1)
				_MD->m_RedMemberVect.push_back(_MD->m_AllMemberVect[index]);
		}

		//还原蓝队队列表
		_MD->m_BlueMemberVect.clear();
		for (int i = 0; i < (int)m_BlueMemberVect_Redo_Temp.size(); ++i)
		{
			int index = IsMemberInVect_index(m_BlueMemberVect_Redo_Temp[i], _MD->m_AllMemberVect);
			if (index != -1)
				_MD->m_BlueMemberVect.push_back(_MD->m_AllMemberVect[index]);
		}

		//重新刷新列表框数据
		SetListBoxData();

		//重新计算最佳支付方案
		PayScheme(&_MD->m_PaySchemeString, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);
	}

	//撤销
	virtual void undo()
	{
		//还原执行命令前的数据
		MemberCopy(_MD->m_AllMemberVect, m_AllMemberVect_Undo_Temp);

		//还原红队列表
		_MD->m_RedMemberVect.clear();
		for (int i = 0; i < (int)m_RedMemberVect_Undo_Temp.size(); ++i)
		{
			int index = IsMemberInVect_index(m_RedMemberVect_Undo_Temp[i], _MD->m_AllMemberVect);
			if (index != -1)
				_MD->m_RedMemberVect.push_back(_MD->m_AllMemberVect[index]);
		}

		//还原蓝队队列表
		_MD->m_BlueMemberVect.clear();
		for (int i = 0; i < (int)m_BlueMemberVect_Undo_Temp.size(); ++i)
		{
			int index = IsMemberInVect_index(m_BlueMemberVect_Undo_Temp[i], _MD->m_AllMemberVect);
			if (index != -1)
				_MD->m_BlueMemberVect.push_back(_MD->m_AllMemberVect[index]);
		}

		//重新刷新列表框数据
		SetListBoxData();

		//重新计算最佳支付方案
		PayScheme(&_MD->m_PaySchemeString, _MD->m_AllMemberVect, _MD->m_Sum, _MD->m_PaySchemeListBox);
	}
};
