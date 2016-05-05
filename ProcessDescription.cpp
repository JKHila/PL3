#include "stdafx.h"


#include "ProcessDescription.h"


//생성자
ProcessDescription::ProcessDescription(CStringA buffer, std::list<DEFUN> tmp_defun_list)
{
	defun_list = tmp_defun_list;


	int nTokenPos = 0;
	CStringA strToken = buffer.Tokenize("\r\n", nTokenPos);

	while (!strToken.IsEmpty())
	{
		program_list.push_back(strToken);
		strToken = buffer.Tokenize("\r\n\0", nTokenPos);
	}

	message.Append("********Process Description********\r\n");
	//라인별로 하나씩 꺼내 처리
	for (std::list<CStringA>::iterator it = program_list.begin(); it != program_list.end(); ++it){
		//현재 바뀐 문장이 if minus로만 구성되있나 체크
		only_if_minus_parameter = false;
		check_error = false;
		//초기 문장이 if minus로만 구성
		only_if_minus = true;

		size = it->GetLength();
		tmpbuf_CStringA = &(*it);
		buf = it->GetBuffer();

		//cond 확인
		if (!cond_process(it)){
			message.Append("\r\n");
			message.Append(*it);
			temp_buffer.Format("");
		}


		//문장안에 if minus만 남을 때 까지 반복
		while (!only_if_minus_parameter){
			program_convert(0);
			if (check_error){
				break;
			}
			tmpbuf_CStringA->Format("%s", temp_buffer);
			buf = tmpbuf_CStringA->GetBuffer();
			size = tmpbuf_CStringA->GetLength();

		}
		//문장 변환 한 경우
		if (!only_if_minus&& !check_error){
			message.Append("\r\n-> ");
			message.Append(temp_buffer);
			after_program_list.push_back(temp_buffer);
		}
		// if minus로만 이루어진 문장
		else if (only_if_minus&& !check_error){
			after_program_list.push_back(temp_buffer);
		}
		//에러
		else if (check_error){
			message.Append("\r\n-> PROGRAM ERROR");
			after_program_list.push_back(*tmpbuf_CStringA);
		}
		message.Append("\r\n");
	}

}


//변환된 프로그램 리스트
std::list<CStringA> ProcessDescription::get_program_list(){
	return after_program_list;
}
//메세지
CStringA ProcessDescription::get_message(){
	return message;
}


//if 혹은 minus로 변환 시키는 함수
int ProcessDescription::program_convert(int i){
	int start_i = i;
	std::list<CStringA> cur_parameter_list;
	CStringA cur_function_tmp;
	//공백 넘기
	i = skip_whitespace(i);
	if (i == -1){
		check_error = true;
		return i;
	}

	if (buf[i] == '('){
		CStringA temp_function_name;
		int cur_parameter_num = 0;
		DEFUN *cur_fun_point = NULL;

		cur_function_tmp.Append("( ");
		i++;
		i = skip_whitespace(i);

		//함수 이름 축출
		if (isalpha(buf[i]) != 0){
			while (isspace(buf[i]) == 0 && buf[i] != '('){
				cur_function_tmp.AppendChar(buf[i]);
				temp_function_name.AppendChar(buf[i]);
				i++;
			}

		}
		else{
			check_error = true;
			return i;
		}
		cur_function_tmp.AppendChar(' ');

		//함수 확인

		//IF
		if (temp_function_name.Compare("IF") == 0){
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
					cur_fun_point = &(*it);
					find = true;
				}
			}
			//못찾은경우
			if (!find){
				check_error = true;
				return i;
			}
		}

		//공백 넘기
		i = skip_whitespace(i);
		if (i == -1){
			check_error = true;
			return i;
		}

		//파라미터 체크

		for (int j = 0; j < cur_parameter_num; j++){
			CStringA curparameter;



			if (isalpha(buf[i]) != 0){
				while (isspace(buf[i]) == 0 && buf[i] != '(' && buf[i] != ')'){
					cur_function_tmp.AppendChar(buf[i]);
					curparameter.AppendChar(buf[i]);
					i++;
				}
			}
			else if ((isdigit(buf[i]) != 0) || buf[i] == '-'){
				while (isspace(buf[i]) == 0 && buf[i] != '('&& buf[i] != ')'){
					cur_function_tmp.AppendChar(buf[i]);
					curparameter.AppendChar(buf[i]);
					i++;
				}
			}
			else if (buf[i] == '('){
				int tmp_index = i;
				only_if_minus_parameter = false;

				while (!only_if_minus_parameter){

					i = program_convert(tmp_index);



					int count_p = 0;
					int pointer_i = tmp_index;

					//공백 넘기
					pointer_i = skip_whitespace(pointer_i);
					if (pointer_i == -1){
						check_error = true;
						return pointer_i;
					}

					CStringA tmp_cur_parametr;
					while (1){
						tmp_cur_parametr.AppendChar(buf[pointer_i]);
						if (buf[pointer_i] == '('){
							count_p++;

						}
						else if (buf[pointer_i] == ')'){
							count_p--;
						}
						pointer_i++;
						if (count_p == 0){
							break;
						}
						if (pointer_i >= size){
							check_error = true;
							return i;
						}
					}
					tmpbuf_CStringA->ReleaseBuffer();
					temp_buffer.Trim(" ");
					tmpbuf_CStringA->Replace(tmp_cur_parametr, temp_buffer);
					size = tmpbuf_CStringA->GetLength();
					buf = tmpbuf_CStringA->GetBuffer();
					i = tmp_index + temp_buffer.GetLength() - 1;
				}
				curparameter.Format("%s", temp_buffer);
				i++;
			}
			else{
				check_error = true;
				return i;
			}

			cur_parameter_list.push_back(curparameter);
			cur_function_tmp.AppendChar(' ');
			//공백 넘기
			i = skip_whitespace(i);
			if (i == -1){
				check_error = true;
				return i;
			}

		}

		if (buf[i] != ')'){
			check_error = true;
			return i;
		}
		i++;
		//IF
		if (temp_function_name.Compare("IF") == 0){
			temp_buffer.Format("(IF");
			for (std::list<CStringA>::iterator it = cur_parameter_list.begin(); it != cur_parameter_list.end(); ++it){
				temp_buffer.AppendFormat(" %s", *it);
			}
			temp_buffer.AppendFormat(")");
			only_if_minus_parameter = true;
		}
		//MINUS
		else if (temp_function_name.Compare("MINUS") == 0){
			temp_buffer.Format("(MINUS");
			for (std::list<CStringA>::iterator it = cur_parameter_list.begin(); it != cur_parameter_list.end(); ++it){
				temp_buffer.AppendFormat(" %s", *it);
			}
			temp_buffer.AppendFormat(")");
			only_if_minus_parameter = true;
		}
		//현재 정의된 list에서
		else{
			temp_buffer.Format("%s", cur_fun_point->changeIFMINUS(cur_parameter_list));
			if (temp_buffer.Compare("error") == 0){
				check_error = true;
				return i;
			}
			only_if_minus = false;
			only_if_minus_parameter = false;
		}

		return i;
	}
	else{
		if (isalpha(buf[i]) != 0 || isdigit(buf[i]) != 0 || buf[i] =='-'){
			only_if_minus_parameter = true;
			temp_buffer.Format("%s",buf);
		}
		return 0;
	}
}

//buf 해당 index부터 화이트스페이스가 아닌 곳의 index를 return
int ProcessDescription::skip_whitespace(int index){
	int i = index;
	if (i >= size){
		return -1;
	}
	while (isspace(buf[i]) != 0){
		i++;
		//끝날 때까지 화이트 스페이스만 있으면 -1 리턴
		if (i == size){
			return -1;
		}
	}
	return i;
}
// COND를 -> if then else 문장으로 바꿔줌 -  COND 일시 true  아니면 false리턴
bool ProcessDescription::cond_process(std::list<CStringA>::iterator &it){

	CStringA cond_term;
	int i = 0;
	//공백 넘기
	i = skip_whitespace(i);
	if (i == -1){
		return false;
	}

	if (buf[i] == '('){
		CStringA temp_function_name;
		bool is_else = false;
		int count_ite = 0;
		cond_term.Append("( ");
		i++;
		i = skip_whitespace(i);

		//함수 이름 축출
		if (isalpha(buf[i]) != 0){
			while (isspace(buf[i]) == 0 && buf[i] != '('&& i < size){
				cond_term.AppendChar(buf[i]);
				temp_function_name.AppendChar(buf[i]);
				i++;
			}

		}
		else{
			return false;
		}
		cond_term.Append("\r\n");

		//COND
		if (temp_function_name.Compare("COND") == 0){
			CStringA if_then_else_term;

			while (1){
				it++;
				size = it->GetLength();
				tmpbuf_CStringA = &(*it);

				cond_term.Append(*it);
				cond_term.Append("\r\n");

				buf = it->GetBuffer();

				//공백 넘기
				i = skip_whitespace(0);
				if (i == -1){
					return false;
				}

				if (buf[i] != '('&&buf[i] != ')'){
					return false;
				}
				//cond 종료
				else if (buf[i] == ')'){


					if (is_else){
						if_then_else_term.Append(" )");
					}
					else{
						if_then_else_term.Append(" undefined )");
					}

					for (int j = 0; j < count_ite - 1; j++){
						if_then_else_term.AppendChar(')');
					}

					break;
				}
				else{
					i++;
				}

				//공백 넘기
				i = skip_whitespace(i);
				if (i == -1){
					return false;
				}

				//predicate
				if (buf[i] == '('){
					CStringA tmp_cur_parametr;
					int count_p = 0;
					while (1){
						tmp_cur_parametr.AppendChar(buf[i]);
						if (buf[i] == '('){
							count_p++;

						}
						else if (buf[i] == ')'){
							count_p--;
						}
						i++;
						if (count_p == 0){
							break;
						}
						if (i >= size){
							return false;
						}
					}

					if (!is_else){
						if_then_else_term.Append("(IF/THEN/ELSE ");
						count_ite++;
					}
					if_then_else_term.AppendFormat(" %s ", tmp_cur_parametr);
				}
				else{
					//변수나 상수
					if (isspace(buf[i]) == 0){
						CStringA tmp_term;
						while (isspace(buf[i]) == 0 && buf[i] != '('){
							tmp_term.AppendChar(buf[i]);
							i++;
						}
						//ELSE 일때 체크
						if (tmp_term.Compare("ELSE") == 0){
							is_else = true;
						}
						else{
							if (!is_else){
								if_then_else_term.Append("(IF/THEN/ELSE ");
								count_ite++;
							}
							if_then_else_term.AppendFormat(" %s ", tmp_term);
						}
					}
					else{
						return false;
					}
				}

				//term
				//공백 넘기
				i = skip_whitespace(i);
				if (i == -1){
					return false;
				}

				if (buf[i] == '('){
					CStringA tmp_cur_parametr;
					int count_p = 0;
					while (1){
						tmp_cur_parametr.AppendChar(buf[i]);
						if (buf[i] == '('){
							count_p++;

						}
						else if (buf[i] == ')'){
							count_p--;
						}
						i++;
						if (count_p == 0){
							break;
						}
						if (i >= size){
							return false;
						}
					}
					if_then_else_term.AppendFormat(" %s ", tmp_cur_parametr);
				}
				else{
					//변수나 상수
					if (isspace(buf[i]) == 0){
						CStringA tmp_term;
						while (isspace(buf[i]) == 0 && buf[i] != ')'){
							tmp_term.AppendChar(buf[i]);
							i++;
						}
						if_then_else_term.AppendFormat(" %s ", tmp_term);
					}
					else{
						return false;
					}
				}
				//공백 넘기
				i = skip_whitespace(i);
				if (i == -1){
					return false;
				}
				//마지막 )
				if (buf[i] != ')'){
					return false;
				}

			}

			//메세지에 COND, IF/THEN/ELSE로 변형된 문장 첨부
			message.Append("\r\n");
			message.Append(cond_term);
			message.Append("->");
			message.Append(if_then_else_term);

			//현재 가르키고 있는것 IF/THEN/ELSE문으로 변경
			(*it).ReleaseBuffer();
			(*it).Format("%s", if_then_else_term);

			size = it->GetLength();
			tmpbuf_CStringA = &(*it);
			buf = it->GetBuffer();

			return true;
		}
		else{
			return false;

		}



	}
	else{
		return false;
	}

}