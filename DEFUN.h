#pragma once

#include <list>
#include "afxwin.h"


//DEFUN �Լ� ����Ǵ� class

class DEFUN
{
private:
	//�Լ� �̸� ����
	CStringA function_name;
	//�Ķ���� ����Ʈ�� ����
	std::list<CStringA> parameter_list;
	//�Լ� ���Ǻ�
	CStringA process_description;
	//�Լ� �̸�, �ĸ�����, �Լ� ���Ǻ�
	CStringA full_line;
	//�ùٸ� �Լ����� üũ(1 �ùٸ� �Լ�, 2 �ߺ� �Լ�, 3 parameter ����, 4 �Լ� ���Ǻ� ���� 5 ��Ÿ����)
	int right_function;
	//���ǵ� �Լ� ����Ʈ staic���� ����
	static std::list<DEFUN> defun_list;

	//�ӽ� CstringA
	CStringA temp_buf;
	//�ӽ� char*
	char* buf_pointer;
	//buf_pointer ����
	int buf_pointer_len;
	//����Լ����� check
	bool is_recurcive;

	//�Լ� ��� �ϴ� ����
	void setting();

	//�Լ� Ȯ��
	int function_start(int n);
	//�ùٸ� �Ķ�������� Ȯ��
	int parameter_check(int n);
	//��� ���� Ȯ��
	int constant_check(int n);

	//temp_buf �ش� index���� ȭ��Ʈ�����̽��� �ƴ� ���� index�� return
	int skip_whitespace(int index);


	//���� ������ Ȯ��
	bool meet_var;

	//�־��� ���� ���� �̸� �Ǵ�����
	// condetion�� 0, ����,���� 0 ������ ���� -1 ����, ���� ����� ���� 1
	int evalu_condition(char* sentence);
	//minus ���
	int evalu_condition_minus(char *sentence, int &index);
	//if ���
	int evalu_condition_if(char *sentence, int &index);

public:
	//DEFUN�Լ��� ���� ������ ����
	DEFUN(CStringA temp);
	//�Լ� �̸�
	CStringA getFunctionName();
	//�Լ� ����
	CStringA getProcessDescription();
	//<function name>(<parameter 1>...<parameter n>) <process description>
	//���·� return
	CStringA getFullLine();
	//formal parameter �̸��� return
	CStringA getParameter(int index);

	//�Ķ���� �� ����
	int getNumofParameter();

	//���� �Է��� �Լ��� �ùٸ� �Լ����� �ƴ��� return
	//(1 �ùٸ� �Լ�, 2 �ߺ� �Լ�, 3 parameter ����, 4 �Լ� ���Ǻ� ����)
	int IsFunctionOk();
	//�Ķ���� ����Ʈ�� �Ѱ��ָ� process_description�� �°� �ٲ��� �ִ� ������ �ٲ㼭 �Ѱ���
	CStringA changeIFMINUS(std::list<CStringA> parameter_list_temp);


	//defun_list �ʱ�ȭ
	static void clearDefunList(){
		defun_list.clear();
	}
	//�������� ���ǵ� defun�� �̸� ����, index�� �Էµ� ����
	static CStringA getDefunName(int index){
		int i = 0;
		for (std::list<DEFUN>::iterator it = defun_list.begin(); it != defun_list.end(); ++it){
			if (index == i)
				return (*it).function_name;
		}
		CStringA error("@WRONG INDEX");
		return error;
	}
	//�������� ���ǵ� defun�� �Ķ���� �� ����, index�� �Էµ� ����
	static int getDefun_Parameter_num(int index){
		int i = 0;
		for (std::list<DEFUN>::iterator it = defun_list.begin(); it != defun_list.end(); ++it){
			if (index == i)
				return (*it).getNumofParameter();
		}
		return -1;
	}
	//defun�� �� ����
	static int getNumofDefun(){
		return defun_list.size();
	}

};
