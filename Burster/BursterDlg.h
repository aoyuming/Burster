
// BursterDlg.h : 头文件

#pragma once
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <time.h>
#include "addName.h"
#include "SelectDlg.h"
#include "member.h"
#include "agreement.h"

class CConfig;

//下载远程文件 下载线程
UINT downRemoteFile(LPVOID lpParam);

// CBursterDlg 对话框
class CBursterDlg : public CDialogEx
{
// 构造
public:
	CBursterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	addName* m_addNameDlg;

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	friend class AddCommand;
	friend class EraseCommand;
	friend class GroupingCommand;
	friend class WinCommand;
	friend class ChangedMoneyCommand;
	friend class ConfigCommand;
	friend class CConfig;

	CWinThread*	m_pThread;

	bool m_isManualUpdate;//是否手动更新
	//检查更新
	bool inspectUpdate();

private:

	//版本号
	int m_Version[3];
	
	//赌金
	int m_Sum;

	//上一次更新时间
	time_t m_LastTime;

	CListBox* m_CurListBox;
	CListBox* m_RedListBox;
	CListBox* m_BlueListBox;
	CListBox* m_AllListBox;
	CListBox* m_PaySchemeListBox;//最佳支付方案

	CEdit* m_SumEdit;
	CFile m_FlagFile;

	UpdateMode m_LiveUpdateMode;//检查更新的方式

	//当前成员列表
	vector<stMember*> m_CurMemberVect;

	//所有成员表
	vector<stMember*> m_AllMemberVect;

	//蓝队表
	vector<stMember*> m_BlueMemberVect;

	//红队表
	vector<stMember*> m_RedMemberVect;

	//鸽子表
	vector<stMember*> m_DoveMemberVect;

	//支付方案
	vector<CString> m_PaySchemeString;

	//对战信息
	vector<stData> m_Data;

	//保存
	bool Save(const char* fn,const char* rb);

	//保存配置文件
	void SaveConfiguration();

	//加载配置文件
	bool LoadConfiguration();

	//比较版本信息 
	int compareVersion(int v1, int v2, int v3);

	//界面美化
	void UiBeautify();

	BOOL CBursterDlg::EndThread();
	BOOL CBursterDlg::ForceTerminate();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton8_Add();
	afx_msg void OnBnClickedButton2_Delete();
	afx_msg void OnBnClickedButton3_Clear();
	afx_msg void OnBnClickedButton1_Save();
	afx_msg void OnBnClickedButton4_Separate();
	afx_msg void OnBnClickedButton6_Change();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton5_Blue();
	afx_msg void OnBnClickedButton7_Red();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton11();
	afx_msg virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void On32771();
	afx_msg void On32775();
};
