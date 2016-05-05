

// PostfixToIntermediate.h : 헤더 파일
// 후위표기식을 중간 코드로 변환하는 class
// 생성자 인자로 받는 list<CStringA>에는 형식에 맞게 후위표기식이 들어가 있다.
// 에러 체크는 모두 InfixToPostfix(중위표기식->후위표기식) 에서 한다.

#pragma once
#include "afxwin.h"
#include <list>

class PostfixToIntermediate
{

private:

	//중간 코드
	CStringA Intermediate;

	//생성자 인자로 받은 Postfix 포인터
	std::list<CStringA> Postfix;

	//line 가르키는 포인터
	char * linepointer;

	//line 길이
	int linelen;

	//중간 코드 만드는 함수
	void make_Intermediate();

	//읽은 마지막 index 리턴
	//열린 괄호인가?
	int Is_open_parentheses(int index);

	//index에 문자 들어와야함
	//읽은 마지막 index 리턴
	//변수인가?
	int Is_val(int index);

	//index에 숫자 들어와야함
	//읽은 마지막 index 리턴
	//상수인가?
	int Is_con(int index);

	//index에 문자 들어와야함
	//읽은 마지막 index 리턴
	//연산자 인가?
	int Is_op(int index);

	//연산자 모아놓은 배열
	char *oper[2];

public:
	//생성자
	PostfixToIntermediate(std::list<CStringA> Postfix_tmp);

	//올바른 식들 후위식으로 넘겨줌
	CStringA getIntermediate();
};


