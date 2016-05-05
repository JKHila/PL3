#pragma once


#include "afxwin.h"
#include <list>
#include "DEFUN.h"

//����� ���� �Լ�,COND�� IF,MINUS ������
class ProcessDescription
{
private:

	//if�� minus�θ� �̷���� �������� Ȯ��
	bool only_if_minus;
	//�ش� parameter�� if / minus�θ� �̷����� üũ
	bool only_if_minus_parameter;
	//���� üũ
	bool check_error;
	//�ӽ� CstringA
	CStringA temp_buffer;
	//���α׷� ����Ʈ�� ����
	std::list<CStringA> program_list;
	//��ȯ�� ���α׷� ����Ʈ
	std::list<CStringA> after_program_list;
	//�޼��� (��ȯ�� ���α׷� ����Ʈ + ���� �޼���)
	CStringA message;

	//�ӽÿ�
	CStringA* tmpbuf_CStringA;
	char * buf;
	int size;

	//defun list
	std::list<DEFUN> defun_list;

	//if Ȥ�� minus�� ��ȯ ��Ű�� �Լ�
	int program_convert(int i);
	//���� �ٱ�
	int skip_whitespace(int index);

	// COND�� -> if then else �������� �ٲ��� -  COND �Ͻ� true  �ƴϸ� false����
	bool cond_process(std::list<CStringA>::iterator &it);

public:
	//������
	ProcessDescription(CStringA buf, std::list<DEFUN> tmp_defun_list);
	//��ȯ�� ���α׷� ����Ʈ
	std::list<CStringA> get_program_list();
	//�޼���
	CStringA get_message();
};


