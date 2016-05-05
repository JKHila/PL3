
// TOY2Dlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include <list>

// CTOY2Dlg ��ȭ ����
class CTOY2Dlg : public CDialogEx
{
// �����Դϴ�.
public:
	CTOY2Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TOY2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// <�����Է�>
	CString expression_editbox;
	//<ǥ��� ��ȯ/�߰��ڵ�>
	CString intermediate_code_editbox;
	//<����� ���>
	CString result_editbox;
	// <�����Է�> ��Ʈ��
	CEdit expression_editbox_control;
	// <ǥ��� ��ȯ/�߰��ڵ�/defun ���> ��Ʈ��
	CEdit intermediate_code_editbox_control;
	// <����� ���> ��Ʈ��
	CEdit result_editbox_control;
	//<<command line>>
	CString command_line;
	//command line control
	CEdit command_line__control;
	//ȭ�������
	afx_msg void OnBnClickedButton1();
	//�Լ����
	afx_msg void OnBnClickedButton2();
	//���α׷��ε�
	afx_msg void OnBnClickedButton3();
	//���α׷�����
	afx_msg void OnBnClickedButton4();
	//������
	afx_msg void OnBnClickedButton5();
	//���� ó��
	virtual void OnCancel();

	//��ɾ� �Է� �ޱ�
	//���Ϳ� ����
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//��ɾ� ó��
	void process_command_line();

	//DEFUN ��� �ҷ�����
	void load_defun();
	//���ο� DEFUN �߰��ϱ�
	void save_new_defun(CStringA defun_command);
	//�������ϱ�
	void compile(CStringA filename);
	//�����ϱ�
	void run(CStringA filename);
	//defun list ����
	void view_defun_list();

	//defun�� list�� ���� ����
	std::list<DEFUN> defun_list;

};
