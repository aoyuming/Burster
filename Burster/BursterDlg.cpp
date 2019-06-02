
// BursterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Burster.h"
#include "BursterDlg.h"
#include "afxdialogex.h"
#include "projectCommand.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBursterDlg �Ի���



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
	ON_EN_CHANGE(IDC_EDIT2, &CBursterDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON9, &CBursterDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CBursterDlg::OnBnClickedButton10)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON11, &CBursterDlg::OnBnClickedButton11)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CBursterDlg ��Ϣ�������

BOOL CBursterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//SetWindowLong(AfxGetMainWnd()->m_hWnd, GWL_STYLE,WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU | WS_MINIMIZEBOX);
	m_Sum = 100;
	GetDlgItem(IDC_EDIT2)->SetWindowText(CString(_TEXT("100")));
	::SetWindowText(AfxGetMainWnd()->m_hWnd,_TEXT("������ - v1.0.0.0  -  ��������ǹ����"));


	LoadConfiguration();

	CListBox* redList = (CListBox*)GetDlgItem(IDC_LIST4);
	CListBox* blueList = (CListBox*)GetDlgItem(IDC_LIST3);
	redList->ResetContent();
	blueList->ResetContent();

	for( int i = 0; i < m_RedMemberVect.size(); ++i)
		redList->AddString(m_RedMemberVect[i]->name);
	for( int i = 0; i < m_BlueMemberVect.size(); ++i)
		blueList->AddString(m_BlueMemberVect[i]->name);

	CListBox* listbox1 = (CListBox*)GetDlgItem(IDC_LIST2);
	CListBox* listbox2 = (CListBox*)GetDlgItem(IDC_LIST5);

	for (int i = 0; i < m_AllMemberVect.size(); ++i)
	{
		char buf[32];
		_itoa_s(m_AllMemberVect[i]->money * m_Sum,buf,10);
		
		if (m_AllMemberVect[i]->money >= 0)
			listbox2->InsertString(0,m_AllMemberVect[i]->name + " +" + buf);
		else
			listbox2->InsertString(0,m_AllMemberVect[i]->name + " " + buf);
	}

	for (int i = 0; i < m_CurMemberVect.size(); ++i)
		listbox1->AddString(m_CurMemberVect[i]->name);
	
	//���֧������
	PayScheme_g(&m_PaySchemeString, &m_AllMemberVect, m_Sum, (CListBox*)GetDlgItem(IDC_LIST6));

	//�б��
	CListBox* list[] =
	{
		(CListBox*)GetDlgItem(IDC_LIST5),
		(CListBox*)GetDlgItem(IDC_LIST3),
		(CListBox*)GetDlgItem(IDC_LIST4),
		(CListBox*)GetDlgItem(IDC_LIST6),
		(CListBox*)GetDlgItem(IDC_LIST2)
	};

	//for (int i = 0; i < 5; ++i)
	//	list[i]->EnableWindow(FALSE);

	SetTimer(1, 33, NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}



// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBursterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBursterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//���������ļ�
void CBursterDlg::SaveConfiguration()
{
	FILE* pf = NULL;
	fopen_s(&pf,"config.txt","wb");

	char buf[32];
	_itoa_s(m_AllMemberVect.size(),buf,10);

	//ȫ��������
	CString Num = _T("ȫ������=") + CString(buf);
	fwrite(Num.GetBuffer(),Num.GetLength(),1,pf);

	//д��ȫ����Ա����Ϣ
	for (int i = 0; i < m_AllMemberVect.size(); ++i)
	{
		_itoa_s(m_AllMemberVect[i]->money,buf,10);
		CString s = _T("\r\n����=") + m_AllMemberVect[i]->name;
		fwrite(s.GetBuffer(),s.GetLength(),1,pf);
		s = + _T("\r\n��Ǯ=") + CString(buf);
		fwrite(s.GetBuffer(),s.GetLength(),1,pf);
	}

	fwrite("\r\n",strlen("\r\n"),1,pf);

	//д���������
	_itoa_s(m_DoveMemberVect.size(),buf,10);
	Num = _T("\r\n��������=") + CString(buf);
	fwrite(Num.GetBuffer(),Num.GetLength(),1,pf);

	//д����ӳ�Ա����Ϣ
	for (int i = 0; i < m_DoveMemberVect.size(); ++i)
	{
		_itoa_s(m_DoveMemberVect[i]->money,buf,10);
		CString s = _T("\r\n����=") + m_DoveMemberVect[i]->name;
		fwrite(s.GetBuffer(),s.GetLength(),1,pf);
	}

	fwrite("\r\n",strlen("\r\n"),1,pf);

	_itoa_s(m_RedMemberVect.size(),buf,10);
	Num = _T("\r\n�������=") + CString(buf);
	fwrite(Num.GetBuffer(),Num.GetLength(),1,pf);

	//д��������
	if (m_RedMemberVect.size() != 0)
	{
		for (int i = 0; i < m_RedMemberVect.size(); ++i)
		{
			_itoa_s(m_RedMemberVect[i]->money,buf,10);
			CString s = _T("\r\n����=") + m_RedMemberVect[i]->name;
			fwrite(s.GetBuffer(),s.GetLength(),1,pf);
		}
	}

	fwrite("\r\n",strlen("\r\n"),1,pf);

	_itoa_s(m_BlueMemberVect.size(),buf,10);
	Num = _T("\r\n��������=") + CString(buf);
	fwrite(Num.GetBuffer(),Num.GetLength(),1,pf);

	//д����������
	if (m_BlueMemberVect.size() != 0)
	{
		for (int i = 0; i < m_BlueMemberVect.size(); ++i)
		{
			_itoa_s(m_BlueMemberVect[i]->money,buf,10);
			CString s = _T("\r\n����=") + m_BlueMemberVect[i]->name;
			fwrite(s.GetBuffer(),s.GetLength(),1,pf);
		}
	}
	fwrite("\r\n",strlen("\r\n"),1,pf);
	fwrite("\r\n",strlen("\r\n"),1,pf);

	fclose(pf);
}

//���������ļ�
bool CBursterDlg::LoadConfiguration()
{
	FILE* pf = NULL;
	fopen_s(&pf,"config.txt","rb");

	if (NULL == pf)
		return false;

	//ȫ������
	int allNum = 0;
	fscanf_s(pf,"ȫ������=%d",&allNum);

	char buf[64];
	for (int i = 0; i < allNum; ++i)
	{
		stMember* p = new stMember;
		fscanf_s(pf,"\r\n����=%s",buf,32);
		fscanf_s(pf,"\r\n��Ǯ=%d",&p->money);
		p->name = buf;
		m_AllMemberVect.push_back(p);
		m_CurMemberVect.push_back(p);
	}

	//��������
	allNum = 0;
	fscanf_s(pf,"\r\n��������=%d",&allNum);

	for (int i = 0; i < allNum; ++i)
	{
		fscanf_s(pf,"\r\n����=%s",buf,32);
		
		for (int j = 0; j < m_CurMemberVect.size(); ++j)
		{
			if (buf == m_CurMemberVect[j]->name)
			{
				m_DoveMemberVect.push_back(m_CurMemberVect[j]);
				m_CurMemberVect.erase(m_CurMemberVect.begin() + j);
				break;
			}
		}
	}

	//�������
	allNum = 0;
	fscanf_s(pf,"\r\n�������=%d",&allNum);

	for (int i = 0; i < allNum; ++i)
	{
		fscanf_s(pf,"\r\n����=%s",buf,32);

		for (int j = 0; j < m_CurMemberVect.size(); ++j)
		{
			if (buf == m_CurMemberVect[j]->name)
			{
				m_RedMemberVect.push_back(m_CurMemberVect[j]);
				break;
			}
		}
	}

	//��������
	allNum = 0;
	fscanf_s(pf,"\r\n��������=%d",&allNum);

	for (int i = 0; i < allNum; ++i)
	{
		fscanf_s(pf,"\r\n����=%s",buf,32);

		for (int j = 0; j < m_CurMemberVect.size(); ++j)
		{
			if (buf == m_CurMemberVect[j]->name)
			{
				m_BlueMemberVect.push_back(m_CurMemberVect[j]);
				break;
			}
		}
	}

	//������Ϣ
	allNum = 0;
	fscanf_s(pf,"\r\n��ϸ��Ϣ%d",&allNum);
	for (int i = 0; i < allNum; ++i)
	{
		stData d;
		fscanf_s(pf,"\r\n����ʱ��:%s",buf,64);
		d.fenZutime = buf;

		int redNum = 0,blueNum = 0;
		fscanf_s(pf,"\r\n���%d",&redNum);

		for (int j = 0; j < redNum; ++j)
		{
			stMember m;
			fscanf_s(pf,"\r\n  %s  %d",buf,64,&m.money);
			m.money /= m_Sum;
			m.name = buf;
			d.red.push_back(m);
		}

		fscanf_s(pf,"\r\n����%d",&blueNum);

		for (int j = 0; j < blueNum; ++j)
		{
			stMember m;
			fscanf_s(pf,"\r\n  %s  %d",buf,64,&m.money);
			m.money /= m_Sum;
			m.name = buf;
			d.blue.push_back(m);
		}
		fscanf_s(pf,"\r\nʤ������:%s",buf,64);
		if (strcmp(buf,"δ����") == 0)
			d.redLose = 2;
		else if (strcmp(buf,"���") == 0)
			d.redLose = 0;
		else
			d.redLose = 1;

		fscanf_s(pf,"\r\nʤ��ʱ��:%s",buf,64);
		if (strcmp(buf,"δ����") == 0)
			d.redLose = 2;
		else
			d.vectoryTmie = buf;

		m_Data.push_back(d);
	}

	fclose(pf);

	return true;
}

//�ж�stMember�Ƿ���vect����
stMember* CBursterDlg::IsMemberInVect(stMember m,vector<stMember*>& ls)
{
	for(vector<stMember*>::iterator it = ls.begin(); it != ls.end(); ++it)
	{
		if (m.name == (*it)->name)
			return *it;
	}

	return NULL;
}

//�ж�stMember�Ƿ���vect����
int CBursterDlg::IsMemberInVect_index(stMember m, vector<stMember*>& ls)
{
	for (int i = 0; i < (int)ls.size(); ++i)
	{
		if (m.name == ls[i]->name)
			return i;
	}

	return -1;
}

//���
void CBursterDlg::OnBnClickedButton8_Add()
{
	//������Ӵ���
	CString name;
	addName addDlg(name, this);
	addDlg.DoModal();

	if (name == _T(""))
		return;

	//������ӳ�Ա����
	AddCommand* add = new AddCommand((CListBox*)GetDlgItem(IDC_LIST5), 
								  (CListBox*)GetDlgItem(IDC_LIST2),		
								   name,
								   this);
	//ִ������
	add->execute();
	CommandManager::getInstance()->StoreCommand(add);

	//�ݹ����
	OnBnClickedButton8_Add();
}

//ɾ��
void CBursterDlg::OnBnClickedButton2_Delete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CListBox* listbox = (CListBox*)GetDlgItem(IDC_LIST2);
	int index = -1;
	CSelectDlg dlg(m_CurMemberVect, index, NULL,  _T("��ѡ��Ҫɾ���ĳ�Ա"), _T("ɾ��"));
	dlg.DoModal();

	if (index == -1)
		return;

	//������ӳ�Ա����
	EraseCommand* erase = new EraseCommand(listbox,index, this);
	//ִ������
	erase->execute();
	CommandManager::getInstance()->StoreCommand(erase);
}

//���
void CBursterDlg::OnBnClickedButton3_Clear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ʾ
	if (MessageBox(_TEXT("ȷ�������  �˲������ɳ��أ�"), _TEXT("��ʾ"),MB_YESNO) == IDNO)
		return;
	
	//�б��
	CListBox* list[] = 
	{
		(CListBox*)GetDlgItem(IDC_LIST5),
		(CListBox*)GetDlgItem(IDC_LIST3),
		(CListBox*)GetDlgItem(IDC_LIST4),
		(CListBox*)GetDlgItem(IDC_LIST6),
		(CListBox*)GetDlgItem(IDC_LIST2)
	};

	//���
	for (int i = 0; i < 5 ; ++i)
		list[i]->ResetContent();

	//�ͷŶ��ڴ�
	for (int i = 0; i < (int)m_AllMemberVect.size(); ++i)
		delete m_AllMemberVect[i];

	m_AllMemberVect.clear();
	m_CurMemberVect.clear();
	m_RedMemberVect.clear();
	m_BlueMemberVect.clear();
	m_Data.clear();
	m_DoveMemberVect.clear();

	//�ͷ�����
	CommandManager::getInstance()->release();
}

//����
void CBursterDlg::OnBnClickedButton1_Save()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Save("ս�����.txt","wb");
}

//����
void CBursterDlg::Save(const char* fn,const char* rb)
{
	if (m_Data.size() == 0)
		return;
	

	if (m_RedMemberVect.size() > 0 || m_BlueMemberVect.size() > 0)
	{
		m_Data[m_Data.size() - 1].red.clear();
		m_Data[m_Data.size() - 1].blue.clear();
		for (int i = 0; i < m_RedMemberVect.size(); ++i)
			m_Data[m_Data.size() - 1].red.push_back(*m_RedMemberVect[i]);

		for (int i = 0; i < m_BlueMemberVect.size(); ++i)
			m_Data[m_Data.size() - 1].blue.push_back(*m_BlueMemberVect[i]);
	}

	FILE* pf = 0;
	char buf[64];
	fopen_s(&pf,fn,rb);

	sprintf_s(buf,"��ϸ��Ϣ%d",m_Data.size());
	fwrite(buf,strlen(buf),1,pf);
	fwrite("\r\n",strlen("\r\n"),1,pf);
	//д����Ϣ
	for (int i = 0; i < m_Data.size(); ++i)
	{
		fwrite("\r\n",strlen("\r\n"),1,pf);
		fwrite("\r\n",strlen("\r\n"),1,pf);
		fwrite("����ʱ��: ",strlen("����ʱ��: "),1,pf);
		fwrite(m_Data[i].fenZutime.GetBuffer(),m_Data[i].fenZutime.GetLength(),1,pf);
		fwrite("\r\n",strlen("\r\n"),1,pf);

		sprintf_s(buf,"���%d",m_Data[i].red.size());
		fwrite(buf,strlen(buf),1,pf);
		fwrite("\r\n",strlen("\r\n"),1,pf);

		for (vector<stMember>::iterator it = m_Data[i].red.begin(); it != m_Data[i].red.end(); ++it)
		{
			CString s1 = _T("");
			s1 = (*it).money > 0 ? _T("+") : _T("");
			_itoa_s((*it).money * m_Sum,buf,10);
			CString s2 = (*it).name + _T("  ") + s1 + buf;
			fwrite("  ",strlen("  "),1,pf);
			fwrite(s2.GetBuffer(),s2.GetLength(),1,pf);
			fwrite("\r\n",strlen("\r\n"),1,pf);
		}

		fwrite("\r\n",strlen("\r\n"),1,pf);
		sprintf_s(buf,"����%d",m_Data[i].blue.size());
		fwrite(buf,strlen(buf),1,pf);
		fwrite("\r\n",strlen("\r\n"),1,pf);
		for (vector<stMember>::iterator it = m_Data[i].blue.begin(); it != m_Data[i].blue.end(); ++it)
		{
			CString s1 = _T("");
			s1 = (*it).money > 0 ? _T("+") : _T("");
			_itoa_s((*it).money * m_Sum,buf,10);
			CString s2 = (*it).name + _T("  ") + s1 + buf;
			fwrite("  ",strlen("  "),1,pf);
			fwrite(s2.GetBuffer(),s2.GetLength(),1,pf);
			fwrite("\r\n",strlen("\r\n"),1,pf);
		}

		fwrite("\r\n",strlen("\r\n"),1,pf);

		char* vectory;
		if(m_Data[i].redLose == 1)
			vectory = "����";
		else if (m_Data[i].redLose == 0)
			vectory = "���";
		else
			vectory = "δ����";

		fwrite("ʤ������:",strlen("ʤ������: "),1,pf);
		fwrite(vectory,strlen(vectory),1,pf);
		fwrite("\r\n",strlen("\r\n"),1,pf);
		fwrite("ʤ��ʱ��: ",strlen("ʤ��ʱ��: "),1,pf);
		if (m_Data[i].redLose == 2)
			fwrite(vectory,strlen(vectory),1,pf);
		else
			fwrite(m_Data[i].vectoryTmie.GetBuffer(),m_Data[i].vectoryTmie.GetLength(),1,pf);

		fwrite("\r\n",strlen("\r\n"),1,pf);
		fwrite("\r\n",strlen("\r\n"),1,pf);
	}

	for(int i = 0; i < 8; ++i)
	{
		if (i == 3)
				fwrite("\r\n===========================================================",
					   strlen("\r\n==========================================================="),1,pf);
		else
			fwrite("\r\n",strlen("\r\n"),1,pf);
	}


	fwrite("ս�����",strlen("ս�����"),1,pf);
	fwrite("\r\n",strlen("\r\n"),1,pf);
	//д�������˵��˵�
	for (int i = 0; i < m_AllMemberVect.size(); ++i)
	{
		CString s1 = _T("");
		s1 = m_AllMemberVect[i]->money > 0 ? _T("+") : _T("");
		_itoa_s(m_AllMemberVect[i]->money * m_Sum,buf,10);
		CString s2 = m_AllMemberVect[i]->name + _T("  ") + s1 + buf;
		fwrite(s2.GetBuffer(),s2.GetLength(),1,pf);
		fwrite("\r\n",strlen("\r\n"),1,pf);
	}

	fwrite("\r\n",strlen("\r\n"),1,pf);
	fwrite("���֧������",strlen("���֧������"),1,pf);
	fwrite("\r\n",strlen("\r\n"),1,pf);

	//д�����֧��
	for (int i = 0; i < m_PaySchemeString.size(); ++i)
	{
		fwrite(m_PaySchemeString[i].GetBuffer(),m_PaySchemeString[i].GetLength(),1,pf);
		fwrite("\r\n",strlen("\r\n"),1,pf);
	}

	fwrite("\r\n",strlen("\r\n"),1,pf);

	fclose(pf);

	CString path;
    GetModuleFileName(NULL,path.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
    path.ReleaseBuffer();
    int pos = path.ReverseFind('\\');
    path = path.Left(pos);

	CString s = "�Ա�����" + path;
	MessageBox(_TEXT(s.GetString()),_TEXT("��ʾ"),MB_OK);
}

//����
void CBursterDlg::OnBnClickedButton4_Separate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_CurMemberVect.size() % 2 != 0 || m_CurMemberVect.size() < 2)
	{
		MessageBox(_TEXT("������ƽ��"),_TEXT("��ʾ"),MB_OK);
		return;
	}

	if (m_RedMemberVect.size() > 0 || m_BlueMemberVect.size() > 0)
	{
		if (MessageBox(_TEXT("�Ƿ����·���"),_TEXT("��ʾ"),MB_YESNO) == IDNO)
			return;
	}

	MessageBox(_TEXT("����ɹ�"),_TEXT("��ʾ"),MB_OK);

	//������������
	GroupingCommand* grouoping = new GroupingCommand((CListBox*)GetDlgItem(IDC_LIST4),
												 (CListBox*)GetDlgItem(IDC_LIST3),
												 this);
	grouoping->execute();
	CommandManager::getInstance()->StoreCommand(grouoping);
}

//�ı��Ǯ
void CBursterDlg::OnBnClickedButton6_Change()
{
	//�����ı��Ǯ����
	ChangedMoneyCommand* com = new ChangedMoneyCommand((CListBox*)GetDlgItem(IDC_LIST5),
													(CListBox*)GetDlgItem(IDC_LIST6),
													(CEdit*)GetDlgItem(IDC_EDIT2),
													this);
	com->execute();
	CommandManager::getInstance()->StoreCommand(com);
}


//�˳�
void CBursterDlg::OnClose()
{
	SaveConfiguration();
	Save("ս�����.txt","wb");

	for (int i = 0; i < (int)m_AllMemberVect.size(); ++i)
		delete m_AllMemberVect[i];

	CDialogEx::OnClose();
}

//����ʤ��
void CBursterDlg::OnBnClickedButton5_Blue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (MessageBox(_TEXT("����ʤ��?"),_TEXT("��ʾ"),MB_YESNO) == IDNO)
		return;

	//����ʤ������
	WinCommand* win = new WinCommand((CListBox*)GetDlgItem(IDC_LIST4),
									(CListBox*)GetDlgItem(IDC_LIST3),
									(CListBox*)GetDlgItem(IDC_LIST5),
									(CListBox*)GetDlgItem(IDC_LIST6),
									false,
									this);
	win->execute();
	CommandManager::getInstance()->StoreCommand(win);
}

//���ʤ��
void CBursterDlg::OnBnClickedButton7_Red()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (MessageBox(_TEXT("���ʤ��?"),_TEXT("��ʾ"),MB_YESNO) == IDNO)
		return;

	//����ʤ������
	WinCommand* win = new WinCommand((CListBox*)GetDlgItem(IDC_LIST4),
									(CListBox*)GetDlgItem(IDC_LIST3),
									(CListBox*)GetDlgItem(IDC_LIST5),
									(CListBox*)GetDlgItem(IDC_LIST6),
									true,
									this);
	win->execute();
	CommandManager::getInstance()->StoreCommand(win);
}


void CBursterDlg::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�



	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

//����
void CBursterDlg::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CommandManager::getInstance()->undo();
}

//����
void CBursterDlg::OnBnClickedButton10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CommandManager::getInstance()->redo();
}

//��ʱ����Ϣ
void CBursterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
		CButton* undo = (CButton*)GetDlgItem(IDC_BUTTON9);
		CButton* redo = (CButton*)GetDlgItem(IDC_BUTTON10);
		int RedoSize = CommandManager::getInstance()->getRedoSize();
		int UndoSize = CommandManager::getInstance()->getUndoSize();
		undo->EnableWindow(UndoSize > 0 ? TRUE : FALSE);
		redo->EnableWindow(RedoSize > 0 ? TRUE : FALSE);

		CListBox* red = (CListBox*)GetDlgItem(IDC_LIST4);
		CListBox* blue = (CListBox*)GetDlgItem(IDC_LIST3);
		//CButton* btn1 = (CButton*)GetDlgItem(IDC_BUTTON8);
		CButton* btn2 = (CButton*)GetDlgItem(IDC_BUTTON2);
		bool enable = (red->GetCount() == 0 && blue->GetCount() == 0);
		//btn1->EnableWindow(enable);
		btn2->EnableWindow(enable);
	}

	CDialogEx::OnTimer(nIDEvent);
}

//����
void CBursterDlg::OnBnClickedButton11()
{
	if (m_CurMemberVect.size() % 2 != 0)
	{
		MessageBox(_T("��Ա��ƽ��"), _T("��ʾ"), MB_OK);
		return;
	}

	//������������
	ConfigCommand* com = new ConfigCommand((CListBox*)GetDlgItem(IDC_LIST2),
										(CListBox*)GetDlgItem(IDC_LIST5),
										(CListBox*)GetDlgItem(IDC_LIST3),
										(CListBox*)GetDlgItem(IDC_LIST4),
										(CListBox*)GetDlgItem(IDC_LIST6),
										 this);
	//ִ������
	com->execute();
	CommandManager::getInstance()->StoreCommand(com);

}

//���̰���
void CBursterDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar == VK_SHIFT)
	{
	}
}

//���̵���
void CBursterDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}
