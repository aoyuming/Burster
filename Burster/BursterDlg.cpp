
// BursterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Burster.h"
#include "BursterDlg.h"
#include "afxdialogex.h"


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
	ON_BN_CLICKED(IDC_BUTTON5, &CBursterDlg::OnBnClickedButton5_Bule)
	ON_BN_CLICKED(IDC_BUTTON7, &CBursterDlg::OnBnClickedButton7_Red)
	ON_EN_CHANGE(IDC_EDIT2, &CBursterDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON9, &CBursterDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CBursterDlg::OnBnClickedButton10)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON11, &CBursterDlg::OnBnClickedButton11)
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
	CListBox* buleList = (CListBox*)GetDlgItem(IDC_LIST3);
	redList->ResetContent();
	buleList->ResetContent();

	for( int i = 0; i < m_RedMenberVect.size(); ++i)
		redList->AddString(m_RedMenberVect[i]->name);
	for( int i = 0; i < m_BuleMenberVect.size(); ++i)
		buleList->AddString(m_BuleMenberVect[i]->name);

	CListBox* listbox1 = (CListBox*)GetDlgItem(IDC_LIST2);
	CListBox* listbox2 = (CListBox*)GetDlgItem(IDC_LIST5);

	for (int i = 0; i < m_AllMenberVect.size(); ++i)
	{
		char buf[32];
		_itoa_s(m_AllMenberVect[i]->money * m_Sum,buf,10);
		
		if (m_AllMenberVect[i]->money >= 0)
			listbox2->InsertString(0,m_AllMenberVect[i]->name + " +" + buf);
		else
			listbox2->InsertString(0,m_AllMenberVect[i]->name + " " + buf);
	}

	for (int i = 0; i < m_CurMenberVect.size(); ++i)
		listbox1->AddString(m_CurMenberVect[i]->name);
	PayScheme();

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
	_itoa_s(m_AllMenberVect.size(),buf,10);

	//ȫ��������
	CString Num = _T("ȫ������=") + CString(buf);
	fwrite(Num.GetBuffer(),Num.GetLength(),1,pf);

	//д��ȫ����Ա����Ϣ
	for (int i = 0; i < m_AllMenberVect.size(); ++i)
	{
		_itoa_s(m_AllMenberVect[i]->money,buf,10);
		CString s = _T("\r\n����=") + m_AllMenberVect[i]->name;
		fwrite(s.GetBuffer(),s.GetLength(),1,pf);
		s = + _T("\r\n��Ǯ=") + CString(buf);
		fwrite(s.GetBuffer(),s.GetLength(),1,pf);
	}

	fwrite("\r\n",strlen("\r\n"),1,pf);

	//д���������
	_itoa_s(m_DoveMenberVect.size(),buf,10);
	Num = _T("\r\n��������=") + CString(buf);
	fwrite(Num.GetBuffer(),Num.GetLength(),1,pf);

	//д����ӳ�Ա����Ϣ
	for (int i = 0; i < m_DoveMenberVect.size(); ++i)
	{
		_itoa_s(m_DoveMenberVect[i]->money,buf,10);
		CString s = _T("\r\n����=") + m_DoveMenberVect[i]->name;
		fwrite(s.GetBuffer(),s.GetLength(),1,pf);
	}

	fwrite("\r\n",strlen("\r\n"),1,pf);

	_itoa_s(m_RedMenberVect.size(),buf,10);
	Num = _T("\r\n�������=") + CString(buf);
	fwrite(Num.GetBuffer(),Num.GetLength(),1,pf);

	//д��������
	if (m_RedMenberVect.size() != 0)
	{
		for (int i = 0; i < m_RedMenberVect.size(); ++i)
		{
			_itoa_s(m_RedMenberVect[i]->money,buf,10);
			CString s = _T("\r\n����=") + m_RedMenberVect[i]->name;
			fwrite(s.GetBuffer(),s.GetLength(),1,pf);
		}
	}

	fwrite("\r\n",strlen("\r\n"),1,pf);

	_itoa_s(m_BuleMenberVect.size(),buf,10);
	Num = _T("\r\n��������=") + CString(buf);
	fwrite(Num.GetBuffer(),Num.GetLength(),1,pf);

	//д����������
	if (m_BuleMenberVect.size() != 0)
	{
		for (int i = 0; i < m_BuleMenberVect.size(); ++i)
		{
			_itoa_s(m_BuleMenberVect[i]->money,buf,10);
			CString s = _T("\r\n����=") + m_BuleMenberVect[i]->name;
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
		m_AllMenberVect.push_back(p);
		m_CurMenberVect.push_back(p);
	}

	//��������
	allNum = 0;
	fscanf_s(pf,"\r\n��������=%d",&allNum);

	for (int i = 0; i < allNum; ++i)
	{
		fscanf_s(pf,"\r\n����=%s",buf,32);
		
		for (int j = 0; j < m_CurMenberVect.size(); ++j)
		{
			if (buf == m_CurMenberVect[j]->name)
			{
				m_DoveMenberVect.push_back(m_CurMenberVect[j]);
				m_CurMenberVect.erase(m_CurMenberVect.begin() + j);
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

		for (int j = 0; j < m_CurMenberVect.size(); ++j)
		{
			if (buf == m_CurMenberVect[j]->name)
			{
				m_RedMenberVect.push_back(m_CurMenberVect[j]);
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

		for (int j = 0; j < m_CurMenberVect.size(); ++j)
		{
			if (buf == m_CurMenberVect[j]->name)
			{
				m_BuleMenberVect.push_back(m_CurMenberVect[j]);
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

		int redNum = 0,buleNum = 0;
		fscanf_s(pf,"\r\n���%d",&redNum);

		for (int j = 0; j < redNum; ++j)
		{
			stMember m;
			fscanf_s(pf,"\r\n  %s  %d",buf,64,&m.money);
			m.money /= m_Sum;
			m.name = buf;
			d.red.push_back(m);
		}

		fscanf_s(pf,"\r\n����%d",&buleNum);

		for (int j = 0; j < buleNum; ++j)
		{
			stMember m;
			fscanf_s(pf,"\r\n  %s  %d",buf,64,&m.money);
			m.money /= m_Sum;
			m.name = buf;
			d.bule.push_back(m);
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
								   &m_AllMenberVect,
								   &m_CurMenberVect, 
								   &m_DoveMenberVect,
								   name);
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
	CSelectDlg dlg(m_CurMenberVect, index, NULL,  _T("��ѡ��Ҫɾ���ĳ�Ա"), _T("ɾ��"));
	dlg.DoModal();

	if (index == -1)
		return;

	//������ӳ�Ա����
	EraseCommand* erase = new EraseCommand(listbox,
										&m_CurMenberVect,
										&m_DoveMenberVect,
										index);
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
	for (int i = 0; i < (int)m_AllMenberVect.size(); ++i)
		delete m_AllMenberVect[i];

	m_AllMenberVect.clear();
	m_CurMenberVect.clear();
	m_RedMenberVect.clear();
	m_BuleMenberVect.clear();
	m_Data.clear();
	m_DoveMenberVect.clear();

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
	

	if (m_RedMenberVect.size() > 0 || m_BuleMenberVect.size() > 0)
	{
		m_Data[m_Data.size() - 1].red.clear();
		m_Data[m_Data.size() - 1].bule.clear();
		for (int i = 0; i < m_RedMenberVect.size(); ++i)
			m_Data[m_Data.size() - 1].red.push_back(*m_RedMenberVect[i]);

		for (int i = 0; i < m_BuleMenberVect.size(); ++i)
			m_Data[m_Data.size() - 1].bule.push_back(*m_BuleMenberVect[i]);
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
		sprintf_s(buf,"����%d",m_Data[i].bule.size());
		fwrite(buf,strlen(buf),1,pf);
		fwrite("\r\n",strlen("\r\n"),1,pf);
		for (vector<stMember>::iterator it = m_Data[i].bule.begin(); it != m_Data[i].bule.end(); ++it)
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
	for (int i = 0; i < m_AllMenberVect.size(); ++i)
	{
		CString s1 = _T("");
		s1 = m_AllMenberVect[i]->money > 0 ? _T("+") : _T("");
		_itoa_s(m_AllMenberVect[i]->money * m_Sum,buf,10);
		CString s2 = m_AllMenberVect[i]->name + _T("  ") + s1 + buf;
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
	if (m_CurMenberVect.size() % 2 != 0 || m_CurMenberVect.size() < 2)
	{
		MessageBox(_TEXT("������ƽ��"),_TEXT("��ʾ"),MB_OK);
		return;
	}

	if (m_RedMenberVect.size() > 0 || m_BuleMenberVect.size() > 0)
	{
		if (MessageBox(_TEXT("�Ƿ����·���"),_TEXT("��ʾ"),MB_YESNO) == IDNO)
			return;
	}

	MessageBox(_TEXT("����ɹ�"),_TEXT("��ʾ"),MB_OK);

	//������������
	GroupingCommand* grouoping = new GroupingCommand((CListBox*)GetDlgItem(IDC_LIST4),
													(CListBox*)GetDlgItem(IDC_LIST3),
													&m_CurMenberVect,
													&m_RedMenberVect,
													&m_BuleMenberVect,
													&m_Data);
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
													&m_AllMenberVect,
													&m_PaySchemeString,
													&m_Sum);
	com->execute();
	CommandManager::getInstance()->StoreCommand(com);
}

//���֧���������ݹ飩
void CBursterDlg::_PayScheme(vector<stMember>& lose, vector<stMember>& win, vector<CString>& payScheme)
{
	
}

//���֧������
void CBursterDlg::PayScheme()
{
	m_PaySchemeString.clear();
	char buf[32];

	//��ӮǮ�ĺ���Ǯ�ķֿ�
	vector<stMember> lose,vectory;
	for (int i = 0; i < m_AllMenberVect.size(); ++i)
	{
		if (m_AllMenberVect[i]->money < 0)
			lose.push_back(*m_AllMenberVect[i]);
		else if (m_AllMenberVect[i]->money > 0)
			vectory.push_back(*m_AllMenberVect[i]);
	}

	////�ݹ����
	//_PayScheme(lose, vectory, m_PaySchemeString);

	vector<stMember>::iterator loseIt = lose.begin();
	while (loseIt != lose.end())
	{
		bool flag = true;
		for (vector<stMember>::iterator it = vectory.begin(); it != vectory.end();)
		{
			if (-((*loseIt).money) == (*it).money)
			{
				_itoa_s((*it).money * m_Sum,buf,10);
				CString s = (*loseIt).name + _TEXT("  ��  ") + (*it).name + _TEXT("  ") + buf;
				m_PaySchemeString.push_back(s);

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
				_itoa_s((*it).money * m_Sum,buf,10);
				CString s = (*loseIt).name + _TEXT("  ��  ") + (*it).name + _TEXT("  ") + buf;
				m_PaySchemeString.push_back(s);

				loseIt = lose.erase(loseIt);
				it = vectory.erase(it);
				flag = false;
				break;
			}
			else if (-((*loseIt).money) < (*it).money)
			{
				_itoa_s(-(*loseIt).money * m_Sum,buf,10);
				CString s = (*loseIt).name + _TEXT("  ��  ") + (*it).name +  _TEXT("  ") + buf;
				m_PaySchemeString.push_back(s);

				(*it).money += (*loseIt).money;
				loseIt = lose.erase(loseIt);
				flag = false;

				if ((*it).money <= 0)
					it = vectory.erase(it);
				break;
			}
			else
			{
				_itoa_s((*it).money * m_Sum,buf,10);
				CString s = (*loseIt).name + _TEXT("  ��  ") + (*it).name +  _TEXT("  ") + buf;
				m_PaySchemeString.push_back(s);

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
		m_PaySchemeString.push_back(s);
	}

	for (auto it = vectory.begin(); it != vectory.end(); ++it)
	{
		_itoa_s(abs((*it).money * m_Sum), buf, 10);
		CString s = _T("[???]  ��  ") + (*it).name +  + _TEXT("  ") + buf;
		m_PaySchemeString.push_back(s);
	}

	CListBox* listbox1 = (CListBox*)GetDlgItem(IDC_LIST6);
	listbox1->ResetContent();

	for (int i = 0; i < m_PaySchemeString.size(); ++i)
		listbox1->InsertString(0, m_PaySchemeString[i]);
}

//�˳�
void CBursterDlg::OnClose()
{
	SaveConfiguration();
	Save("ս�����.txt","wb");

	for (int i = 0; i < (int)m_AllMenberVect.size(); ++i)
		delete m_AllMenberVect[i];

	CDialogEx::OnClose();
}

//����ʤ��
void CBursterDlg::OnBnClickedButton5_Bule()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (MessageBox(_TEXT("����ʤ��?"),_TEXT("��ʾ"),MB_YESNO) == IDNO)
		return;

	//����ʤ������
	WinCommand* win = new WinCommand((CListBox*)GetDlgItem(IDC_LIST4),
									(CListBox*)GetDlgItem(IDC_LIST3),
									(CListBox*)GetDlgItem(IDC_LIST5),
									(CListBox*)GetDlgItem(IDC_LIST6),
									&m_AllMenberVect,
									&m_RedMenberVect,
									&m_BuleMenberVect,
									&m_PaySchemeString,
									&m_Data,
									m_Sum,
									false);
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
									&m_AllMenberVect,
									&m_RedMenberVect,
									&m_BuleMenberVect,
									&m_PaySchemeString,
									&m_Data,
									m_Sum,
									true);
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
		CListBox* bule = (CListBox*)GetDlgItem(IDC_LIST3);
		//CButton* btn1 = (CButton*)GetDlgItem(IDC_BUTTON8);
		CButton* btn2 = (CButton*)GetDlgItem(IDC_BUTTON2);
		bool enable = (red->GetCount() == 0 && bule->GetCount() == 0);
		//btn1->EnableWindow(enable);
		btn2->EnableWindow(enable);
	}

	CDialogEx::OnTimer(nIDEvent);
}

//����
void CBursterDlg::OnBnClickedButton11()
{
	if (m_CurMenberVect.size() % 2 != 0)
	{
		MessageBox(_T("��Ա��ƽ��"), _T("��ʾ"), MB_OK);
		return;
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool save = false;
	CConfig dlg((CListBox*)GetDlgItem(IDC_LIST5),
				&m_CurMenberVect,
				&m_AllMenberVect,
				&m_RedMenberVect,
				&m_BuleMenberVect,
				m_Sum,
				save);

	dlg.DoModal();

	//�޸�
	if (save)
	{
		CListBox* list[] = 
		{ 
			(CListBox*)GetDlgItem(IDC_LIST2),
			(CListBox*)GetDlgItem(IDC_LIST4), 
			(CListBox*)GetDlgItem(IDC_LIST3), 
			(CListBox*)GetDlgItem(IDC_LIST5) 
		};

		vector<stMember*>* vect[] = 
		{ 
			&m_CurMenberVect,
			&m_RedMenberVect, 
			&m_BuleMenberVect, 
			&m_AllMenberVect 
		};

		for (int i = 0; i < 4; ++i)
		{
			list[i]->ResetContent();
			for (auto it = vect[i]->begin(); it != vect[i]->end(); ++it)
			{
				CString str = (*it)->name;
				CString money;
				money.Format("%d", (*it)->money * m_Sum);
				if ((*it)->money >= 0)
					money = _T("+") + money;
				if (i == 3)
					str = str + _T("  ") + money;
				list[i]->AddString(str);
			}
		}

		m_CurMenberVect = m_AllMenberVect;
		for (int i = 0; i < m_DoveMenberVect.size(); ++i)
		{
			int index = IsMemberInVect_index(*m_DoveMenberVect[i], m_CurMenberVect);
			if (index != -1)
				m_CurMenberVect.erase(m_CurMenberVect.begin() + index);
		}

		//���¼������֧������
		PayScheme();
	}
}
