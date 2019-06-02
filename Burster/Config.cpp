// Config.cpp : 实现文件
//

#include "stdafx.h"
#include "Burster.h"
#include "Config.h"
#include "afxdialogex.h"
#include "addName.h"
#include "SelectDlg.h"
#include "EditMember.h"


// CConfig 对话框

IMPLEMENT_DYNAMIC(CConfig, CDialogEx)

CConfig::CConfig(CListBox* all_ext,
				vector<stMember*>* cur,
				vector<stMember*>* all,
				vector<stMember*>* red,
				vector<stMember*>* blue, 
				int sum,
				bool& saveFlag,
				CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTING, pParent),
	m_AllListBoxExternal(all_ext),
	m_CurMemberExterior(cur),
	m_AllMemberExterior(all),
	m_RedMemberExterior(red),
	m_BlueMemberExterior(blue),
	m_Sum(sum),
	m_SaveFlag(saveFlag)
{
	for (int i = 0; i < (int)cur->size(); ++i)
		m_CurMember_Temp.push_back(*((*cur)[i]));
	for (int i = 0; i < (int)all->size(); ++i)
		m_AllMember_Temp.push_back(*((*all)[i]));
	for (int i = 0; i < (int)red->size(); ++i)
		m_RedMember_Temp.push_back(*((*red)[i]));
	for (int i = 0; i < (int)blue->size(); ++i)
		m_BlueMember_Temp.push_back(*((*blue)[i]));
	for (int i = 0; i < (int)cur->size(); ++i)
		m_RemainMember_Temp.push_back(*((*cur)[i]));
}

CConfig::~CConfig()
{
}

void CConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST5, m_History_LC);
}


BEGIN_MESSAGE_MAP(CConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON8, &CConfig::OnBnClickedButton_RedAdd)
	ON_BN_CLICKED(IDC_BUTTON10, &CConfig::OnBnClickedButtonBlueAdd)
	ON_BN_CLICKED(IDC_BUTTON2, &CConfig::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CConfig::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3, &CConfig::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON12, &CConfig::OnBnClickedButton12)
	ON_NOTIFY(NM_CLICK, IDC_LIST5, &CConfig::OnNMClickList5)
	ON_BN_CLICKED(IDC_BUTTON13, &CConfig::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON1, &CConfig::OnBnClickedButton1)
END_MESSAGE_MAP()

//初始化
BOOL CConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_RedListBox = (CListBox*)GetDlgItem(IDC_LIST4);
	m_BlueListBox = (CListBox*)GetDlgItem(IDC_LIST3);

	for (int i = 0; i < (int)m_RedMemberExterior->size(); ++i)
	{
		m_RedListBox->AddString((*m_RedMemberExterior)[i]->name);
		m_EraseMember.push_back(*(*m_RedMemberExterior)[i]);
		int index = find((*m_RedMemberExterior)[i]->name, m_RemainMember_Temp);
		if (index != -1)
			m_RemainMember_Temp.erase(m_RemainMember_Temp.begin() + index);
	}

	for (int i = 0; i < (int)m_BlueMemberExterior->size(); ++i)
	{
		m_BlueListBox->AddString((*m_BlueMemberExterior)[i]->name);
		m_EraseMember.push_back(*(*m_BlueMemberExterior)[i]);
		int index = find((*m_BlueMemberExterior)[i]->name, m_RemainMember_Temp);
		if (index != -1)
			m_RemainMember_Temp.erase(m_RemainMember_Temp.begin() + index);
	}


	LONG lStyle;
	lStyle = GetWindowLong(m_History_LC.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_History_LC.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_History_LC.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_History_LC.SetExtendedStyle(dwStyle); //设置扩展风格

	CRect rect;
	m_History_LC.GetWindowRect(&rect);
	m_History_LC.InsertColumn(0, _T("名字"), LVCFMT_LEFT, rect.Width() / 2);
	m_History_LC.InsertColumn(1, _T("金钱"), LVCFMT_LEFT, rect.Width() / 2 - 3);

	for (int i = 0; i < (int)m_AllMemberExterior->size(); ++i)
	{
		CString money, flag("");
		money.Format(_T("%d"), (*m_AllMemberExterior)[i]->money * m_Sum);
		int row = m_History_LC.InsertItem(i, (*m_AllMemberExterior)[i]->name);
		if ((*m_AllMemberExterior)[i]->money >= 0)
			flag = "+";
		m_History_LC.SetItemText(row, 1, flag + money);
	}

	return TRUE;
}

int CConfig::find(CString name, const vector<stMember>& vect)
{
	for (int i = 0; i < (int)vect.size(); ++i)
	{
		if (vect[i].name == name)
			return i;
	}
	return -1;
}

int CConfig::find(CString name, const vector<stMember*>* vect)
{
	for (int i = 0; i < (int)vect->size(); ++i)
	{
		if ((*vect)[i]->name == name)
			return i;
	}
	return -1;
}

// CConfig 消息处理程序
//红队添加
void CConfig::OnBnClickedButton_RedAdd()
{
	CString selectStr;
	int selectIndex = -1;
	CSelectDlg dlg(m_RemainMember_Temp, selectIndex, &selectStr,  _T("请选择要添加的成员"), _T("添加"));
	dlg.DoModal();

	if (selectIndex == -1)
		return;

	int index1 = find(selectStr, m_RemainMember_Temp);
	m_RedListBox->AddString(m_RemainMember_Temp[index1].name);
	m_EraseMember.push_back(m_RemainMember_Temp[index1]);
	m_RedMember_Temp.push_back(m_RemainMember_Temp[index1]);
	m_RemainMember_Temp.erase(m_RemainMember_Temp.begin() + index1);
}

//蓝队添加
void CConfig::OnBnClickedButtonBlueAdd()
{
	CString selectStr;
	int selectIndex = -1;
	CSelectDlg dlg(m_RemainMember_Temp, selectIndex, &selectStr, _T("请选择要添加的成员"), _T("添加"));
	dlg.DoModal();

	if (selectIndex == -1)
		return;

	int index1 = find(selectStr, m_RemainMember_Temp);
	m_BlueListBox->AddString(m_RemainMember_Temp[index1].name);
	m_EraseMember.push_back(m_RemainMember_Temp[index1]);
	m_BlueMember_Temp.push_back(m_RemainMember_Temp[index1]);
	m_RemainMember_Temp.erase(m_RemainMember_Temp.begin() + index1);
}

//红队删除
void CConfig::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	int selectIndex = -1;
	CSelectDlg dlg(m_RedMember_Temp, selectIndex, NULL, _T("请选择要删除的成员"), _T("删除"));
	dlg.DoModal();

	if (selectIndex == -1)
		return;

	CString selectStr;
	m_RedListBox->GetText(selectIndex, selectStr);
	m_RedListBox->DeleteString(selectIndex);

	int index1 = find(selectStr, m_EraseMember);
	m_RemainMember_Temp.push_back(m_EraseMember[index1]);
	m_EraseMember.erase(m_EraseMember.begin() + index1);
	
	int index2 = find(selectStr, m_RedMember_Temp);
	m_RedMember_Temp.erase(m_RedMember_Temp.begin() + index2);
}

//蓝队删除
void CConfig::OnBnClickedButton5()
{
	int selectIndex = -1;
	CSelectDlg dlg(m_BlueMember_Temp, selectIndex, NULL, _T("请选择要删除的成员"), _T("删除"));
	dlg.DoModal();

	if (selectIndex == -1)
		return;

	CString selectStr;
	m_BlueListBox->GetText(selectIndex, selectStr);
	m_BlueListBox->DeleteString(selectIndex);

	int index1 = find(selectStr, m_EraseMember);
	m_RemainMember_Temp.push_back(m_EraseMember[index1]);
	m_EraseMember.erase(m_EraseMember.begin() + index1);

	int index2 = find(selectStr, m_BlueMember_Temp);
	m_BlueMember_Temp.erase(m_BlueMember_Temp.begin() + index2);
}

//红队清空
void CConfig::OnBnClickedButton3()
{
	for (int i = 0; i < (int)m_RedMember_Temp.size(); ++i)
		m_RemainMember_Temp.push_back(m_RedMember_Temp[i]);
	m_RedMember_Temp.clear();
	m_RedListBox->ResetContent();
}

//蓝队清空
void CConfig::OnBnClickedButton12()
{
	for (int i = 0; i < (int)m_BlueMember_Temp.size(); ++i)
		m_RemainMember_Temp.push_back(m_BlueMember_Temp[i]);
	m_BlueMember_Temp.clear();
	m_BlueListBox->ResetContent();
}

//编辑
void CConfig::OnNMClickList5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//得到选中的行
	int rank = m_History_LC.GetSelectionMark();
	CString name = m_History_LC.GetItemText(rank, 0);
	CString money = m_History_LC.GetItemText(rank, 1);
	CString newName = name;

	//创建设置成员信息对话框
	CEditMember dlg(newName, money, m_Sum);
	dlg.DoModal();

	for (int i = 0; i < (int)m_AllMember_Temp.size(); ++i)
	{
		if (newName == m_AllMember_Temp[i].name && i != rank)
		{
			MessageBox(_T("已存在名字"), _T("提示"), MB_OK);
			OnNMClickList5(pNMHDR, pResult);
			return;
		}
	}

	//修改名字
	if (newName != name)
	{
		//修改红队列表名字
		int index = find(name, m_RedMember_Temp);
		if (index != -1)
		{
			m_RedMember_Temp[index].name = newName;
			index = m_RedListBox->FindString(0, name);
			m_RedListBox->DeleteString(index);
			m_RedListBox->AddString(newName);
		}

		//修改蓝队列表名字
		index = find(name, m_BlueMember_Temp);
		if (index != -1)
		{
			m_BlueMember_Temp[index].name = newName;
			index = m_BlueListBox->FindString(0, name);
			m_BlueListBox->DeleteString(index);
			m_BlueListBox->AddString(newName);
		}

		//修改外部待分组列表名字
		index = find(name, m_CurMember_Temp);
		if (index != -1)
		{
			m_CurMember_Temp[index].name = newName;
		}

		//修改历史记录里面的名字
		m_History_LC.SetItemText(rank, 0, newName);
		m_AllMember_Temp[rank].name = newName;
	}

	//修改金币
	m_History_LC.SetItemText(rank, 1, money);
	m_AllMember_Temp[rank].money = _ttoi(money) / m_Sum;
}

//保存
void CConfig::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_RedMember_Temp.size() != m_BlueMember_Temp.size())
	{
		MessageBox(_T("人员不平均"), _T("提示"), MB_OK);
		return;
	}

	//修改外部成员 为新设置的属性
	m_SaveFlag = true;
	for (int i = 0; i < (int)m_AllMember_Temp.size(); ++i)
		*(*m_AllMemberExterior)[i] = m_AllMember_Temp[i];

	m_RedMemberExterior->clear();
	for (int i = 0; i < (int)m_RedMember_Temp.size(); ++i)
	{
		int index = find(m_RedMember_Temp[i].name, m_AllMemberExterior);
		m_RedMemberExterior->push_back((*m_AllMemberExterior)[index]);
	}

	m_BlueMemberExterior->clear();
	for (int i = 0; i < (int)m_BlueMember_Temp.size(); ++i)
	{
		int index = find(m_BlueMember_Temp[i].name, m_AllMemberExterior);
		m_BlueMemberExterior->push_back((*m_AllMemberExterior)[index]);
	}

	SendMessage(WM_CLOSE);
}

//取消
void CConfig::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	SendMessage(WM_CLOSE);
}
