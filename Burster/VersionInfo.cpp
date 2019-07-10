// VersionInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "Burster.h"
#include "VersionInfo.h"
#include "afxdialogex.h"


// CVersionInfo 对话框

IMPLEMENT_DYNAMIC(CVersionInfo, CDialog)

CVersionInfo::CVersionInfo(CString verInfo, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_VERSION_INFO, pParent),
	m_VerInfo(verInfo)
{

}

CVersionInfo::~CVersionInfo()
{
}

void CVersionInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT4, m_VerInfoEdit);
}


BEGIN_MESSAGE_MAP(CVersionInfo, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CVersionInfo::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CVersionInfo::OnBnClickedButton2)
END_MESSAGE_MAP()


// CVersionInfo 消息处理程序


BOOL CVersionInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_VerInfoEdit.SetWindowText(m_VerInfo);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//更新
void CVersionInfo::OnBnClickedButton1()
{
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	path = path.Left(pos);

	//写入本地信息
	CString data = _T("远程分组器地址=http://129.226.48.122/burster/Burster.exe");
	FILE* pf = NULL;
	fopen_s(&pf, path + _T('\\') + _T("version.txt"), "wb");
	if (!pf)
		return;
	fwrite(data.GetBuffer(), data.GetLength(), 1, pf);
	fclose(pf);

	//打开更新程序
	CString livePath = path + _T('\\') + LOCAL_LIVE_UPDATE;
	SHELLEXECUTEINFO ShExecInfo;

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = _T("open");
	//输入要调用的exe文件路径
	ShExecInfo.lpFile = livePath;
	//传入命令行参数数据
	ShExecInfo.lpParameters = _T("fzq_update"); //若没有命令行参数，可为NULL
	ShExecInfo.lpDirectory = NULL;//这里exe的目录可忽略，写为NULL
	ShExecInfo.nShow = SW_SHOWDEFAULT;//这里设置为不显示exe界面，若设置为SW_SHOW，则可以显示exe界面
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);

	//关闭本程序
	exit(0);
}

//取消
void CVersionInfo::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	SendMessage(WM_CLOSE);
}
