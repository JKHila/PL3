#pragma once


#include "afxwin.h"
#include <list>
#include "DEFUN.h"

//사용자 정의 함수,COND를 IF,MINUS 문으로
class ProcessDescription
{
private:

	//if와 minus로만 이루어진 문장인지 확인
	bool only_if_minus;
	//해당 parameter가 if / minus로만 이뤄졌나 체크
	bool only_if_minus_parameter;
	//에러 체크
	bool check_error;
	//임시 CstringA
	CStringA temp_buffer;
	//프로그램 리스트로 저장
	std::list<CStringA> program_list;
	//변환된 프로그램 리스트
	std::list<CStringA> after_program_list;
	//메세지 (변환된 프로그램 리스트 + 에러 메세지)
	CStringA message;

	//임시용
	CStringA* tmpbuf_CStringA;
	char * buf;
	int size;

	//defun list
	std::list<DEFUN> defun_list;

	//if 혹은 minus로 변환 시키는 함수
	int program_convert(int i);
	//공백 뛰기
	int skip_whitespace(int index);

	// COND를 -> if then else 문장으로 바꿔줌 -  COND 일시 true  아니면 false리턴
	bool cond_process(std::list<CStringA>::iterator &it);

public:
	//생성자
	ProcessDescription(CStringA buf, std::list<DEFUN> tmp_defun_list);
	//변환된 프로그램 리스트
	std::list<CStringA> get_program_list();
	//메세지
	CStringA get_message();
};


