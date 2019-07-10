// VersionInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Burster.h"
#include "VersionInfo.h"
#include "afxdialogex.h"


// CVersionInfo �Ի���

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


// CVersionInfo ��Ϣ�������


BOOL CVersionInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_VerInfoEdit.SetWindowText(m_VerInfo);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//����
void CVersionInfo::OnBnClickedButton1()
{
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	path = path.Left(pos);

	//д�뱾����Ϣ
	CString data = _T("Զ�̷�������ַ=http://129.226.48.122/burster/Burster.exe");
	FILE* pf = NULL;
	fopen_s(&pf, path + _T('\\') + _T("version.txt"), "wb");
	if (!pf)
		return;
	fwrite(data.GetBuffer(), data.GetLength(), 1, pf);
	fclose(pf);

	//�򿪸��³���
	CString livePath = path + _T('\\') + LOCAL_LIVE_UPDATE;
	SHELLEXECUTEINFO ShExecInfo;

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = _T("open");
	//����Ҫ���õ�exe�ļ�·��
	ShExecInfo.lpFile = livePath;
	//���������в�������
	ShExecInfo.lpParameters = _T("fzq_update"); //��û�������в�������ΪNULL
	ShExecInfo.lpDirectory = NULL;//����exe��Ŀ¼�ɺ��ԣ�дΪNULL
	ShExecInfo.nShow = SW_SHOWDEFAULT;//��������Ϊ����ʾexe���棬������ΪSW_SHOW���������ʾexe����
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);

	//�رձ�����
	exit(0);
}

//ȡ��
void CVersionInfo::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendMessage(WM_CLOSE);
}
