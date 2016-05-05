
// TOY2Dlg.cpp : ���� ����
//

#include "stdafx.h"
#include "TOY2.h"
#include "TOY2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTOY2Dlg ��ȭ ����



CTOY2Dlg::CTOY2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTOY2Dlg::IDD, pParent)
	, expression_editbox(_T(""))
	, intermediate_code_editbox(_T(""))
	, result_editbox(_T(""))
	, command_line(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTOY2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, expression_editbox_control);
	DDX_Control(pDX, IDC_EDIT3, intermediate_code_editbox_control);
	DDX_Control(pDX, IDC_EDIT4, result_editbox_control);
	DDX_Text(pDX, IDC_EDIT2, expression_editbox);
	DDX_Text(pDX, IDC_EDIT3, intermediate_code_editbox);
	DDX_Text(pDX, IDC_EDIT4, result_editbox);
	DDX_Text(pDX, IDC_EDIT1, command_line);
	DDX_Control(pDX, IDC_EDIT1, command_line__control);
}

BEGIN_MESSAGE_MAP(CTOY2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTOY2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTOY2Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTOY2Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTOY2Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTOY2Dlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CTOY2Dlg �޽��� ó����

BOOL CTOY2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.


	//���� ���̾�α� edit box ���� �ҷ�����
	UpdateData(TRUE);

	command_line.Format(_T("> "));

	//������� edit box�� ����
	UpdateData(FALSE);



	//defun���� �ҷ�����
	load_defun();



	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CTOY2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CTOY2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CTOY2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//ȭ�� ���� �����
//��ɾ� ���� ������ ����
void CTOY2Dlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//���� ���̾�α� edit box ���� �ҷ�����
	UpdateData(TRUE);
	// <�����Է�> �ʱ�ȭ
	expression_editbox.Format(_T(""));
	//<ǥ��� ��ȯ/�߰��ڵ�>
	intermediate_code_editbox.Format(_T(""));
	//<����� ���>
	result_editbox.Format(_T(""));
	//���� ���� ����
	UpdateData(FALSE);


}


//DEFUN LIST �����ֱ�
//���Ͽ��� �ҷ����°��� �ƴ϶�
//���α׷� �� �ö�� DEFUN LIST�� �����ش�.
//���α׷� ���۽� �ѹ��� �θ��� �ȴ�.
//���� �߰��Ѱ͵� �ٷιٷ� ���Ͽ� ����ȴ�.
void CTOY2Dlg::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//defun list����
	view_defun_list();
}

//���α׷��ε�
void CTOY2Dlg::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.




	//�ҷ��� ��� �����ϱ� ���� CFileDiaglog�� class 
	CFileDialog fDlg(true, _T("s"), _T("program.s"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("TOY���α׷��ҽ�����(*.s)|*.s|"));

	//���� ��θ� �ҷ����� ���̾�α� ȣ��
	//���������� ���� ��ΰ� ��� ������
	//���� �ҷ��������
	if (IDOK == fDlg.DoModal())
	{
		//���� ��ũ����
		CFile savefile;

		//��θ� ����� ����
		CString Pathname = fDlg.GetPathName();

		//���� ���� ������ ����
		CStringA tmp;


		//�о�帰 ������ index;
		int index = 0;

		//���� ����
		if (!savefile.Open(Pathname, CFile::modeRead))
		{
			MessageBox(_T("������ ���� �����ϴ�."), _T(""), MB_OK);
			return;
		}

		//CStringA�� char�� ������ ó�� �̿��Ҷ� �ʿ��� char�� ������
		char* wheretmp;

		//���̺� ������ ũ��
		int lenfile = (int)savefile.GetLength();

		//CString class�� char�迭 ó�� ���
		//���̺� ���� ũ�� ��ŭ ���� ��
		wheretmp = tmp.GetBuffer(lenfile + 1);

		//���� ���θ� ���ۿ� ��
		savefile.Read(wheretmp, lenfile);


		//�������� NULL�־���
		wheretmp[lenfile] = NULL;

		//���� �ݱ�
		savefile.Close();

		//���� ���� ����
		tmp.ReleaseBuffer();

		//CStringA�� CString������
		CString tmp2(tmp);


		//���� ���̾�α� edit box ���� �ҷ�����
		UpdateData(TRUE);
		//���� �Էµ� ���ڰ� ������ �̾� ������ �ƴϸ� ����� ���� �ҷ����� ���Ҽ� �ִ�.
		if ((expression_editbox.GetLength() != 0)){
			if (MessageBox(_T("�Էµ� ���ڿ��� �ֽ��ϴ�\n�̾� ���̽ðڽ��ϱ�?"), _T(""), MB_YESNO | MB_ICONQUESTION) == IDNO){
				expression_editbox.Format(_T(""));
			}
			//�̾� ���϶� �����ѹ� ���ְ� �̾� ����
			else{
				expression_editbox.Append(_T("\r\n"));
			}
		}
		//�о�� ������ edit box�� ����Ѵ�.
		expression_editbox.Append(tmp2);

		//����� edit box�� ����
		UpdateData(FALSE);

		//��ũ�� edit box ���������� ������

	}



}

//���α׷�����
void CTOY2Dlg::OnBnClickedButton4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//���� ���̾�α� edit box ���� �ҷ�����
	UpdateData(TRUE);
	if (expression_editbox.GetLength() == 0){
		MessageBox(_T("�����Ұ��� �����ϴ�."), _T(""), MB_OK);
		return;
	}

	//���� ��� �ҷ����� ���� CFileDialog class 
	CFileDialog fDlg(false, _T("s"), _T("Program.s"), OFN_OVERWRITEPROMPT, _T("TOY���α׷��ҽ�����(*.s)|*.s|"));

	//���̾�α� ȣ��
	if (IDOK == fDlg.DoModal())
	{
		//��θ� ����
		CString Pathname = fDlg.GetPathName();
		//���� ��ũ����
		CFile savefile;

		//������ ����
		if (!savefile.Open(Pathname, CFile::modeWrite | CFile::modeCreate))
		{
			MessageBox(_T("������ ���� �����ϴ�."), _T(""), MB_OK);
			return;
		}
		//���� ���̾�α� edit box ���� �ҷ�����
		UpdateData(TRUE);
		CStringA tmpS(expression_editbox);
		//���Ͽ� ����
		savefile.Write(tmpS, tmpS.GetLength());

		//���� �ݱ�
		savefile.Close();

	}



}


void CTOY2Dlg::OnBnClickedButton5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//���� �޼��� ����(Oncancel ȣ��)
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_APP_EXIT, NULL);
}


void CTOY2Dlg::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (MessageBox(_T("�����Ͻðڽ��ϱ�?"), _T(""), MB_YESNO | MB_ICONQUESTION) == IDYES){
		CDialogEx::OnCancel();
	}
}

//DEFUN ��� �ҷ�����
void CTOY2Dlg::load_defun(){
	//���� ��ũ����
	CFile savefile;

	//��θ� ����� ����
	CString Pathname = _T("defun.txt");

	//���� ���� ������ ����
	CStringA tmp;


	//�о�帰 ������ index;
	int index = 0;

	//���� ����
	if (!savefile.Open(Pathname, CFile::modeRead))
	{
		MessageBox(_T("defun.txt ������ �����ϴ�."), _T(""), MB_OK);
		return;
	}

	//CStringA�� char�� ������ ó�� �̿��Ҷ� �ʿ��� char�� ������
	char* wheretmp;

	//���̺� ������ ũ��
	int lenfile = (int)savefile.GetLength();

	//CString class�� char�迭 ó�� ���
	//���̺� ���� ũ�� ��ŭ ���� ��
	wheretmp = tmp.GetBuffer(lenfile + 1);

	//���� ���θ� ���ۿ� ��
	savefile.Read(wheretmp, lenfile);


	//�������� NULL�־���
	wheretmp[lenfile] = NULL;

	//���� �ݱ�
	savefile.Close();

	//���� ���� ����
	tmp.ReleaseBuffer();

	//�ٺ��� �и��� DEFUN�� �Է��ϱ�
	int nTokenPos = 0;
	CStringA strToken = tmp.Tokenize("\r\n", nTokenPos);


	while (!strToken.IsEmpty())
	{
		CStringA tmpdefun;
		tmpdefun.Format("(DEFUN %s)", strToken);
		DEFUN a(tmpdefun);
		//�ùٸ� defun�ĸ� defun_list�� ����
		if (a.IsFunctionOk() == 1){
			defun_list.push_back(a);
		}
		strToken = tmp.Tokenize("\r\n\0", nTokenPos);
	}

}

//�������ϱ�
void CTOY2Dlg::compile(CStringA filename){

	//���� ��ũ����
	CFile savefile;

	//��θ� ����� ����
	CString Pathname(filename);

	//���� ���� ������ ����
	CStringA tmp;


	//�о�帰 ������ index;
	int index = 0;

	//���� ����
	if (!savefile.Open(Pathname, CFile::modeRead))
	{
		MessageBox(_T("������ �����ϴ�."), _T(""), MB_OK);
		return;
	}

	//CStringA�� char�� ������ ó�� �̿��Ҷ� �ʿ��� char�� ������
	char* wheretmp;

	//���̺� ������ ũ��
	int lenfile = (int)savefile.GetLength();

	//CString class�� char�迭 ó�� ���
	//���̺� ���� ũ�� ��ŭ ���� ��
	wheretmp = tmp.GetBuffer(lenfile + 1);

	//���� ���θ� ���ۿ� ��
	savefile.Read(wheretmp, lenfile);


	//�������� NULL�־���
	wheretmp[lenfile] = NULL;

	//���� �ݱ�
	savefile.Close();

	//���� ���� ����
	tmp.ReleaseBuffer();


	//if�� ���̳ʽ��� �ִ� ���α׷����� ����
	ProcessDescription pd(tmp, defun_list);
	
	//���� �� ���������� �ٲٱ�
	std::list<CStringA> program_list = pd.get_program_list();
	CStringA tmpmessage;
	for (std::list<CStringA>::iterator it = program_list.begin(); it != program_list.end(); ++it){
		tmpmessage.Append(*it);
		tmpmessage.Append("\r\n");
	}
	CString tmptmp(tmpmessage);
	PrefixToPostfix postfixproram(tmptmp);
	PostfixToIntermediate intermediate(program_list);//PostfixToIntermediate intermediate(postfixproram.getPostfix());
	//�޼����� ���

	//���� ���̾�α� edit box ���� �ҷ�����
	UpdateData(TRUE);
	tmpmessage.Format("%s", pd.get_message());
	CString tmpmessage2(tmpmessage);
	intermediate_code_editbox.Format(_T("%s\r\n"), tmpmessage2);
	intermediate_code_editbox.AppendFormat(_T("**********Prefix To Postfix***********\r\n"));
	intermediate_code_editbox.AppendFormat(_T("%s\r\n"), postfixproram.getMessage_Postfix());
	intermediate_code_editbox.AppendFormat(_T("**********Postfix To Intermediate***********\r\n"));
	CString tmpmessage3(intermediate.getIntermediate());
	intermediate_code_editbox.AppendFormat(_T("%s\r\n"), tmpmessage3);
	// ���̾�α� edit box ���� ����
	result_editbox.Format(_T(""));
	UpdateData(FALSE);
	
	//��������
	{
		filename.Replace(".s", ".o");
		//��θ� ����
		CString Pathname(filename);
		//���� ��ũ����
		CFile savefile;

		//������ ����
		if (!savefile.Open(Pathname, CFile::modeWrite | CFile::modeCreate))
		{
			MessageBox(_T("������ ���� �����ϴ�."), _T(""), MB_OK);
			return;
		}

		CStringA tmpS(intermediate.getIntermediate());
		//���Ͽ� ����
		savefile.Write(tmpS, tmpS.GetLength());

		//���� �ݱ�
		savefile.Close();
	}
}
//�����ϱ�
void CTOY2Dlg::run(CStringA filename){
	
	//���� ��ũ����
	CFile savefile;

	//��θ� ����� ����
	CString Pathname(filename);

	//���� ���� ������ ����
	CStringA tmp;


	//�о�帰 ������ index;
	int index = 0;

	//���� ����
	if (!savefile.Open(Pathname, CFile::modeRead))
	{
		MessageBox(_T("������ �����ϴ�."), _T(""), MB_OK);
		return;
	}

	//CStringA�� char�� ������ ó�� �̿��Ҷ� �ʿ��� char�� ������
	char* wheretmp;

	//���̺� ������ ũ��
	int lenfile = (int)savefile.GetLength();

	//CString class�� char�迭 ó�� ���
	//���̺� ���� ũ�� ��ŭ ���� ��
	wheretmp = tmp.GetBuffer(lenfile + 1);

	//���� ���θ� ���ۿ� ��
	savefile.Read(wheretmp, lenfile);


	//�������� NULL�־���
	wheretmp[lenfile] = NULL;

	//���� �ݱ�
	savefile.Close();

	//���� ���� ����
	tmp.ReleaseBuffer();

	//�߰��ڵ� ȭ�鿡 �����ֱ�
	CString printtmp(tmp);
	intermediate_code_editbox.Format(_T("%s"), printtmp);


	Interpreter in(tmp);
	CString result(in.getResult());

	result_editbox.Format(_T("%s"),result);
	// ���̾�α� edit box ���� ����
	UpdateData(FALSE);

}
//���ο� DEFUN �߰��ϱ�
void CTOY2Dlg::save_new_defun(CStringA defun_command){

	DEFUN a(defun_command);
	//�ùٸ� defun�ĸ� defun_list�� ����
	//���� ���̾�α� edit box ���� �ҷ�����
	UpdateData(TRUE);
	int defun_status = a.IsFunctionOk();
	if (defun_status == 1){
		defun_list.push_back(a);
		view_defun_list();
	}
	else if (defun_status == 2){
		view_defun_list();
		intermediate_code_editbox.Append(_T("�ߺ��Լ��Դϴ�.\r\n"));
	}
	else{
		view_defun_list();
		intermediate_code_editbox.Append(_T("������ ���� �ʽ��ϴ�.\r\n"));
	}
	//������� edit box�� ����

	UpdateData(FALSE);

	//��ũ�� edit box ���������� ������
	intermediate_code_editbox_control.LineScroll(intermediate_code_editbox_control.GetLineCount());

	CStringA save_message;
	//�� �Լ� �����ϱ�
	for (std::list<DEFUN>::iterator it = defun_list.begin(); it != defun_list.end(); ++it){
		save_message.Append(it->getFullLine());
		save_message.Append("\r\n");
	}

	//��θ� ����
	CString Pathname = _T("defun.txt");
	//���� ��ũ����
	CFile savefile;

	//������ ����
	if (!savefile.Open(Pathname, CFile::modeWrite | CFile::modeCreate))
	{
		MessageBox(_T("������ ���� �����ϴ�."), _T(""), MB_OK);
		return;
	}

	savefile.Write(save_message, save_message.GetLength());

	//���� �ݱ�
	savefile.Close();

}

//defun list ����
void CTOY2Dlg::view_defun_list(){

	//���� ���̾�α� edit box ���� �ҷ�����
	UpdateData(TRUE);
	intermediate_code_editbox.Format(_T("*******DEFUN LIST*******\r\n"));
	for (std::list<DEFUN>::iterator it = defun_list.begin(); it != defun_list.end(); ++it){
		CString tmp(it->getFullLine());
		intermediate_code_editbox.Append(tmp);
		intermediate_code_editbox.Append(_T("\r\n"));
	}
	//������� edit box�� ����
	UpdateData(FALSE);

	//��ũ�� edit box ���������� ������
	intermediate_code_editbox_control.LineScroll(intermediate_code_editbox_control.GetLineCount());

}

//��ɾ� ó��
void CTOY2Dlg::process_command_line(){
	
	//���� ���̾�α� edit box ���� �ҷ�����
	UpdateData(TRUE);
	//Ŀ�ǵ���ο� ���� �� �ҷ�����
	CStringA temp_command(command_line);
	command_line.Format(_T("> "));
	//������� edit box�� ����
	UpdateData(FALSE);
	//Ŀ�� �ڷ� ������
	command_line__control.SetFocus();
	command_line__control.SetSel(0, -1);
	command_line__control.SetSel(-1); 

	//> ��ȣ ���ֱ�
	temp_command.Replace(">", "");

	if (temp_command.Find("Compile") != -1){
		//���ϸ� �����
		temp_command.Replace("Compile", "");
		temp_command.Replace(" ", "");
		temp_command.Replace("\t", "");
		temp_command.Replace("\r", "");
		temp_command.Replace("\n", "");
		
		if ( ((temp_command.Find(".s") == -1) && (temp_command.Find(".S") == -1) ) ){
			MessageBoxA(0, "*.s ���ĸ� ������ �����մϴ�.", "", MB_OK);
		}
		else{
			compile(temp_command);
		}
	}
	else if (temp_command.Find("Run") != -1){
		//���ϸ� �����
		temp_command.Replace("Run", "");
		temp_command.Replace(" ", "");
		temp_command.Replace("\t", "");
		temp_command.Replace("\r", "");
		temp_command.Replace("\n", "");
		

		if (((temp_command.Find(".o") == -1) && (temp_command.Find(".O") == -1))){
			MessageBoxA(0, "*.o ���ĸ� ������ �����մϴ�.", "", MB_OK);
		}
		else{
			run(temp_command);
		}

	}
	else if (temp_command.Find("DEFUN") != -1){
		save_new_defun(temp_command);

	}
	else{
		MessageBoxA(0, "�߸��� ��ɾ� �Դϴ�.", "", MB_OK);
	}


}

//��ɾ� �Է� �ޱ�
//���Ϳ� ����
BOOL CTOY2Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN){
		int idCtrl = this->GetFocus()->GetDlgCtrlID();
		if (idCtrl == IDC_EDIT1){
			process_command_line();
			return true;
		}
		else{
			return CDialogEx::PreTranslateMessage(pMsg);
		}
	}
	else{
		return CDialogEx::PreTranslateMessage(pMsg);
	}

	
}

