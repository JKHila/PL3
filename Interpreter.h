
// Interpreter.h : 헤더 파일
// class Interpreter 
// 중간 코드를 받아 계산한다.


#pragma once

#include "afxwin.h"
#include <stack>

class Interpreter
{
private:
	//중간 코드 계산
	void calc();
	//중간코드
	CStringA intermediate;
	//결과 
	CStringA result;

	//연산자 모아놓은 배열
	char *oper[2];

public:
	//중간코드 인자로 생성
	Interpreter(CStringA intermediate);

	//결과 리턴
	CStringA getResult();
};