#include "stdafx.h"


#include "ProcessDescription.h"


//������
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
	//���κ��� �ϳ��� ���� ó��
	for (std::list<CStringA>::iterator it = program_list.begin(); it != program_list.end(); ++it){
		//���� �ٲ� ������ if minus�θ� �������ֳ� üũ
		only_if_minus_parameter = false;
		check_error = false;
		//�ʱ� ������ if minus�θ� ����
		only_if_minus = true;

		size = it->GetLength();
		tmpbuf_CStringA = &(*it);
		buf = it->GetBuffer();

		//cond Ȯ��
		if (!cond_process(it)){
			message.Append("\r\n");
			message.Append(*it);
			temp_buffer.Format("");
		}


		//����ȿ� if minus�� ���� �� ���� �ݺ�
		while (!only_if_minus_parameter){
			program_convert(0);
			if (check_error){
				break;
			}
			tmpbuf_CStringA->Format("%s", temp_buffer);
			buf = tmpbuf_CStringA->GetBuffer();
			size = tmpbuf_CStringA->GetLength();

		}
		//���� ��ȯ �� ���
		if (!only_if_minus&& !check_error){
			message.Append("\r\n-> ");
			message.Append(temp_buffer);
			after_program_list.push_back(temp_buffer);
		}
		// if minus�θ� �̷���� ����
		else if (only_if_minus&& !check_error){
			after_program_list.push_back(temp_buffer);
		}
		//����
		else if (check_error){
			message.Append("\r\n-> PROGRAM ERROR");
			after_program_list.push_back(*tmpbuf_CStringA);
		}
		message.Append("\r\n");
	}

}


//��ȯ�� ���α׷� ����Ʈ
std::list<CStringA> ProcessDescription::get_program_list(){
	return after_program_list;
}
//�޼���
CStringA ProcessDescription::get_message(){
	return message;
}


//if Ȥ�� minus�� ��ȯ ��Ű�� �Լ�
int ProcessDescription::program_convert(int i){
	int start_i = i;
	std::list<CStringA> cur_parameter_list;
	CStringA cur_function_tmp;
	//���� �ѱ�
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

		//�Լ� �̸� ����
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

		//�Լ� Ȯ��

		//IF
		if (temp_function_name.Compare("IF") == 0){
			cur_parameter_num = 2;
		}
		//MINUS
		else if (temp_function_name.Compare("MINUS") == 0){
			cur_parameter_num = 2;
		}
		//���� ���ǵ� list����
		else{
			bool find = false;
			for (std::list<DEFUN>::iterator it = defun_list.begin(); it != defun_list.end(); ++it){
				if (temp_function_name.Compare(it->getFunctionName()) == 0){
					cur_parameter_num = it->getNumofParameter();
					cur_fun_point = &(*it);
					find = true;
				}
			}
			//��ã�����
			if (!find){
				check_error = true;
				return i;
			}
		}

		//���� �ѱ�
		i = skip_whitespace(i);
		if (i == -1){
			check_error = true;
			return i;
		}

		//�Ķ���� üũ

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

					//���� �ѱ�
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
			//���� �ѱ�
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
		//���� ���ǵ� list����
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

//buf �ش� index���� ȭ��Ʈ�����̽��� �ƴ� ���� index�� return
int ProcessDescription::skip_whitespace(int index){
	int i = index;
	if (i >= size){
		return -1;
	}
	while (isspace(buf[i]) != 0){
		i++;
		//���� ������ ȭ��Ʈ �����̽��� ������ -1 ����
		if (i == size){
			return -1;
		}
	}
	return i;
}
// COND�� -> if then else �������� �ٲ��� -  COND �Ͻ� true  �ƴϸ� false����
bool ProcessDescription::cond_process(std::list<CStringA>::iterator &it){

	CStringA cond_term;
	int i = 0;
	//���� �ѱ�
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

		//�Լ� �̸� ����
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

				//���� �ѱ�
				i = skip_whitespace(0);
				if (i == -1){
					return false;
				}

				if (buf[i] != '('&&buf[i] != ')'){
					return false;
				}
				//cond ����
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

				//���� �ѱ�
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
					//������ ���
					if (isspace(buf[i]) == 0){
						CStringA tmp_term;
						while (isspace(buf[i]) == 0 && buf[i] != '('){
							tmp_term.AppendChar(buf[i]);
							i++;
						}
						//ELSE �϶� üũ
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
				//���� �ѱ�
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
					//������ ���
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
				//���� �ѱ�
				i = skip_whitespace(i);
				if (i == -1){
					return false;
				}
				//������ )
				if (buf[i] != ')'){
					return false;
				}

			}

			//�޼����� COND, IF/THEN/ELSE�� ������ ���� ÷��
			message.Append("\r\n");
			message.Append(cond_term);
			message.Append("->");
			message.Append(if_then_else_term);

			//���� ����Ű�� �ִ°� IF/THEN/ELSE������ ����
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