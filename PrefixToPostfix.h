

// PrefixToPostfix.h : 헤더 파일
// 전위표기식을 후위표기식으로 변환하는 class

#pragma once
#include "afxwin.h"
#include <list>


class PrefixToPostfix{
private:

	//Prefix 가르킬 CStringA형 포인터
	CStringA* Prefix;
	//CStringA를 char형 포인터 처럼 이용할때 필요한 char형 포인터
	char* tmpbuf;
	//문자열 길이 
	int len;

	//가장 최근 연산자
	int current_opnum;
	//올바른 식들을 list형태로 저장하게 된다.
	std::list<CStringA> listofPostfix;

	//오류 메세지 + 올바른 식의 후위표기식
	CStringA message;

	//줄 단위로 임시 저장
	CStringA tmp_str;

	//에러 번호
	//0: 에러 X, 1: 피연산자가 실수 일때, 2: 괄호 잘못사용 되었을때
	//3: 알파벳과 - 제외한 기호 입력 되었을때
	//4: 연산자 자리에 IF와 MINUS를 제외한 기호가 입력되는 경우
	//5: 피연산자에 -연속으로 들어올때
	//6: 숫자뒤에 숫자 아닌게 공백 없이 들어올때
	int errnum;

	//라인의 시작 index
	int linestart;

	//라인의 끝 index
	int lineend;

	//후위표기식으로 만드는 함수
	void make_Postfix();
	//읽은 마지막 index 리턴 err시 해당 errnum 
	//열린 괄호인가?
	int Is_open_parentheses(int index);
	//index에 문자 들어와야함
	//읽은 마지막 index 리턴 err시 해당 errnum 
	//변수인가?
	int Is_val(int index);
	//index에 숫자 들어와야함
	//읽은 마지막 index 리턴 err시 해당 errnum 
	//상수인가?
	int Is_con(int index);
	//index에 문자 들어와야함
	//읽은 마지막 index 리턴 err시 해당 errnum 
	//연산자 인가?
	int Is_op(int index);
	//index에 - 들어와야함
	//읽은 마지막 index 리턴 err시 해당 errnum 
	//-로 시작되는 상수인가?
	int Is_minus_con(int index);


	//에러메세지 출력, 출력후 errnum=0로 만듬
	void printerror();

	//하나의 프로그램이 끝난후 부터 엔터의 index를 리턴한다.
	//만약 프로그램과 엔터 사이에 공백을 제외한 다른 문자가 있을경우 err 체크한다.
	int check_err_after_program(int index);

	//연산자 모아놓은 배열
	char *oper[2];

	//에러 문자열
	char* errnum_string[7];

public:
	//생성자
	PrefixToPostfix(CString Prefix_tmp);
	//소멸자
	~PrefixToPostfix();
	//올바른 식들 후위식으로 넘겨줌
	std::list<CStringA> getPostfix();
	//오류 메세지 + 올바른 식의 후위표기식
	CString getMessage_Postfix();
};

