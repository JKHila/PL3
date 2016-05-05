
// PostfixToIntermediate.cpp : cpp 파일
// 후위표기식을 중간 코드로 변환하는 class
// 생성자 인자로 받는 list<CStringA>에는 형식에 맞게 후위표기식이 들어가 있다.
// 에러 체크는 모두 InfixToPostfix(중위표기식->후위표기식) 에서 한다.


#include "PostfixToIntermediate.h"
#include "stdafx.h"

//생성자
PostfixToIntermediate::PostfixToIntermediate(std::list<CStringA> Postfix_tmp){
	Postfix = Postfix_tmp;
	//연산자 초기화
	oper[0] = "IF";
	oper[1] = "MINUS";

	//중간코드 만들기
	make_Intermediate();
}

//중간 코드 만드는 함수
void PostfixToIntermediate::make_Intermediate(){
	//리스트가 빌때 까지 앞에서 하나 씩 꺼낸다.
	//(CStringA하나당 후위표기식 라인 하나)
	while (!Postfix.empty())
	{
		//앞부분 line에 복사
		CStringA line = Postfix.front().GetString();
		//list item하나 꺼내기
		Postfix.pop_front();
		//라인 길이
		linelen = line.GetLength();
		//CStrinA char*로 사용하기 위해
		linepointer = line.GetBuffer();
		int i = 0;
		//프로그램 시작 부분
		Intermediate.Append("begin\r\n");

		//괄호 인지 확인
		if (linepointer[i] == '('){
			Is_open_parentheses(i);
		}
		//변수 인지 확인
		else if (isalpha(linepointer[i]) || linepointer[i] == '-'){
			Is_val(i);
		}
		//상수 인지 확인
		else if (isdigit(linepointer[i])){
			Is_con(i);
		}
		//프로그램 끝 부분
		Intermediate.Append("end\r\n");
	}
}

//읽은 마지막 index 리턴
//열린 괄호인가?
int PostfixToIntermediate::Is_open_parentheses(int index){
	index++;

	//term1 확인

	//괄호 인지 확인
	if (linepointer[index] == '('){
		index = Is_open_parentheses(index);
	}
	//변수 인지 확인
	else if (isalpha(linepointer[index]) || linepointer[index] == '-'){
		index = Is_val(index);
	}
	//상수 인지 확인
	else if (isdigit(linepointer[index])){
		index = Is_con(index);
	}
	index += 2;

	//term2 확인

	//괄호 인지 확인
	if (linepointer[index] == '('){
		index = Is_open_parentheses(index);
	}
	//변수 인지 확인
	else if (isalpha(linepointer[index]) || linepointer[index] == '-'){
		index = Is_val(index);
	}
	//상수 인지 확인
	else if (isdigit(linepointer[index])){
		index = Is_con(index);
	}
	index += 2;

	//function 확인
	index = Is_op(index);

	return index + 1;
}

//index에 문자 들어와야함
//읽은 마지막 index 리턴
//변수인가?
int PostfixToIntermediate::Is_val(int index){
	int i;
	//push
	Intermediate.Append("push ");
	//변수 마지막 index 찾기
	for (i = index; i < linelen; i++){
		if (linepointer[i] == ' '){
			break;
		}
		else{
			Intermediate.AppendChar(linepointer[i]);
		}
	}
	Intermediate.Append("\r\n");
	return i - 1;
}

//index에 숫자 들어와야함
//읽은 마지막 index 리턴
//상수인가?
int PostfixToIntermediate::Is_con(int index){
	int i;
	//push
	Intermediate.Append("push ");
	//상수 마지막 index 찾기
	for (i = index; i < linelen; i++){
		if (linepointer[i] == ' '){
			break;
		}
		else{
			Intermediate.AppendChar(linepointer[i]);
		}
	}
	Intermediate.Append("\r\n");
	return i - 1;
}

//index에 문자 들어와야함
//읽은 마지막 index 리턴
//연산자 인가?
int PostfixToIntermediate::Is_op(int index){
	int i, j;
	bool correct;
	for (i = 0; i < (sizeof(oper) / 4); i++){
		//연산자의 길이
		int OP_len = strlen(oper[i]);

		//현재 index 와 연산자 길이의 합이 전체 라인 길이보다 작아야된다.
		//안그러면 실제 짧은 연산자인데 긴 연산자 찾다가 메모리 덤프 날수 있음
		if (index + OP_len < linelen){
			correct = true;
			for (j = 0; j < OP_len; j++){
				if (linepointer[index + j] != oper[i][j]){
					correct = false;
					break;
				}
			}
			//해당연산자 일경우
			if (correct){
				Intermediate.Append(oper[i]);
				break;
			}
		}
	}
	Intermediate.Append("\r\n");
	return index + j - 1;
}

//올바른 식들 후위식으로 넘겨줌
CStringA PostfixToIntermediate::getIntermediate(){
	return Intermediate;
}