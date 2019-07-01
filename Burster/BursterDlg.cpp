
// BursterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Burster.h"
#include "BursterDlg.h"
#include "afxdialogex.h"
#include "projectCommand.h"
#include "Config.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString REMOTE_VERSION_URL = _T("http://129.226.48.122/burster/version.txt");
const CString LOCAL_LIVE_UPDATE = _T("liveUpdate.exe");
const CString TEMP_PATH = _T("c:\\temp");
const CString REMOTE_LIVE_UPDATE = _T("http://129.226.48.122/burster/liveUpdate.exe");

// CBursterDlg 对话框

CBursterDlg::CBursterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBursterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBursterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBursterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON8, &CBursterDlg::OnBnClickedButton8_Add)
	ON_BN_CLICKED(IDC_BUTTON2, &CBursterDlg::OnBnClickedButton2_Delete)
	ON_BN_CLICKED(IDC_BUTTON3, &CBursterDlg::OnBnClickedButton3_Clear)
	ON_BN_CLICKED(IDC_BUTTON1, &CBursterDlg::OnBnClickedButton1_Save)
	ON_BN_CLICKED(IDC_BUTTON4, &CBursterDlg::OnBnClickedButton4_Separate)
	ON_BN_CLICKED(IDC_BUTTON6, &CBursterDlg::OnBnClickedButton6_Change)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON5, &CBursterDlg::OnBnClickedButton5_Blue)
	ON_BN_CLICKED(IDC_BUTTON7, &CBursterDlg::OnBnClickedButton7_Red)
	ON_BN_CLICKED(IDC_BUTTON9, &CBursterDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CBursterDlg::OnBnClickedButton10)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON11, &CBursterDlg::OnBnClickedButton11)
END_MESSAGE_MAP()


// CBursterDlg 消息处理程序

BOOL CBursterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	::CreateMutex(NULL, TRUE, "4v4分组器");//字符串里面的内容可以随便改.他只是一个名字
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AfxMessageBox("你已经打开了该程序");
		exit(0);
		return false;
	}

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	srand((unsigned int)time(0));

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_Version[0] = 1;
	m_Version[1] = 2;
	m_Version[2] = 5;

	CString ver;
	ver.Format("%d.%d.%d", m_Version[0], m_Version[1], m_Version[2]);
	m_Sum = 100;
	m_SumEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	m_SumEdit->SetWindowText(CString(_TEXT("100")));
	CString title = _TEXT("分组器 - v");
	title += ver + _T("  斗鱼王大枪制作");
	SetWindowText(title);

	LoadConfiguration();//加载配置文件
	m_CurListBox = (CListBox*)GetDlgItem(IDC_LIST2);
	m_RedListBox = (CListBox*)GetDlgItem(IDC_LIST4);
	m_BlueListBox = (CListBox*)GetDlgItem(IDC_LIST3);
	m_AllListBox = (CListBox*)GetDlgItem(IDC_LIST5);
	m_PaySchemeListBox = (CListBox*)GetDlgItem(IDC_LIST6);//最佳支付方案

	m_RedListBox->ResetContent();
	m_BlueListBox->ResetContent();

	for (int i = 0; i < (int)m_RedMemberVect.size(); ++i)
		m_RedListBox->AddString(m_RedMemberVect[i]->name);
	for (int i = 0; i < (int)m_BlueMemberVect.size(); ++i)
		m_BlueListBox->AddString(m_BlueMemberVect[i]->name);

	for (int i = 0; i < (int)m_AllMemberVect.size(); ++i)
	{
		char buf[32];
		_itoa_s(m_AllMemberVect[i]->money * m_Sum, buf, 10);

		if (m_AllMemberVect[i]->money >= 0)
			m_AllListBox->AddString(m_AllMemberVect[i]->name + "  +" + buf);
		else
			m_AllListBox->AddString(m_AllMemberVect[i]->name + "  " + buf);
	}

	for (int i = 0; i < (int)m_CurMemberVect.size(); ++i)
		m_CurListBox->AddString(m_CurMemberVect[i]->name);

	//最佳支付方案
	PayScheme(&m_PaySchemeString, m_AllMemberVect, m_Sum, m_PaySchemeListBox);

	SetTimer(1, 33, NULL);
	SetTimer(2, 2000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//比较版本信息 
//return < 0 更新
//return == 0 不用更新
//return > 0 不用更新
int CBursterDlg::compareVersion(int v1, int v2, int v3)
{
	int version1 = m_Version[0] * 100 + m_Version[1] * 10 + m_Version[2];
	int version2 = v1 * 100 + v2 * 10 + v3;
	return version1 - version2;
}

//下载远程文件 下载线程
UINT downRemoteFile(LPVOID lpParam)
{
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	path = path.Left(pos);

	//更新程序是否存在
	if (!PathFileExists(path + _T('\\') + LOCAL_LIVE_UPDATE))
	{
		//下载远程更新程序
		CString szUrl = REMOTE_LIVE_UPDATE, rdStr;
		rdStr.Format(_T("?abc=%d"), time(NULL)); // 生成随机URL
		szUrl += rdStr;
		CString localPath = path + _T('\\') + LOCAL_LIVE_UPDATE;
		HRESULT ret = URLDownloadToFile(NULL, szUrl, localPath, 0, NULL);

		if (S_OK != ret)//下载出错
			return -1;
	}

	//检测更新
	CBursterDlg* dlg = (CBursterDlg*)(AfxGetApp()->GetMainWnd());
	dlg->inspectUpdate();

	return 0;
}

//检查更新
bool CBursterDlg::inspectUpdate()
{
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	path = path.Left(pos);

	CString szUrl = REMOTE_VERSION_URL, rdStr;
	rdStr.Format(_T("?abc=%d"), time(NULL)); // 生成随机URL
	szUrl += rdStr;
	CString localPath = TEMP_PATH;
	HRESULT ret = URLDownloadToFile(NULL, szUrl, localPath, 0, NULL);

	if (S_OK != ret)//下载出错
		return false;

	FILE* pf = NULL;
	fopen_s(&pf, localPath, "rb");

	if (NULL == pf)
		return false;

	//版本号
	int v1, v2, v3;
	fscanf_s(pf, "版本%d.%d.%d", &v1, &v2, &v3);
	fclose(pf);

	//判断是否更新
	if (compareVersion(v1, v2, v3) >= 0)
		return false;

	CFile file;
	if (file.Open(localPath, CFile::modeRead))
	{
		int length = file.GetLength();
		char* buf = new char[length + 1];
		file.Read(buf, length + 1);
		file.Close();
		buf[length] = 0;
		MessageBox(buf, _T("发现更新"), MB_OK);
		delete []buf;

		////打开更新文件
		//WinExec(LIVE_UPDATE_NAME, WM_SHOWWINDOW);

		//写入本地信息
		CString data = _T("远程分组器地址=http://129.226.48.122/burster/Burster.exe");
		CFile file;
		if (file.Open(path + _T('\\') + _T("version.txt"), CFile::modeCreate | CFile::modeWrite))
		{
			file.Write(data.GetBuffer(), data.GetLength());
			file.Close();
		}

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
		ShExecInfo.lpParameters = _T("fzq_com"); //若没有命令行参数，可为NULL

		ShExecInfo.lpDirectory = NULL;//这里exe的目录可忽略，写为NULL
		ShExecInfo.nShow = SW_SHOWDEFAULT;//这里设置为不显示exe界面，若设置为SW_SHOW，则可以显示exe界面
		ShExecInfo.hInstApp = NULL;
		ShellExecuteEx(&ShExecInfo);

		//关闭本程序
		SendMessage(WM_CLOSE);
	}

	return true;
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBursterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBursterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//保存配置文件
void CBursterDlg::SaveConfiguration()
{
	FILE* pf = NULL;
	fopen_s(&pf, "config.txt", "wb");

	char buf[32];
	_itoa_s(m_AllMemberVect.size(), buf, 10);

	//全部的人数
	CString Num = _T("全部人数=") + CString(buf);
	fwrite(Num.GetBuffer(), Num.GetLength(), 1, pf);

	//写入全部成员的信息
	for (int i = 0; i < (int)m_AllMemberVect.size(); ++i)
	{
		_itoa_s(m_AllMemberVect[i]->money, buf, 10);
		CString s = _T("\r\n姓名=") + m_AllMemberVect[i]->name;
		fwrite(s.GetBuffer(), s.GetLength(), 1, pf);
		s = +_T("\r\n金钱=") + CString(buf);
		fwrite(s.GetBuffer(), s.GetLength(), 1, pf);
	}

	fwrite("\r\n", strlen("\r\n"), 1, pf);

	//写入鸽子人数
	_itoa_s(m_DoveMemberVect.size(), buf, 10);
	Num = _T("\r\n鸽子人数=") + CString(buf);
	fwrite(Num.GetBuffer(), Num.GetLength(), 1, pf);

	//写入鸽子成员的信息
	for (int i = 0; i < (int)m_DoveMemberVect.size(); ++i)
	{
		_itoa_s(m_DoveMemberVect[i]->money, buf, 10);
		CString s = _T("\r\n姓名=") + m_DoveMemberVect[i]->name;
		fwrite(s.GetBuffer(), s.GetLength(), 1, pf);
	}

	fwrite("\r\n", strlen("\r\n"), 1, pf);

	_itoa_s(m_RedMemberVect.size(), buf, 10);
	Num = _T("\r\n红队人数=") + CString(buf);
	fwrite(Num.GetBuffer(), Num.GetLength(), 1, pf);

	//写入红队人数
	if (m_RedMemberVect.size() != 0)
	{
		for (int i = 0; i < (int)m_RedMemberVect.size(); ++i)
		{
			_itoa_s(m_RedMemberVect[i]->money, buf, 10);
			CString s = _T("\r\n姓名=") + m_RedMemberVect[i]->name;
			fwrite(s.GetBuffer(), s.GetLength(), 1, pf);
		}
	}

	fwrite("\r\n", strlen("\r\n"), 1, pf);

	_itoa_s(m_BlueMemberVect.size(), buf, 10);
	Num = _T("\r\n蓝队人数=") + CString(buf);
	fwrite(Num.GetBuffer(), Num.GetLength(), 1, pf);

	//写入蓝队人数
	if (m_BlueMemberVect.size() != 0)
	{
		for (int i = 0; i < (int)m_BlueMemberVect.size(); ++i)
		{
			_itoa_s(m_BlueMemberVect[i]->money, buf, 10);
			CString s = _T("\r\n姓名=") + m_BlueMemberVect[i]->name;
			fwrite(s.GetBuffer(), s.GetLength(), 1, pf);
		}
	}
	fwrite("\r\n", strlen("\r\n"), 1, pf);
	fwrite("\r\n", strlen("\r\n"), 1, pf);

	fclose(pf);
}

//加载配置文件
bool CBursterDlg::LoadConfiguration()
{
	FILE* pf = NULL;
	fopen_s(&pf, "config.txt", "rb");

	if (NULL == pf)
		return false;

	//全部人数
	int allNum = 0;
	fscanf_s(pf, "全部人数=%d", &allNum);

	char buf[64];
	for (int i = 0; i < allNum; ++i)
	{
		stMember* p = new stMember;
		fscanf_s(pf, "\r\n姓名=%s", buf, 32);
		fscanf_s(pf, "\r\n金钱=%d", &p->money);
		p->name = buf;
		m_AllMemberVect.push_back(p);
		m_CurMemberVect.push_back(p);
	}

	//鸽子人数
	allNum = 0;
	fscanf_s(pf, "\r\n鸽子人数=%d", &allNum);

	for (int i = 0; i < allNum; ++i)
	{
		fscanf_s(pf, "\r\n姓名=%s", buf, 32);

		for (int j = 0; j < (int)m_CurMemberVect.size(); ++j)
		{
			if (buf == m_CurMemberVect[j]->name)
			{
				m_DoveMemberVect.push_back(m_CurMemberVect[j]);
				m_CurMemberVect.erase(m_CurMemberVect.begin() + j);
				break;
			}
		}
	}

	//红队人数
	allNum = 0;
	fscanf_s(pf, "\r\n红队人数=%d", &allNum);

	for (int i = 0; i < allNum; ++i)
	{
		fscanf_s(pf, "\r\n姓名=%s", buf, 32);

		for (int j = 0; j < (int)m_CurMemberVect.size(); ++j)
		{
			if (buf == m_CurMemberVect[j]->name)
			{
				m_RedMemberVect.push_back(m_CurMemberVect[j]);
				break;
			}
		}
	}

	//蓝队人数
	allNum = 0;
	fscanf_s(pf, "\r\n蓝队人数=%d", &allNum);

	for (int i = 0; i < allNum; ++i)
	{
		fscanf_s(pf, "\r\n姓名=%s", buf, 32);

		for (int j = 0; j < (int)m_CurMemberVect.size(); ++j)
		{
			if (buf == m_CurMemberVect[j]->name)
			{
				m_BlueMemberVect.push_back(m_CurMemberVect[j]);
				break;
			}
		}
	}

	//加载信息
	allNum = 0;
	fscanf_s(pf, "\r\n详细信息%d", &allNum);
	for (int i = 0; i < allNum; ++i)
	{
		stData d;
		fscanf_s(pf, "\r\n分组时间:%s", buf, 64);
		d.fenZutime = buf;

		int redNum = 0, blueNum = 0;
		fscanf_s(pf, "\r\n红队%d", &redNum);

		for (int j = 0; j < redNum; ++j)
		{
			stMember m;
			fscanf_s(pf, "\r\n  %s  %d", buf, 64, &m.money);
			m.money /= m_Sum;
			m.name = buf;
			d.red.push_back(m);
		}

		fscanf_s(pf, "\r\n蓝队%d", &blueNum);

		for (int j = 0; j < blueNum; ++j)
		{
			stMember m;
			fscanf_s(pf, "\r\n  %s  %d", buf, 64, &m.money);
			m.money /= m_Sum;
			m.name = buf;
			d.blue.push_back(m);
		}
		fscanf_s(pf, "\r\n胜利队伍:%s", buf, 64);
		if (strcmp(buf, "未结束") == 0)
			d.redLose = 2;
		else if (strcmp(buf, "红队") == 0)
			d.redLose = 0;
		else
			d.redLose = 1;

		fscanf_s(pf, "\r\n胜利时间:%s", buf, 64);
		if (strcmp(buf, "未结束") == 0)
			d.redLose = 2;
		else
			d.vectoryTmie = buf;

		m_Data.push_back(d);
	}

	fclose(pf);

	return true;
}

//添加
void CBursterDlg::OnBnClickedButton8_Add()
{
	//创建添加窗口
	CString name;
	addName addDlg(name, this);
	addDlg.DoModal();

	if (name == _T(""))
		return;

	//创建添加成员命令
	AddCommand* com = new AddCommand(name, this);

	//执行命令 返回成功就放入命令管理器里面 否则删除自己
	if (com->execute())
		CommandManager::getInstance()->StoreCommand(com);
	else
		delete com;

	//递归添加
	OnBnClickedButton8_Add();
}

//删除
void CBursterDlg::OnBnClickedButton2_Delete()
{
	// TODO: 在此添加控件通知处理程序代码
	CListBox* listbox = (CListBox*)GetDlgItem(IDC_LIST2);
	int index = -1;
	CSelectDlg dlg(m_CurMemberVect, index, NULL, _T("请选择要删除的成员"), _T("删除"));
	dlg.DoModal();

	if (index == -1)
		return;

	//创建添加成员命令
	EraseCommand* com = new EraseCommand(index, this);

	//执行命令 返回成功就放入命令管理器里面 否则删除自己
	if (com->execute())
		CommandManager::getInstance()->StoreCommand(com);
	else
		delete com;
}

//清空
void CBursterDlg::OnBnClickedButton3_Clear()
{
	//提示
	if (MessageBox(_TEXT("确定清空吗？  此操作不可撤回！"), _TEXT("提示"), MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		return;

	//列表框
	CListBox* list[] =
	{
		m_AllListBox,
		m_CurListBox,
		m_RedListBox,
		m_BlueListBox,
		m_PaySchemeListBox
	};

	//清空
	for (int i = 0; i < 5; ++i)
		list[i]->ResetContent();

	//释放堆内存
	for (int i = 0; i < (int)m_AllMemberVect.size(); ++i)
		delete m_AllMemberVect[i];

	m_AllMemberVect.clear();
	m_CurMemberVect.clear();
	m_RedMemberVect.clear();
	m_BlueMemberVect.clear();
	m_Data.clear();
	m_DoveMemberVect.clear();

	//释放
	CommandManager::getInstance()->release();
}

//保存
void CBursterDlg::OnBnClickedButton1_Save()
{
	Save("战绩情况.txt", "wb");
}

//保存
void CBursterDlg::Save(const char* fn, const char* rb)
{
	if (m_Data.size() == 0)
		return;

	FILE* pf = 0;
	char buf[64];
	fopen_s(&pf, fn, rb);

	sprintf_s(buf, "详细信息%d", m_Data.size());
	fwrite(buf, strlen(buf), 1, pf);
	fwrite("\r\n", strlen("\r\n"), 1, pf);
	//写入信息
	for (int i = 0; i < (int)m_Data.size(); ++i)
	{
		fwrite("\r\n", strlen("\r\n"), 1, pf);
		fwrite("\r\n", strlen("\r\n"), 1, pf);
		fwrite("分组时间: ", strlen("分组时间: "), 1, pf);
		fwrite(m_Data[i].fenZutime.GetBuffer(), m_Data[i].fenZutime.GetLength(), 1, pf);
		fwrite("\r\n", strlen("\r\n"), 1, pf);

		sprintf_s(buf, "红队%d", m_Data[i].red.size());
		fwrite(buf, strlen(buf), 1, pf);
		fwrite("\r\n", strlen("\r\n"), 1, pf);

		for (vector<stMember>::iterator it = m_Data[i].red.begin(); it != m_Data[i].red.end(); ++it)
		{
			CString s1 = _T("");
			s1 = (*it).money > 0 ? _T("+") : _T("");
			_itoa_s((*it).money * m_Sum, buf, 10);
			CString s2 = (*it).name + _T("  ") + s1 + buf;
			fwrite("  ", strlen("  "), 1, pf);
			fwrite(s2.GetBuffer(), s2.GetLength(), 1, pf);
			fwrite("\r\n", strlen("\r\n"), 1, pf);
		}

		fwrite("\r\n", strlen("\r\n"), 1, pf);
		sprintf_s(buf, "蓝队%d", m_Data[i].blue.size());
		fwrite(buf, strlen(buf), 1, pf);
		fwrite("\r\n", strlen("\r\n"), 1, pf);
		for (vector<stMember>::iterator it = m_Data[i].blue.begin(); it != m_Data[i].blue.end(); ++it)
		{
			CString s1 = _T("");
			s1 = (*it).money > 0 ? _T("+") : _T("");
			_itoa_s((*it).money * m_Sum, buf, 10);
			CString s2 = (*it).name + _T("  ") + s1 + buf;
			fwrite("  ", strlen("  "), 1, pf);
			fwrite(s2.GetBuffer(), s2.GetLength(), 1, pf);
			fwrite("\r\n", strlen("\r\n"), 1, pf);
		}

		fwrite("\r\n", strlen("\r\n"), 1, pf);

		char* vectory;
		if (m_Data[i].redLose == 1)
			vectory = "蓝队";
		else if (m_Data[i].redLose == 0)
			vectory = "红队";
		else
			vectory = "未结束";

		fwrite("胜利队伍:", strlen("胜利队伍: "), 1, pf);
		fwrite(vectory, strlen(vectory), 1, pf);
		fwrite("\r\n", strlen("\r\n"), 1, pf);
		fwrite("胜利时间: ", strlen("胜利时间: "), 1, pf);
		if (m_Data[i].redLose == 2)
			fwrite(vectory, strlen(vectory), 1, pf);
		else
			fwrite(m_Data[i].vectoryTmie.GetBuffer(), m_Data[i].vectoryTmie.GetLength(), 1, pf);

		fwrite("\r\n", strlen("\r\n"), 1, pf);
		fwrite("\r\n", strlen("\r\n"), 1, pf);
	}

	for (int i = 0; i < 8; ++i)
	{
		if (i == 3)
			fwrite("\r\n===========================================================",
				strlen("\r\n==========================================================="), 1, pf);
		else
			fwrite("\r\n", strlen("\r\n"), 1, pf);
	}


	fwrite("战绩情况", strlen("战绩情况"), 1, pf);
	fwrite("\r\n", strlen("\r\n"), 1, pf);
	//写入所有人的账单
	for (int i = 0; i < (int)m_AllMemberVect.size(); ++i)
	{
		CString s1 = _T("");
		s1 = m_AllMemberVect[i]->money > 0 ? _T("+") : _T("");
		_itoa_s(m_AllMemberVect[i]->money * m_Sum, buf, 10);
		CString s2 = m_AllMemberVect[i]->name + _T("  ") + s1 + buf;
		fwrite(s2.GetBuffer(), s2.GetLength(), 1, pf);
		fwrite("\r\n", strlen("\r\n"), 1, pf);
	}

	fwrite("\r\n", strlen("\r\n"), 1, pf);
	fwrite("最佳支付方案", strlen("最佳支付方案"), 1, pf);
	fwrite("\r\n", strlen("\r\n"), 1, pf);

	//写入最佳支付
	for (int i = 0; i < (int)m_PaySchemeString.size(); ++i)
	{
		fwrite(m_PaySchemeString[i].GetBuffer(), m_PaySchemeString[i].GetLength(), 1, pf);
		fwrite("\r\n", strlen("\r\n"), 1, pf);
	}

	fwrite("\r\n", strlen("\r\n"), 1, pf);

	fclose(pf);

	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	path = path.Left(pos);

	//CString s = "配置信息以保存在" + path;
	//MessageBox(_TEXT(s.GetString()), _TEXT("提示"), MB_OK | MB_ICONASTERISK);
}

//分组
void CBursterDlg::OnBnClickedButton4_Separate()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_CurMemberVect.size() % 2 != 0 || m_CurMemberVect.size() < 2)
	{
		MessageBox(_TEXT("人数不平均"), _TEXT("提示"), MB_OK | MB_ICONERROR);
		return;
	}

	if (m_RedMemberVect.size() > 0 || m_BlueMemberVect.size() > 0)
	{
		if (MessageBox(_TEXT("是否重新分组"), _TEXT("提示"), MB_YESNO | MB_ICONQUESTION) == IDNO)
			return;
	}

	MessageBox(_TEXT("分组成功"), _TEXT("提示"), MB_OK | MB_ICONASTERISK);

	//创建分组命令
	GroupingCommand* com = new GroupingCommand(this);

	//执行命令 返回成功就放入命令管理器里面 否则删除自己
	if (com->execute())
		CommandManager::getInstance()->StoreCommand(com);
	else
		delete com;
}

//改变金钱
void CBursterDlg::OnBnClickedButton6_Change()
{
	//创建改变金钱命令
	ChangedMoneyCommand* com = new ChangedMoneyCommand(this);

	//执行命令 返回成功就放入命令管理器里面 否则删除自己
	if (com->execute())
		CommandManager::getInstance()->StoreCommand(com);
	else
		delete com;
}

//退出
void CBursterDlg::OnClose()
{
	SaveConfiguration();
	Save("战绩情况.txt", "wb");

	for (int i = 0; i < (int)m_AllMemberVect.size(); ++i)
		delete m_AllMemberVect[i];

	//释放命令缓存
	CommandManager::getInstance()->release();

	//删除临时文件
	if (PathFileExists(TEMP_PATH))
		CFile::Remove(TEMP_PATH);

	CDialogEx::OnClose();
}

//蓝队胜利
void CBursterDlg::OnBnClickedButton5_Blue()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(_TEXT("蓝队胜利?"), _TEXT("提示"), MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;

	//创建胜利命令
	WinCommand* com = new WinCommand(false, this);

	//执行命令 返回成功就放入命令管理器里面 否则删除自己
	if (com->execute())
		CommandManager::getInstance()->StoreCommand(com);
	else
		delete com;
}

//红队胜利
void CBursterDlg::OnBnClickedButton7_Red()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(_TEXT("红队胜利?"), _TEXT("提示"), MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;

	//创建胜利命令
	WinCommand* com = new WinCommand(true, this);

	//执行命令 返回成功就放入命令管理器里面 否则删除自己
	if (com->execute())
		CommandManager::getInstance()->StoreCommand(com);
	else
		delete com;
}

//撤销
void CBursterDlg::OnBnClickedButton9()
{
	CommandManager::getInstance()->undo();
}

//重做
void CBursterDlg::OnBnClickedButton10()
{
	CommandManager::getInstance()->redo();
}

//计时器消息
void CBursterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		CButton* undo = (CButton*)GetDlgItem(IDC_BUTTON9);
		CButton* redo = (CButton*)GetDlgItem(IDC_BUTTON10);

		int RedoSize = CommandManager::getInstance()->GetRedoSize();
		int UndoSize = CommandManager::getInstance()->GetUndoSize();
		undo->EnableWindow(UndoSize > 0 ? TRUE : FALSE);
		redo->EnableWindow(RedoSize > 0 ? TRUE : FALSE);

		//CButton* btn1 = (CButton*)GetDlgItem(IDC_BUTTON8);
		CButton* btn2 = (CButton*)GetDlgItem(IDC_BUTTON2);
		bool enable = (m_RedListBox->GetCount() == 0 && m_BlueListBox->GetCount() == 0);
		//btn1->EnableWindow(enable);
		btn2->EnableWindow(enable);

		CButton* red = (CButton*)GetDlgItem(IDC_BUTTON7);
		CButton* blue = (CButton*)GetDlgItem(IDC_BUTTON5);

		red->EnableWindow(m_RedMemberVect.size() > 0);
		blue->EnableWindow(m_BlueMemberVect.size() > 0);
	}
	else if (nIDEvent == 2)//检查更新
	{
		//清除当前计时器
		KillTimer(2);

		//检测更新
		m_pThread = AfxBeginThread(downRemoteFile, this);
	}

	CDialogEx::OnTimer(nIDEvent);
}

//设置
void CBursterDlg::OnBnClickedButton11()
{
	//创建配置命令
	ConfigCommand* com = new ConfigCommand(this);

	//执行命令 返回成功就放入命令管理器里面 否则删除自己
	if (com->execute())
		CommandManager::getInstance()->StoreCommand(com);
	else
		delete com;
}

//处理消息
BOOL CBursterDlg::PreTranslateMessage(MSG* pMsg)
{
	do
	{
		if (pMsg->message != WM_KEYDOWN)
			break;

		const SHORT l_cnKeyState = 0x8000;
		if (l_cnKeyState != (GetKeyState(VK_CONTROL) & l_cnKeyState))
			break;

		// do ctrl+Z
		if (pMsg->wParam == 'Z')
			CommandManager::getInstance()->undo();
		// do ctrl+Y
		else if (pMsg->wParam == 'Y')
			CommandManager::getInstance()->redo();

	} while (FALSE);

	return CDialog::PreTranslateMessage(pMsg);
}

