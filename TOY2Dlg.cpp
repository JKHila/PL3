
// TOY2Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "TOY2.h"
#include "TOY2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CTOY2Dlg 대화 상자



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


// CTOY2Dlg 메시지 처리기

BOOL CTOY2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.


	//현재 다이얼로그 edit box 상태 불러오기
	UpdateData(TRUE);

	command_line.Format(_T("> "));

	//변경사항 edit box에 저장
	UpdateData(FALSE);



	//defun파일 불러오기
	load_defun();



	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTOY2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTOY2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//화면 내용 지우기
//명령어 줄을 지우지 않음
void CTOY2Dlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//현재 다이얼로그 edit box 상태 불러오기
	UpdateData(TRUE);
	// <수식입력> 초기화
	expression_editbox.Format(_T(""));
	//<표기법 변환/중간코드>
	intermediate_code_editbox.Format(_T(""));
	//<결과값 출력>
	result_editbox.Format(_T(""));
	//변경 내용 적용
	UpdateData(FALSE);


}


//DEFUN LIST 보여주기
//파일에서 불러오는것이 아니라
//프로그램 상에 올라온 DEFUN LIST를 보여준다.
//프로그램 시작시 한번만 부르게 된다.
//새로 추가한것도 바로바로 파일에 저장된다.
void CTOY2Dlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//defun list보기
	view_defun_list();
}

//프로그램로드
void CTOY2Dlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.




	//불러올 경로 지정하기 위한 CFileDiaglog형 class 
	CFileDialog fDlg(true, _T("s"), _T("program.s"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("TOY프로그램소스파일(*.s)|*.s|"));

	//파일 경로를 불러오는 다이얼로그 호출
	//정상적으로 파일 경로가 얻어 졌으면
	//파일 불러오기시작
	if (IDOK == fDlg.DoModal())
	{
		//파일 디스크립터
		CFile savefile;

		//경로명 저장될 변수
		CString Pathname = fDlg.GetPathName();

		//파일 내용 저장할 버퍼
		CStringA tmp;


		//읽어드린 버퍼의 index;
		int index = 0;

		//파일 열기
		if (!savefile.Open(Pathname, CFile::modeRead))
		{
			MessageBox(_T("파일을 열수 없습니다."), _T(""), MB_OK);
			return;
		}

		//CStringA를 char형 포인터 처럼 이용할때 필요한 char형 포인터
		char* wheretmp;

		//세이브 파일의 크기
		int lenfile = (int)savefile.GetLength();

		//CString class를 char배열 처럼 사용
		//세이브 파일 크기 만큼 버퍼 염
		wheretmp = tmp.GetBuffer(lenfile + 1);

		//파일 전부를 버퍼에 씀
		savefile.Read(wheretmp, lenfile);


		//마지막에 NULL넣어줌
		wheretmp[lenfile] = NULL;

		//파일 닫기
		savefile.Close();

		//남은 공간 해제
		tmp.ReleaseBuffer();

		//CStringA를 CString형으로
		CString tmp2(tmp);


		//현재 다이얼로그 edit box 상태 불러오기
		UpdateData(TRUE);
		//기존 입력된 문자가 있을시 이어 붙일지 아니면 지우고 새로 불러올지 정할수 있다.
		if ((expression_editbox.GetLength() != 0)){
			if (MessageBox(_T("입력된 문자열이 있습니다\n이어 붙이시겠습니까?"), _T(""), MB_YESNO | MB_ICONQUESTION) == IDNO){
				expression_editbox.Format(_T(""));
			}
			//이어 붙일때 개행한번 해주고 이어 붙임
			else{
				expression_editbox.Append(_T("\r\n"));
			}
		}
		//읽어온 파일을 edit box에 기록한다.
		expression_editbox.Append(tmp2);

		//변경된 edit box에 적용
		UpdateData(FALSE);

		//스크롤 edit box 마지막으로 보내기

	}



}

//프로그램저장
void CTOY2Dlg::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//현재 다이얼로그 edit box 상태 불러오기
	UpdateData(TRUE);
	if (expression_editbox.GetLength() == 0){
		MessageBox(_T("저장할것이 없습니다."), _T(""), MB_OK);
		return;
	}

	//파일 경로 불러오기 위한 CFileDialog class 
	CFileDialog fDlg(false, _T("s"), _T("Program.s"), OFN_OVERWRITEPROMPT, _T("TOY프로그램소스파일(*.s)|*.s|"));

	//다이얼로그 호출
	if (IDOK == fDlg.DoModal())
	{
		//경로명 저장
		CString Pathname = fDlg.GetPathName();
		//파일 디스크립터
		CFile savefile;

		//파일을 열기
		if (!savefile.Open(Pathname, CFile::modeWrite | CFile::modeCreate))
		{
			MessageBox(_T("파일을 열수 없습니다."), _T(""), MB_OK);
			return;
		}
		//현재 다이얼로그 edit box 상태 불러오기
		UpdateData(TRUE);
		CStringA tmpS(expression_editbox);
		//파일에 쓰기
		savefile.Write(tmpS, tmpS.GetLength());

		//파일 닫기
		savefile.Close();

	}



}


void CTOY2Dlg::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//종료 메세지 전달(Oncancel 호출)
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_APP_EXIT, NULL);
}


void CTOY2Dlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (MessageBox(_T("종료하시겠습니까?"), _T(""), MB_YESNO | MB_ICONQUESTION) == IDYES){
		CDialogEx::OnCancel();
	}
}

//DEFUN 목록 불러오기
void CTOY2Dlg::load_defun(){
	//파일 디스크립터
	CFile savefile;

	//경로명 저장될 변수
	CString Pathname = _T("defun.txt");

	//파일 내용 저장할 버퍼
	CStringA tmp;


	//읽어드린 버퍼의 index;
	int index = 0;

	//파일 열기
	if (!savefile.Open(Pathname, CFile::modeRead))
	{
		MessageBox(_T("defun.txt 파일이 없습니다."), _T(""), MB_OK);
		return;
	}

	//CStringA를 char형 포인터 처럼 이용할때 필요한 char형 포인터
	char* wheretmp;

	//세이브 파일의 크기
	int lenfile = (int)savefile.GetLength();

	//CString class를 char배열 처럼 사용
	//세이브 파일 크기 만큼 버퍼 염
	wheretmp = tmp.GetBuffer(lenfile + 1);

	//파일 전부를 버퍼에 씀
	savefile.Read(wheretmp, lenfile);


	//마지막에 NULL넣어줌
	wheretmp[lenfile] = NULL;

	//파일 닫기
	savefile.Close();

	//남은 공간 해제
	tmp.ReleaseBuffer();

	//줄별로 분리해 DEFUN에 입력하기
	int nTokenPos = 0;
	CStringA strToken = tmp.Tokenize("\r\n", nTokenPos);


	while (!strToken.IsEmpty())
	{
		CStringA tmpdefun;
		tmpdefun.Format("(DEFUN %s)", strToken);
		DEFUN a(tmpdefun);
		//올바른 defun식만 defun_list에 넣음
		if (a.IsFunctionOk() == 1){
			defun_list.push_back(a);
		}
		strToken = tmp.Tokenize("\r\n\0", nTokenPos);
	}

}

//컴파일하기
void CTOY2Dlg::compile(CStringA filename){

	//파일 디스크립터
	CFile savefile;

	//경로명 저장될 변수
	CString Pathname(filename);

	//파일 내용 저장할 버퍼
	CStringA tmp;


	//읽어드린 버퍼의 index;
	int index = 0;

	//파일 열기
	if (!savefile.Open(Pathname, CFile::modeRead))
	{
		MessageBox(_T("파일이 없습니다."), _T(""), MB_OK);
		return;
	}

	//CStringA를 char형 포인터 처럼 이용할때 필요한 char형 포인터
	char* wheretmp;

	//세이브 파일의 크기
	int lenfile = (int)savefile.GetLength();

	//CString class를 char배열 처럼 사용
	//세이브 파일 크기 만큼 버퍼 염
	wheretmp = tmp.GetBuffer(lenfile + 1);

	//파일 전부를 버퍼에 씀
	savefile.Read(wheretmp, lenfile);


	//마지막에 NULL넣어줌
	wheretmp[lenfile] = NULL;

	//파일 닫기
	savefile.Close();

	//남은 공간 해제
	tmp.ReleaseBuffer();


	//if와 마이너스만 있는 프로그램으로 변경
	ProcessDescription pd(tmp, defun_list);
	
	//전위 식 후위식으로 바꾸기
	std::list<CStringA> program_list = pd.get_program_list();
	CStringA tmpmessage;
	for (std::list<CStringA>::iterator it = program_list.begin(); it != program_list.end(); ++it){
		tmpmessage.Append(*it);
		tmpmessage.Append("\r\n");
	}
	CString tmptmp(tmpmessage);
	PrefixToPostfix postfixproram(tmptmp);
	PostfixToIntermediate intermediate(postfixproram.getPostfix());
	//메세지들 출력

	//현재 다이얼로그 edit box 상태 불러오기
	UpdateData(TRUE);
	tmpmessage.Format("%s", pd.get_message());
	CString tmpmessage2(tmpmessage);
	intermediate_code_editbox.Format(_T("%s\r\n"), tmpmessage2);
	intermediate_code_editbox.AppendFormat(_T("**********Prefix To Postfix***********\r\n"));
	intermediate_code_editbox.AppendFormat(_T("%s\r\n"), postfixproram.getMessage_Postfix());
	intermediate_code_editbox.AppendFormat(_T("**********Postfix To Intermediate***********\r\n"));
	CString tmpmessage3(intermediate.getIntermediate());
	intermediate_code_editbox.AppendFormat(_T("%s\r\n"), tmpmessage3);
	// 다이얼로그 edit box 변경 적용
	result_editbox.Format(_T(""));
	UpdateData(FALSE);
	
	//파일저장
	{
		filename.Replace(".s", ".o");
		//경로명 저장
		CString Pathname(filename);
		//파일 디스크립터
		CFile savefile;

		//파일을 열기
		if (!savefile.Open(Pathname, CFile::modeWrite | CFile::modeCreate))
		{
			MessageBox(_T("파일을 열수 없습니다."), _T(""), MB_OK);
			return;
		}

		CStringA tmpS(intermediate.getIntermediate());
		//파일에 쓰기
		savefile.Write(tmpS, tmpS.GetLength());

		//파일 닫기
		savefile.Close();
	}
}
//실행하기
void CTOY2Dlg::run(CStringA filename){
	
	//파일 디스크립터
	CFile savefile;

	//경로명 저장될 변수
	CString Pathname(filename);

	//파일 내용 저장할 버퍼
	CStringA tmp;


	//읽어드린 버퍼의 index;
	int index = 0;

	//파일 열기
	if (!savefile.Open(Pathname, CFile::modeRead))
	{
		MessageBox(_T("파일이 없습니다."), _T(""), MB_OK);
		return;
	}

	//CStringA를 char형 포인터 처럼 이용할때 필요한 char형 포인터
	char* wheretmp;

	//세이브 파일의 크기
	int lenfile = (int)savefile.GetLength();

	//CString class를 char배열 처럼 사용
	//세이브 파일 크기 만큼 버퍼 염
	wheretmp = tmp.GetBuffer(lenfile + 1);

	//파일 전부를 버퍼에 씀
	savefile.Read(wheretmp, lenfile);


	//마지막에 NULL넣어줌
	wheretmp[lenfile] = NULL;

	//파일 닫기
	savefile.Close();

	//남은 공간 해제
	tmp.ReleaseBuffer();

	//중간코드 화면에 보여주기
	CString printtmp(tmp);
	intermediate_code_editbox.Format(_T("%s"), printtmp);


	Interpreter in(tmp);
	CString result(in.getResult());

	result_editbox.Format(_T("%s"),result);
	// 다이얼로그 edit box 변경 적용
	UpdateData(FALSE);

}
//새로운 DEFUN 추가하기
void CTOY2Dlg::save_new_defun(CStringA defun_command){

	DEFUN a(defun_command);
	//올바른 defun식만 defun_list에 넣음
	//현재 다이얼로그 edit box 상태 불러오기
	UpdateData(TRUE);
	int defun_status = a.IsFunctionOk();
	if (defun_status == 1){
		defun_list.push_back(a);
		view_defun_list();
	}
	else if (defun_status == 2){
		view_defun_list();
		intermediate_code_editbox.Append(_T("중복함수입니다.\r\n"));
	}
	else{
		view_defun_list();
		intermediate_code_editbox.Append(_T("문법에 맞지 않습니다.\r\n"));
	}
	//변경사항 edit box에 저장

	UpdateData(FALSE);

	//스크롤 edit box 마지막으로 보내기
	intermediate_code_editbox_control.LineScroll(intermediate_code_editbox_control.GetLineCount());

	CStringA save_message;
	//새 함수 저장하기
	for (std::list<DEFUN>::iterator it = defun_list.begin(); it != defun_list.end(); ++it){
		save_message.Append(it->getFullLine());
		save_message.Append("\r\n");
	}

	//경로명 저장
	CString Pathname = _T("defun.txt");
	//파일 디스크립터
	CFile savefile;

	//파일을 열기
	if (!savefile.Open(Pathname, CFile::modeWrite | CFile::modeCreate))
	{
		MessageBox(_T("파일을 열수 없습니다."), _T(""), MB_OK);
		return;
	}

	savefile.Write(save_message, save_message.GetLength());

	//파일 닫기
	savefile.Close();

}

//defun list 보기
void CTOY2Dlg::view_defun_list(){

	//현재 다이얼로그 edit box 상태 불러오기
	UpdateData(TRUE);
	intermediate_code_editbox.Format(_T("*******DEFUN LIST*******\r\n"));
	for (std::list<DEFUN>::iterator it = defun_list.begin(); it != defun_list.end(); ++it){
		CString tmp(it->getFullLine());
		intermediate_code_editbox.Append(tmp);
		intermediate_code_editbox.Append(_T("\r\n"));
	}
	//변경사항 edit box에 저장
	UpdateData(FALSE);

	//스크롤 edit box 마지막으로 보내기
	intermediate_code_editbox_control.LineScroll(intermediate_code_editbox_control.GetLineCount());

}

//명령어 처리
void CTOY2Dlg::process_command_line(){
	
	//현재 다이얼로그 edit box 상태 불러오기
	UpdateData(TRUE);
	//커맨드라인에 쓰인 것 불러오기
	CStringA temp_command(command_line);
	command_line.Format(_T("> "));
	//변경사항 edit box에 저장
	UpdateData(FALSE);
	//커서 뒤로 보내기
	command_line__control.SetFocus();
	command_line__control.SetSel(0, -1);
	command_line__control.SetSel(-1); 

	//> 기호 없애기
	temp_command.Replace(">", "");

	if (temp_command.Find("Compile") != -1){
		//파일명만 남기기
		temp_command.Replace("Compile", "");
		temp_command.Replace(" ", "");
		temp_command.Replace("\t", "");
		temp_command.Replace("\r", "");
		temp_command.Replace("\n", "");
		
		if ( ((temp_command.Find(".s") == -1) && (temp_command.Find(".S") == -1) ) ){
			MessageBoxA(0, "*.s 형식만 컴파일 가능합니다.", "", MB_OK);
		}
		else{
			compile(temp_command);
		}
	}
	else if (temp_command.Find("Run") != -1){
		//파일명만 남기기
		temp_command.Replace("Run", "");
		temp_command.Replace(" ", "");
		temp_command.Replace("\t", "");
		temp_command.Replace("\r", "");
		temp_command.Replace("\n", "");
		

		if (((temp_command.Find(".o") == -1) && (temp_command.Find(".O") == -1))){
			MessageBoxA(0, "*.o 형식만 컴파일 가능합니다.", "", MB_OK);
		}
		else{
			run(temp_command);
		}

	}
	else if (temp_command.Find("DEFUN") != -1){
		save_new_defun(temp_command);

	}
	else{
		MessageBoxA(0, "잘못된 명령어 입니다.", "", MB_OK);
	}


}

//명령어 입력 받기
//엔터에 반응
BOOL CTOY2Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
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

