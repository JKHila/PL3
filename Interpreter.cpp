
// Interpreter.cpp : cpp 파일
// class Interpreter 구현부
// 중간 코드를 받아 계산한다.


#include "Interpreter.h"
#include "stdafx.h"

//중간코드 인자로 생성
Interpreter::Interpreter(CStringA intermediate_tmp){
	//연산자 초기화
	oper[0] = "IF";
	oper[1] = "MINUS";
	intermediate.Append(intermediate_tmp);
	calc();
}

//결과 리턴
CStringA Interpreter::getResult(){
	return result;
}
//계산
void Interpreter::calc()
{
	//전체 길이
	int len = intermediate.GetLength();
	int i;
	//중간 코드 버퍼
	char *intermediate_pointer = intermediate.GetBuffer();

	char *token;
	//아래 두 bool변수가 true가 되면 
	//end 만날때 까지 만나는 토큰 모두 건너 뛰게 된다.
	bool undefined = false;
	bool error_program = true;
	std::stack<int> cal_stack;

	for (i = 0; i < len; i++){
		//라인 한줄
		CStringA line;
		//라인 한줄 버퍼
		char *line_pointer;
		//한줄 읽기
		while (intermediate_pointer[i] != '\n' && intermediate_pointer[i] != NULL){
			line.AppendChar(intermediate_pointer[i]);
			i++;
		}
		line.AppendChar('\n');
		line_pointer = line.GetBuffer();

		//strtok_s 위해 임시 변수
		char *temp_token;

		//한줄 토큰 단위로 읽기
		token = strtok_s(line_pointer, "\r\n ", &temp_token);

		//빈라인
		if (token == NULL){
			continue;
		}

		//시작일때, undefined false, stack 초기화
		if (!strcmp(token, "begin")){
			undefined = false;
			error_program = false;
			//end 없이 begin다시 만나면
			if (!cal_stack.empty()){
				while (!cal_stack.empty()){
					cal_stack.pop();
				}
				result.Append("Error code\r\n");
			}
		}
		//푸쉬 만남
		else if (!strcmp(token, "push")){
			//아직 변수 나오지 않음
			if (!undefined && !error_program){
				//push 뒤 토큰 읽음
				token = strtok_s(NULL, "\r\n ", &temp_token);
				//상수
				if (isdigit(token[0]) || token[0] == '-'){
					cal_stack.push(atoi(token));
				}
				//변수
				else{
					cal_stack.push(0x7FFFFFFF);
					//undefined = true;
				}
			}
		}
		//end 만남
		else if (!strcmp(token, "end")){
			//변수 있을 때
			if (undefined){
				result.Append("Undefined\r\n");
			}
			//잘못된 프로그램
			else if (error_program){
				result.Append("Error code\r\n");
			}
			else{
				int temp = cal_stack.top();
				cal_stack.pop();
				//스택이 다 비워져야 정상
				if (cal_stack.empty()){
					result.AppendFormat("%d\r\n", temp);
				}
				else{
					result.Append("Error code\r\n");
				}
			}
			//스택 남은거 있으면 다 빼냄
			while (!cal_stack.empty()){
				cal_stack.pop();
			}
		}
		//연산자
		else {
			if (!undefined && !error_program){
				int cal1, cal2;
				//IF
				if (!strcmp(token, oper[0])){
					//stack에 최소 2개이상있어야한다.
					if (cal_stack.size() >= 2){
						cal2 = cal_stack.top();
						cal_stack.pop();
						cal1 = cal_stack.top();
						cal_stack.pop();

						if (cal1 == 0x7FFFFFFF){
							undefined = true;
						}

						if (cal1 > 0){
							if (cal2 == 0x7FFFFFFF){
								undefined = true;
							}
							cal_stack.push(cal2);
						}
						else{
							cal_stack.push(0);
						}
					}
					//err시
					else{
						error_program = true;
					}
				}
				//MINUS
				else if (!strcmp(token, oper[1])){
					if (cal_stack.size() >= 2){
						cal2 = cal_stack.top();
						cal_stack.pop();
						cal1 = cal_stack.top();
						cal_stack.pop();
						if ((cal1 == 0x7FFFFFFF) || (cal2 == 0x7FFFFFFF)){
							undefined = true;
						}
						cal_stack.push(cal1 - cal2);
					}
					//err시
					else{
						error_program = true;
					}
				}
				//정의되지 않은 연산자
				else{
					error_program = true;
				}
			}
		}
	}
}
