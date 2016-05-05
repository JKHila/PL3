#include "stdafx.h"
#include "DEFUN.h"

//static list define
std::list<DEFUN> DEFUN::defun_list;

//DEFUN�Լ��� ���� ������ ����
DEFUN::DEFUN(CStringA temp)
{
	temp_buf.Append(temp);
	buf_pointer_len = temp_buf.GetLength();
	buf_pointer = temp_buf.GetBuffer();
	is_recurcive = false;
	//setting() ȣ�� �Լ� ��� ����
	setting();

}

//�Լ� ����ϴ� ����
void DEFUN::setting(){
	int i = 0;
	right_function = 1;

	//���� ���µ� ����
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}

	//ó�� ���� ��ȣ �ִ��� Ȯ��
	if (buf_pointer[i] != '('){
		right_function = 5;
		return;
	}
	else{
		i++;
	}

	//DEFUN���� �����ϳ� üũ
	if (strncmp(buf_pointer + i, "DEFUN", 5) != 0){
		right_function = 5;
		return;
	}
	else{
		i += 5;
	}

	//���� ���µ� ����
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}

	//�Լ��� ���
	if (isalpha(buf_pointer[i]) != 0){
		function_name.AppendChar(buf_pointer[i]);
		i++;
		while (isspace(buf_pointer[i]) == 0 && buf_pointer[i] != '('){
			function_name.AppendChar(buf_pointer[i]);
			i++;
		}
		//�ߺ� �Լ� �˻�
		{
			int defun_list_size = defun_list.size();
			for (std::list<DEFUN>::iterator it = defun_list.begin(); it != defun_list.end(); ++it){
				if (function_name.Compare(it->getFunctionName()) == 0){
					right_function = 2;
					return;
				}
			}
		}
		//full_line�� �߰�
		full_line.Append(function_name);
		full_line.AppendChar(' ');
	}
	else{
		right_function = 5;
		return;
	}


	//���� ���µ� ����
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}

	//���� ��ȣ �ִ��� Ȯ��
	if (buf_pointer[i] != '('){
		right_function = 5;
		return;
	}
	else{
		i++;
	}

	//���� ���µ� ����
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}

	//full_line�� �߰�
	full_line.Append("( ");

	//parameter ���	
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

			//�ߺ� parameter �˻�
			{
				int parameter_list_size = parameter_list.size();
				for (std::list<CStringA>::iterator it = parameter_list.begin(); it != parameter_list.end(); ++it){
					if (tmpstring.Compare(*it) == 0){
						right_function = 3;
						return;
					}
				}
			}
			//�Ķ���� ����Ʈ�� �߰�
			parameter_list.push_back(tmpstring);

			//full_line�� �߰�
			full_line.Append(tmpstring);
			full_line.AppendChar(' ');

			//���� ���µ� ����
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

	//���� ���µ� ����
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}

	//�ݴ� ��ȣ �ִ��� Ȯ��
	if (buf_pointer[i] != ')'){
		right_function = 5;
		return;
	}
	else{
		i++;
	}

	//full_line�� �߰�
	full_line.Append(")");

	//���� ���µ� ����
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}


	//////////////////

	//�Լ� ���Ǻ� ����üũ �� ���
	i = function_start(i);
	if (right_function != 1){
		return;
	}


	//////////////////

	//���� ���µ� ����
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return;
	}

	//���� ��ȣ �ִ��� Ȯ��
	if (buf_pointer[i] != ')'){
		right_function = 5;
		return;
	}
	else{
		i++;
	}

	//���� ���µ� ����
	i = skip_whitespace(i);
	//DEFUN ������ ��ȣ�� -1�� �ƴ϶�� DEFUN�Լ��ڿ� ���� �پ�ٴ� ��
	if (i != -1){
		right_function = 5;
		return;
	}

	if (right_function == 1)
		defun_list.push_back(*this);

}


//temp_buf �ش� index���� ȭ��Ʈ�����̽��� �ƴ� ���� index�� return
int DEFUN::skip_whitespace(int index){
	int i = index;
	if (i >= buf_pointer_len){
		return -1;
	}
	while (isspace(buf_pointer[i]) != 0){
		i++;
		//���� ������ ȭ��Ʈ �����̽��� ������ -1 ����
		if (i == buf_pointer_len){
			return -1;
		}
	}
	return i;
}

//�Լ� Ȯ��
int DEFUN::function_start(int i){
	CStringA temp_function_name;
	int cur_parameter_num;
	//���� ��ȣ �ִ��� Ȯ��
	if (buf_pointer[i] != '('){
		right_function = 4;
		return i;
	}
	else{
		i++;
	}

	//full_line�� �߰�
	full_line.Append(" (");
	//�Լ� ���Ǻο� �߰�
	process_description.Append(" (");

	//���� ���µ� ����
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return i;
	}

	//���� �Լ� �̸� ã��	
	if (isalpha(buf_pointer[i]) != 0){
		while (isspace(buf_pointer[i]) == 0 && buf_pointer[i] != ')'&& buf_pointer[i] != '('){
			temp_function_name.AppendChar(buf_pointer[i]);
			i++;
		}
		full_line.Append(temp_function_name);
		full_line.Append(" ");
		//�Լ� ���Ǻο� �߰�
		process_description.Append(temp_function_name);
		process_description.Append(" ");
	}
	else{
		right_function = 4;
		return i;
	}

	//���� ���µ� ����
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return i;
	}

	//�Լ�ã�� parameter �� Ȯ��

	//����� ȣ��
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
	//���� ���ǵ� list����
	else{
		bool find = false;
		for (std::list<DEFUN>::iterator it = defun_list.begin(); it != defun_list.end(); ++it){
			if (temp_function_name.Compare(it->getFunctionName()) == 0){
				cur_parameter_num = it->getNumofParameter();
				find = true;
			}
		}
		//��ã�����
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

		//���� ������ �߰��� �����ٸ�
		if (i == -1){
			right_function = 5;
			return i;
		}

		//�߰����� �߸���
		if (right_function != 1){
			return i;
		}
	}

	//���� ��ȣ �ִ��� Ȯ��
	if (buf_pointer[i] != ')'){
		right_function = 5;
		return i;
	}
	else{
		i++;
	}


	//full_line�� �߰�
	full_line.Append(")");
	//�Լ� ���Ǻο� �߰�
	process_description.Append(")");

	//���� ���µ� ����
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return i;
	}

	return i;

}
//�ùٸ� �Ķ�������� Ȯ��
int DEFUN::parameter_check(int i){
	//parameter ���	
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

		//�ߺ� parameter �˻�
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
		//full_line�� �߰�
		full_line.Append(tmpstring);
		full_line.AppendChar(' ');
		//�Լ� ���Ǻο� �߰�
		process_description.AppendChar('@');
		process_description.Append(tmpstring);
		process_description.AppendChar(' ');

	}
	else{
		right_function = 4;
		return i;
	}

	//���� ���µ� ����
	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return i;
	}

	return i;

}
//��� ���� Ȯ��
int DEFUN::constant_check(int i){

	if (buf_pointer[i] == '-'){
		//full_line�� �߰�
		full_line.AppendChar(buf_pointer[i]);
		//�Լ� ���Ǻο� �߰�
		process_description.AppendChar(buf_pointer[i]);
		//���� ���µ� ����
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
		//full_line�� �߰�
		full_line.AppendChar(buf_pointer[i]);
		//�Լ� ���Ǻο� �߰�
		process_description.AppendChar(buf_pointer[i]);
		i++;
	}

	i = skip_whitespace(i);
	if (i == -1){
		right_function = 5;
		return i;
	}

	//full_line�� �߰�
	full_line.AppendChar(' ');
	//�Լ� ���Ǻο� �߰�
	process_description.AppendChar(' ');
	return i;
}



//�Լ� �̸�
CStringA DEFUN::getFunctionName(){
	return function_name;
}
//�Լ� ����
CStringA DEFUN::getProcessDescription(){
	return process_description;
}
//<function name>(<parameter 1>...<parameter n>) <process description>
//���·� return
CStringA DEFUN::getFullLine(){
	return full_line;
}
//formal parameter �̸��� return
CStringA DEFUN::getParameter(int index){
	int i = 0;
	for (std::list<CStringA>::iterator it = parameter_list.begin(); it != parameter_list.end(); ++it){
		if (index == i)
			return *it;
	}
	CStringA error("@WRONG INDEX");
	return error;
}

//�Ķ���� �� ����
int DEFUN::getNumofParameter(){
	return parameter_list.size();
}

//���� �Է��� �Լ��� �ùٸ� �Լ����� �ƴ��� return
//(1 �ùٸ� �Լ�, 2 �ߺ� �Լ�, 3 parameter ����, 4 �Լ� ���Ǻ� ����)
int DEFUN::IsFunctionOk(){
	return right_function;
}
//�Ķ���� ����Ʈ�� �Ѱ��ָ� process_description�� �°� �ٲ��� �ִ� ������ �ٲ㼭 �Ѱ���
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

	//��� �϶� ù��° ������ IF/THEN/ELSE�� condition�� ���ؼ� ������
	//���϶� ���� ���� 0���� ġȯ�Ѵ�.
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
				//���ǽ��� �����϶� ��, if/then/else���� 2��° �Ķ���Ͱ� ������ ���Եȴ�.
				if (condition_val == 0){
					int count_p = 0;
					//���ǹ� �ǳʱ�
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

					//�߰� �� ��ġ ���
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

					//�߰� �� 0 ���� �ٲٱ�
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
				//���ǽ��� ���϶� �ڸ� ����
				else if (condition_val == 1){
					int count_p = 0;
					//���ǹ� �ǳʱ�
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

					//�߰���  �ǳʱ�
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

					//������ �� ��ġ ���
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

					//�������� 0 ���� �ٲٱ�
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
				//���ǽ��� �����϶� ��, if���� 2��° �Ķ���Ͱ� ������ ���Եȴ�.
				if (condition_val == 0){
					int count_p = 0;
					//���ǹ� �ǳʱ�
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

					//�� ��ġ ���
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

					//�߰� �� 0 ���� �ٲٱ�
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

//�־��� ���� ���� �̸� �Ǵ�����
// condetion�� 0, ����,���� 0 ������ ���� -1 ����, ���� ����� ���� 1
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
//minus ���
int DEFUN::evalu_condition_minus(char *sentence, int &i){
	int first_oprand = 0;
	int second_oprand = 0;
	//���� ������ �ֳ� Ȯ��
	if (meet_var){
		return -1;
	}
	//���� �ٱ�
	while (isspace(sentence[i]) != 0){
		i++;
	}

	//����
	if (isdigit(sentence[i]) || sentence[i] == '-'){
		first_oprand = atoi(sentence + i);
		while (isdigit(sentence[i]) || sentence[i] == '-'){
			i++;
		}
	}
	//IF MINUS�� �ش� ���ϴ� �Լ��� �ѱ��
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
	//����ٱ�
	while (isspace(sentence[i]) != 0){
		i++;
	}
	//���� ����
	if (isdigit(sentence[i]) || sentence[i] == '-'){
		second_oprand = atoi(sentence + i);
		while (isdigit(sentence[i]) || sentence[i] == '-'){
			i++;
		}
	}
	//IF �� MINUS ������ �ش� ���ڷ� �ٱ�
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
	//�ݴ� ��ȣ �ѱ�
	i++;

	return first_oprand - second_oprand;
}
//if ���
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
	//if ���ǿ� ���� �� �Ǵ� ����
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
	//�ݴ� ��ȣ �ѱ�
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