
// TOY2Dlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include <list>

// CTOY2Dlg 대화 상자
class CTOY2Dlg : public CDialogEx
{
// 생성입니다.
public:
	CTOY2Dlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOY2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// <수식입력>
	CString expression_editbox;
	//<표기법 변환/중간코드>
	CString intermediate_code_editbox;
	//<결과값 출력>
	CString result_editbox;
	// <수식입력> 콘트롤
	CEdit expression_editbox_control;
	// <표기법 변환/중간코드/defun 목록> 콘트롤
	CEdit intermediate_code_editbox_control;
	// <결과값 출력> 콘트롤
	CEdit result_editbox_control;
	//<<command line>>
	CString command_line;
	//command line control
	CEdit command_line__control;
	//화면지우기
	afx_msg void OnBnClickedButton1();
	//함수목록
	afx_msg void OnBnClickedButton2();
	//프로그램로드
	afx_msg void OnBnClickedButton3();
	//프로그램저장
	afx_msg void OnBnClickedButton4();
	//끝내기
	afx_msg void OnBnClickedButton5();
	//종료 처리
	virtual void OnCancel();

	//명령어 입력 받기
	//엔터에 반응
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//명령어 처리
	void process_command_line();

	//DEFUN 목록 불러오기
	void load_defun();
	//새로운 DEFUN 추가하기
	void save_new_defun(CStringA defun_command);
	//컴파일하기
	void compile(CStringA filename);
	//실행하기
	void run(CStringA filename);
	//defun list 보기
	void view_defun_list();

	//defun을 list로 만들어서 저장
	std::list<DEFUN> defun_list;

};
