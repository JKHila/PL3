#pragma once

#include <list>
#include "afxwin.h"


//DEFUN 함수 저장되는 class

class DEFUN
{
private:
	//함수 이름 저장
	CStringA function_name;
	//파라미터 리스트로 저장
	std::list<CStringA> parameter_list;
	//함수 정의부
	CStringA process_description;
	//함수 이름, 파리미터, 함수 정의부
	CStringA full_line;
	//올바른 함수인지 체크(1 올바른 함수, 2 중복 함수, 3 parameter 오류, 4 함수 정의부 오류 5 기타오류)
	int right_function;
	//정의된 함수 리스트 staic으로 저장
	static std::list<DEFUN> defun_list;

	//임시 CstringA
	CStringA temp_buf;
	//임시 char*
	char* buf_pointer;
	//buf_pointer 길이
	int buf_pointer_len;
	//재귀함수인지 check
	bool is_recurcive;

	//함수 등록 하는 과정
	void setting();

	//함수 확인
	int function_start(int n);
	//올바른 파라미터인지 확인
	int parameter_check(int n);
	//상수 인지 확인
	int constant_check(int n);

	//temp_buf 해당 index부터 화이트스페이스가 아닌 곳의 index를 return
	int skip_whitespace(int index);


	//변수 만났나 확인
	bool meet_var;

	//주어진 식의 값을 미리 판단해줌
	// condetion에 0, 음수,들어가면 0 변수가 들어가면 -1 리턴, 만약 양수가 들어가면 1
	int evalu_condition(char* sentence);
	//minus 계산
	int evalu_condition_minus(char *sentence, int &index);
	//if 계산
	int evalu_condition_if(char *sentence, int &index);

public:
	//DEFUN함수가 쓰인 라인을 받음
	DEFUN(CStringA temp);
	//함수 이름
	CStringA getFunctionName();
	//함수 정의
	CStringA getProcessDescription();
	//<function name>(<parameter 1>...<parameter n>) <process description>
	//형태로 return
	CStringA getFullLine();
	//formal parameter 이름을 return
	CStringA getParameter(int index);

	//파라미터 수 리턴
	int getNumofParameter();

	//현재 입력한 함수가 올바른 함수인지 아닌지 return
	//(1 올바른 함수, 2 중복 함수, 3 parameter 오류, 4 함수 정의부 오류)
	int IsFunctionOk();
	//파라미터 리스트를 넘겨주면 process_description에 맞게 바꿔줌 있는 식으로 바꿔서 넘겨줌
	CStringA changeIFMINUS(std::list<CStringA> parameter_list_temp);


	//defun_list 초기화
	static void clearDefunList(){
		defun_list.clear();
	}
	//이제까지 정의된 defun의 이름 리턴, index는 입력된 순서
	static CStringA getDefunName(int index){
		int i = 0;
		for (std::list<DEFUN>::iterator it = defun_list.begin(); it != defun_list.end(); ++it){
			if (index == i)
				return (*it).function_name;
		}
		CStringA error("@WRONG INDEX");
		return error;
	}
	//이제까지 정의된 defun의 파라미터 수 리턴, index는 입력된 순서
	static int getDefun_Parameter_num(int index){
		int i = 0;
		for (std::list<DEFUN>::iterator it = defun_list.begin(); it != defun_list.end(); ++it){
			if (index == i)
				return (*it).getNumofParameter();
		}
		return -1;
	}
	//defun의 수 리턴
	static int getNumofDefun(){
		return defun_list.size();
	}

};
