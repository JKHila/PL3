#include "stdafx.h"
#include "DEFUN.h"

//static list define
std::list<DEFUN> DEFUN::defun_list;

//DEFUN함수가 쓰인 라인을 받음
DEFUN::DEFUN(CStringA temp)
{
	temp_buf.Append(temp);
	buf_pointer_len = temp_buf.GetLength();
	buf_pointer = temp_buf.GetBuffer();
	is_recurcive = false;
	//setting() 호출 함수 등록 과정
	setting();

}

//함수 등록하는 과정
void DEFUN::setting(){
	int i = 0;
	right_function = 1;

	//공백 없는데 까지
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}

	//처음 시작 괄호 있는지 확인
	if (buf_pointer[i] != '('){
		right_function = 5;
		return;
	}
	else{
		i++;
	}

	//DEFUN으로 시작하나 체크
	if (strncmp(buf_pointer + i, "DEFUN", 5) != 0){
		right_function = 5;
		return;
	}
	else{
		i += 5;
	}

	//공백 없는데 까지
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}

	//함수명 등록
	if (isalpha(buf_pointer[i]) != 0){
		function_name.AppendChar(buf_pointer[i]);
		i++;
		while (isspace(buf_pointer[i]) == 0 && buf_pointer[i] != '('){
			function_name.AppendChar(buf_pointer[i]);
			i++;
		}
		//중복 함수 검사
		{
			int defun_list_size = defun_list.size();
			for (std::list<DEFUN>::iterator it = defun_list.begin(); it != defun_list.end(); ++it){
				if (function_name.Compare(it->getFunctionName()) == 0){
					right_function = 2;
					return;
				}
			}
		}
		//full_line에 추가
		full_line.Append(function_name);
		full_line.AppendChar(' ');
	}
	else{
		right_function = 5;
		return;
	}


	//공백 없는데 까지
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}

	//시작 괄호 있는지 확인
	if (buf_pointer[i] != '('){
		right_function = 5;
		return;
	}
	else{
		i++;
	}

	//공백 없는데 까지
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}

	//full_line에 추가
	full_line.Append("( ");

	//parameter 등록	
	if (isalpha(buf_pointer[i]) != 0){

		while (buf_pointer[i] != ')'){
			CStringA tmpstring;
			while (isspace(buf_pointer[i]) == 0 && buf_pointer[i] != ')'){
				if (isalpha(buf_pointer[i]) || isdigit(buf_pointer[i])){
					tmpstring.AppendChar(buf_pointer[i]);
					i++;
				}
				else{
					right_function = 3;
					return;
				}
			}

			//중복 parameter 검사
			{
				int parameter_list_size = parameter_list.size();
				for (std::list<CStringA>::iterator it = parameter_list.begin(); it != parameter_list.end(); ++it){
					if (tmpstring.Compare(*it) == 0){
						right_function = 3;
						return;
					}
				}
			}
			//파라미터 리스트에 추가
			parameter_list.push_back(tmpstring);

			//full_line에 추가
			full_line.Append(tmpstring);
			full_line.AppendChar(' ');

			//공백 없는데 까지
			i = skip_whitespace(i);
			if (i == -1){
				right_function = 5;
				return;
			}

		}
	}
	else{
		right_function = 3;
		return;
	}

	//공백 없는데 까지
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}

	//닫는 괄호 있는지 확인
	if (buf_pointer[i] != ')'){
		right_function = 5;
		return;
	}
	else{
		i++;
	}

	//full_line에 추가
	full_line.Append(")");

	//공백 없는데 까지
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}


	//////////////////

	//함수 정의부 에러체크 및 등록
	i = function_start(i);
	if (right_function != 1){
		return;
	}


	//////////////////

	//공백 없는데 까지
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}

	//닫힌 괄호 있는지 확인
	if (buf_pointer[i] != ')'){
		right_function = 5;
		return;
	}
	else{
		i++;
	}

	//공백 없는데 까지
	i = skip_whitespace(i);
	//DEFUN 마지막 괄호로 -1이 아니라면 DEFUN함수뒤에 뭔가 붙어다는 뜻
	if (i != -1){
		right_function = 5;
		return;
	}

	if (right_function == 1)
		defun_list.push_back(*this);

}


//temp_buf 해당 index부터 화이트스페이스가 아닌 곳의 index를 return
int DEFUN::skip_whitespace(int index){
	int i = index;
	if (i >= buf_pointer_len){
		return -1;
	}
	while (isspace(buf_pointer[i]) != 0){
		i++;
		//끝날 때까지 화이트 스페이스만 있으면 -1 리턴
		if (i == buf_pointer_len){
			return -1;
		}
	}
	return i;
}

//함수 확인
int DEFUN::function_start(int i){
	CStringA temp_function_name;
	int cur_parameter_num;
	//열린 괄호 있는지 확인
	if (buf_pointer[i] != '('){
		right_function = 4;
		return i;
	}
	else{
		i++;
	}

	//full_line에 추가
	full_line.Append(" (");
	//함수 정의부에 추가
	process_description.Append(" (");

	//공백 없는데 까지
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return i;
	}

	//사용된 함수 이름 찾기	
	if (isalpha(buf_pointer[i]) != 0){
		while (isspace(buf_pointer[i]) == 0 && buf_pointer[i] != ')'&& buf_pointer[i] != '('){
			temp_function_name.AppendChar(buf_pointer[i]);
			i++;
		}
		full_line.Append(temp_function_name);
		full_line.Append(" ");
		//함수 정의부에 추가
		process_description.Append(temp_function_name);
		process_description.Append(" ");
	}
	else{
		right_function = 4;
		return i;
	}

	//공백 없는데 까지
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return i;
	}

	//함수찾고 parameter 수 확인

	//재귀적 호출
	if (temp_function_name.Compare(function_name) == 0){
		is_recurcive = true;
		cur_parameter_num = parameter_list.size();
	}
	//IF
	else if (temp_function_name.Compare("IF") == 0){
		cur_parameter_num = 2;
	}
	//MINUS
	else if (temp_function_name.Compare("MINUS") == 0){
		cur_parameter_num = 2;
	}
	//현재 정의된 list에서
	else{
		bool find = false;
		for (std::list<DEFUN>::iterator it = defun_list.begin(); it != defun_list.end(); ++it){
			if (temp_function_name.Compare(it->getFunctionName()) == 0){
				cur_parameter_num = it->getNumofParameter();
				find = true;
			}
		}
		//못찾은경우
		if (!find){
			right_function = 5;
			return i;
		}
	}


	for (int j = 0; j < cur_parameter_num; j++){

		if (isalpha(buf_pointer[i]) != 0){
			i = parameter_check(i);
		}
		else if ((isdigit(buf_pointer[i]) != 0) || buf_pointer[i] == '-'){
			i = constant_check(i);
		}
		else if (buf_pointer[i] == '('){
			i = function_start(i);
		}

		//만약 문장이 중간에 끝났다면
		if (i == -1){
			right_function = 5;
			return i;
		}

		//중간에서 잘못됨
		if (right_function != 1){
			return i;
		}
	}

	//닫힌 괄호 있는지 확인
	if (buf_pointer[i] != ')'){
		right_function = 5;
		return i;
	}
	else{
		i++;
	}


	//full_line에 추가
	full_line.Append(")");
	//함수 정의부에 추가
	process_description.Append(")");

	//공백 없는데 까지
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return i;
	}

	return i;

}
//올바른 파라미터인지 확인
int DEFUN::parameter_check(int i){
	//parameter 등록	
	if (isalpha(buf_pointer[i]) != 0){
		CStringA tmpstring;
		while (isspace(buf_pointer[i]) == 0 && buf_pointer[i] != ')' && buf_pointer[i] != '('){
			if (isalpha(buf_pointer[i]) || isdigit(buf_pointer[i])){
				tmpstring.AppendChar(buf_pointer[i]);
				i++;
			}
			else{
				right_function = 4;
				return i;
			}
		}

		//중복 parameter 검사
		{
			bool find = false;
			for (std::list<CStringA>::iterator it = parameter_list.begin(); it != parameter_list.end(); ++it){
				if (tmpstring.Compare(*it) == 0){
					find = true;
					break;
				}
			}
			if (!find){
				right_function = 4;
				return i;
			}
		}
		//full_line에 추가
		full_line.Append(tmpstring);
		full_line.AppendChar(' ');
		//함수 정의부에 추가
		process_description.AppendChar('@');
		process_description.Append(tmpstring);
		process_description.AppendChar(' ');

	}
	else{
		right_function = 4;
		return i;
	}

	//공백 없는데 까지
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return i;
	}

	return i;

}
//상수 인지 확인
int DEFUN::constant_check(int i){

	if (buf_pointer[i] == '-'){
		//full_line에 추가
		full_line.AppendChar(buf_pointer[i]);
		//함수 정의부에 추가
		process_description.AppendChar(buf_pointer[i]);
		//공백 없는데 까지
		i = skip_whitespace(i);
		if (i == -1){
			right_function = 5;
			return i;
		}
		if (buf_pointer[i] == '-'){
			right_function = 4;
			return i;
		}
	}

	while (isdigit(buf_pointer[i]) != 0){
		//full_line에 추가
		full_line.AppendChar(buf_pointer[i]);
		//함수 정의부에 추가
		process_description.AppendChar(buf_pointer[i]);
		i++;
	}

	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return i;
	}

	//full_line에 추가
	full_line.AppendChar(' ');
	//함수 정의부에 추가
	process_description.AppendChar(' ');
	return i;
}



//함수 이름
CStringA DEFUN::getFunctionName(){
	return function_name;
}
//함수 정의
CStringA DEFUN::getProcessDescription(){
	return process_description;
}
//<function name>(<parameter 1>...<parameter n>) <process description>
//형태로 return
CStringA DEFUN::getFullLine(){
	return full_line;
}
//formal parameter 이름을 return
CStringA DEFUN::getParameter(int index){
	int i = 0;
	for (std::list<CStringA>::iterator it = parameter_list.begin(); it != parameter_list.end(); ++it){
		if (index == i)
			return *it;
	}
	CStringA error("@WRONG INDEX");
	return error;
}

//파라미터 수 리턴
int DEFUN::getNumofParameter(){
	return parameter_list.size();
}

//현재 입력한 함수가 올바른 함수인지 아닌지 return
//(1 올바른 함수, 2 중복 함수, 3 parameter 오류, 4 함수 정의부 오류)
int DEFUN::IsFunctionOk(){
	return right_function;
}
//파라미터 리스트를 넘겨주면 process_description에 맞게 바꿔줌 있는 식으로 바꿔서 넘겨줌
CStringA DEFUN::changeIFMINUS(std::list<CStringA> parameter_list_temp){

	CStringA tmp_process_description(process_description);

	if (parameter_list_temp.size() == parameter_list.size()){
		std::list<CStringA>::iterator it = parameter_list.begin();
		std::list<CStringA>::iterator it_temp = parameter_list_temp.begin();
		for (; it != parameter_list.end(); ++it, ++it_temp){
			CStringA tmp_parameter("@");
			tmp_parameter.Append(*it);
			tmp_process_description.Replace(tmp_parameter, *it_temp);
		}
	}
	else{
		CStringA error("error");
		return error;
	}

	//재귀 일때 첫번째 나오는 IF/THEN/ELSE의 condition을 평가해서 음수면
	//참일때 나올 식을 0으로 치환한다.
	if (is_recurcive){
		int charpointer_len = tmp_process_description.GetLength();
		char* charpointer = tmp_process_description.GetBuffer();
		int pointer_i = 0;
		while (charpointer_len - 12>pointer_i){
			if (strncmp(charpointer + pointer_i, "IF/THEN/ELSE", 12) == 0){
				int condition_val;
				pointer_i = pointer_i + 12;
				while (isspace(charpointer[pointer_i]) != 0){
					pointer_i++;
				}
				condition_val = evalu_condition(charpointer + pointer_i);
				//조건식이 거짓일때 즉, if/then/else문의 2번째 파라미터가 쓸일이 없게된다.
				if (condition_val == 0){
					int count_p = 0;
					//조건문 건너기
					if (isspace(charpointer[pointer_i]) == 0 && (charpointer[pointer_i] != '(')){
						while (isspace(charpointer[pointer_i]) == 0){
							pointer_i++;
						}
					}
					else{
						while (1){
							if (charpointer[pointer_i] == '('){
								count_p++;

							}
							else if (charpointer[pointer_i] == ')'){
								count_p--;
							}
							pointer_i++;
							if (count_p == 0){
								break;
							}
						}
					}

					while (isspace(charpointer[pointer_i]) != 0){
						pointer_i++;
					}

					//중간 식 위치 잡기
					int start = pointer_i;
					if (isspace(charpointer[pointer_i]) == 0 && (charpointer[pointer_i] != '(')){
						while (isspace(charpointer[pointer_i]) == 0){
							pointer_i++;
						}
					}
					else{
						while (1){
							if (charpointer[pointer_i] == '('){
								count_p++;

							}
							else if (charpointer[pointer_i] == ')'){
								count_p--;
							}
							pointer_i++;
							if (count_p == 0){
								break;
							}
						}
					}

					//중간 식 0 으로 바꾸기
					int end = pointer_i;
					CStringA A;
					for (int j = 0; j < start; j++){
						A.AppendChar(charpointer[j]);
					}
					A.AppendChar(' ');
					A.AppendChar('0');
					A.AppendChar(' ');
					for (int j = end; j < charpointer_len; j++){
						A.AppendChar(charpointer[j]);
					}
					return A;

				}
				//조건식이 참일때 뒤를 없앰
				else if (condition_val == 1){
					int count_p = 0;
					//조건문 건너기
					if (isspace(charpointer[pointer_i]) == 0 && (charpointer[pointer_i] != '(')){
						while (isspace(charpointer[pointer_i]) == 0){
							pointer_i++;
						}
					}
					else{
						while (1){
							if (charpointer[pointer_i] == '('){
								count_p++;

							}
							else if (charpointer[pointer_i] == ')'){
								count_p--;
							}
							pointer_i++;
							if (count_p == 0){
								break;
							}
						}
					}

					while (isspace(charpointer[pointer_i]) != 0){
						pointer_i++;
					}

					//중간식  건너기
					if (isspace(charpointer[pointer_i]) == 0 && (charpointer[pointer_i] != '(')){
						while (isspace(charpointer[pointer_i]) == 0){
							pointer_i++;
						}
					}
					else{
						while (1){
							if (charpointer[pointer_i] == '('){
								count_p++;

							}
							else if (charpointer[pointer_i] == ')'){
								count_p--;
							}
							pointer_i++;
							if (count_p == 0){
								break;
							}
						}
					}

					while (isspace(charpointer[pointer_i]) != 0){
						pointer_i++;
					}

					//마지막 식 위치 잡기
					int start = pointer_i;
					if (isspace(charpointer[pointer_i]) == 0 && (charpointer[pointer_i] != '(')){
						while (isspace(charpointer[pointer_i]) == 0){
							pointer_i++;
						}
					}
					else{
						while (1){
							if (charpointer[pointer_i] == '('){
								count_p++;

							}
							else if (charpointer[pointer_i] == ')'){
								count_p--;
							}
							pointer_i++;
							if (count_p == 0){
								break;
							}
						}
					}

					//마지막식 0 으로 바꾸기
					int end = pointer_i;
					CStringA A;
					for (int j = 0; j < start; j++){
						A.AppendChar(charpointer[j]);
					}
					A.AppendChar(' ');
					A.AppendChar('0');
					A.AppendChar(' ');
					for (int j = end; j < charpointer_len; j++){
						A.AppendChar(charpointer[j]);
					}
					return A;
				}
				else if (condition_val == -1){
					CStringA error("error");
					return error;
				}

				break;
			}
			else if (strncmp(charpointer + pointer_i, "IF", 2) == 0){
				int condition_val;
				pointer_i = pointer_i + 2;
				while (isspace(charpointer[pointer_i]) != 0){
					pointer_i++;
				}
				condition_val = evalu_condition(charpointer + pointer_i);
				//조건식이 거짓일때 즉, if문의 2번째 파라미터가 쓸일이 없게된다.
				if (condition_val == 0){
					int count_p = 0;
					//조건문 건너기
					if (isspace(charpointer[pointer_i]) == 0 && (charpointer[pointer_i] != '(')){
						while (isspace(charpointer[pointer_i]) == 0){
							pointer_i++;
						}
					}
					else{
						while (1){
							if (charpointer[pointer_i] == '('){
								count_p++;

							}
							else if (charpointer[pointer_i] == ')'){
								count_p--;
							}
							pointer_i++;
							if (count_p == 0){
								break;
							}
						}
					}

					while (isspace(charpointer[pointer_i]) != 0){
						pointer_i++;
					}

					//식 위치 잡기
					int start = pointer_i;
					if (isspace(charpointer[pointer_i]) == 0 && (charpointer[pointer_i] != '(')){
						while (isspace(charpointer[pointer_i]) == 0){
							pointer_i++;
						}
					}
					else{
						while (1){
							if (charpointer[pointer_i] == '('){
								count_p++;

							}
							else if (charpointer[pointer_i] == ')'){
								count_p--;
							}
							pointer_i++;
							if (count_p == 0){
								break;
							}
						}
					}

					//중간 식 0 으로 바꾸기
					int end = pointer_i;
					CStringA A;
					for (int j = 0; j < start; j++){
						A.AppendChar(charpointer[j]);
					}
					A.AppendChar(' ');
					A.AppendChar('0');
					A.AppendChar(' ');
					for (int j = end; j < charpointer_len; j++){
						A.AppendChar(charpointer[j]);
					}
					return A;

				}
				else if (condition_val == -1){
					CStringA error("error");
					return error;
				}

				break;
			}
			else pointer_i++;
		}
	}
	tmp_process_description.ReleaseBuffer();
	return tmp_process_description;
}

//주어진 식의 값을 미리 판단해줌
// condetion에 0, 음수,들어가면 0 변수가 들어가면 -1 리턴, 만약 양수가 들어가면 1
int DEFUN::evalu_condition(char* sentence){
	int i = 0;
	meet_var = false;
	if (isspace(sentence[i]) == 0){
		if (isdigit(sentence[i]) || sentence[i] == '-'){
			int tmp_int = atoi(sentence);
			if (tmp_int > 0){
				return 1;
			}
			else
				return 0;
		}
		if (isalpha(sentence[i]) != 0){
			return -1;
		}
		if (sentence[i] == '('){
			i++;
			while (isspace(sentence[i]) != 0){
				i++;
			}

			if (strncmp(sentence + i, "IF", 2) == 0){
				i += 2;
				int tmp_eval = evalu_condition_if(sentence, i);
				if (meet_var){
					return -1;
				}

				if (tmp_eval > 0){
					return 1;
				}
				else
					return 0;
			}
			else if (strncmp(sentence + i, "MINUS", 2) == 0){
				i += 5;
				int tmp_eval = evalu_condition_minus(sentence, i);
				if (meet_var){
					return -1;
				}
				if (tmp_eval > 0){
					return 1;
				}
				else
					return 0;
			}
		}
	}
	return -1;
}
//minus 계산
int DEFUN::evalu_condition_minus(char *sentence, int &i){
	int first_oprand = 0;
	int second_oprand = 0;
	//변수 만난적 있나 확인
	if (meet_var){
		return -1;
	}
	//공백 뛰기
	while (isspace(sentence[i]) != 0){
		i++;
	}

	//숫자
	if (isdigit(sentence[i]) || sentence[i] == '-'){
		first_oprand = atoi(sentence + i);
		while (isdigit(sentence[i]) || sentence[i] == '-'){
			i++;
		}
	}
	//IF MINUS시 해당 평가하는 함수로 넘기기
	else if (sentence[i] == '('){
		i++;
		while (isspace(sentence[i]) != 0){
			i++;
		}

		if (strncmp(sentence + i, "IF", 2) == 0){
			i += 2;
			first_oprand = evalu_condition_if(sentence, i);
		}
		else if (strncmp(sentence + i, "MINUS", 2) == 0){
			i += 5;
			first_oprand = evalu_condition_minus(sentence, i);
		}
	}
	else{
		meet_var = true;
		return -1;
	}
	//공백뛰기
	while (isspace(sentence[i]) != 0){
		i++;
	}
	//숫자 만남
	if (isdigit(sentence[i]) || sentence[i] == '-'){
		second_oprand = atoi(sentence + i);
		while (isdigit(sentence[i]) || sentence[i] == '-'){
			i++;
		}
	}
	//IF 나 MINUS 만나면 해당 숫자로 뛰기
	else if (sentence[i] == '('){
		i++;
		while (isspace(sentence[i]) != 0){
			i++;
		}

		if (strncmp(sentence + i, "IF", 2) == 0){
			i += 2;
			second_oprand = evalu_condition_if(sentence, i);
		}
		else if (strncmp(sentence + i, "MINUS", 2) == 0){
			i += 5;
			second_oprand = evalu_condition_minus(sentence, i);
		}
	}
	else{
		meet_var = true;
		return -1;
	}
	while (isspace(sentence[i]) != 0){
		i++;
	}
	//닫는 괄호 넘기
	i++;

	return first_oprand - second_oprand;
}
//if 계산
int DEFUN::evalu_condition_if(char *sentence, int &i){
	int first_oprand = 0;
	int second_oprand = 0;
	if (meet_var){
		return -1;
	}
	while (isspace(sentence[i]) != 0){
		i++;
	}

	if (isdigit(sentence[i]) || sentence[i] == '-'){
		first_oprand = atoi(sentence + i);
		while (isdigit(sentence[i]) || sentence[i] == '-'){
			i++;
		}
	}
	else if (sentence[i] == '('){
		i++;
		while (isspace(sentence[i]) != 0){
			i++;
		}

		if (strncmp(sentence + i, "IF", 2) == 0){
			i += 2;
			first_oprand = evalu_condition_if(sentence, i);
		}
		else if (strncmp(sentence + i, "MINUS", 2) == 0){
			i += 5;
			first_oprand = evalu_condition_minus(sentence, i);
		}
	}
	//if 조건에 변수 시 판단 못함
	else{
		meet_var = true;
		return -1;

	}

	while (isspace(sentence[i]) != 0){
		i++;
	}
	if (isdigit(sentence[i]) || sentence[i] == '-'){
		second_oprand = atoi(sentence + i);
		while (isdigit(sentence[i]) || sentence[i] == '-'){
			i++;
		}
	}
	else if (sentence[i] == '('){
		i++;
		while (isspace(sentence[i]) != 0){
			i++;
		}

		if (strncmp(sentence + i, "IF", 2) == 0){
			i += 2;
			second_oprand = evalu_condition_if(sentence, i);
		}
		else if (strncmp(sentence + i, "MINUS", 2) == 0){
			i += 5;
			second_oprand = evalu_condition_minus(sentence, i);
		}
	}
	else{
		while (isspace(sentence[i]) == 0 && sentence[i] != ')'){
			i++;
		}

		meet_var = true;
	}

	while (isspace(sentence[i]) != 0){
		i++;
	}
	//닫는 괄호 넘기
	i++;

	if (first_oprand > 0){
		if (meet_var){
			return -1;
		}
		return second_oprand;
	}
	else{
		meet_var = false;
		return 0;
	}
}