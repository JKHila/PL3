#include "PrefixToPostfix.h"
#include "stdafx.h"

//생성자
PrefixToPostfix::PrefixToPostfix(CString Prefix_tmp){
	Prefix = new CStringA(Prefix_tmp);

	//에러 메세지 초기화
	errnum_string[0] = "";
	errnum_string[1] = "피연산자 자리에 정수가 아닌 실수가 들어갔습니다.\n";
	errnum_string[2] = "괄호가 잘 못 사용되었습니다.\n";
	errnum_string[3] = "기호 사용이 잘못 되었습니다\n";
	errnum_string[4] = "정의되지 않은 연산자 입니다.\n";
	errnum_string[5] = "피연산자 자리에 -가 연속으로 입력되었습니다.\n";
	errnum_string[6] = "숫자뒤에 문자 또는 기호가 공백 없이 들어 갔습니다.\n";

	//연산자 초기화
	oper[0] = "IF";
	oper[1] = "MINUS";

	if (Prefix->GetLength() == 0){
		Prefix->AppendChar(' ');
	}

	//들어온 문자열 끝에 엔터 없으면 강제로 넣어준다.
	if (Prefix->GetAt(Prefix->GetLength() - 1) != '\n'){
		Prefix->AppendChar('\n');
	}

	//연산자 주변에 공백 만들기
	Prefix->Replace("IF", " IF ");
	Prefix->Replace("MINUS", " MINUS ");

	//문자열 길이 얻기
	len = Prefix->GetLength();

	//CString class를 char배열 처럼 사용
	//세이브 파일 크기 만큼 버퍼 염	
	tmpbuf = Prefix->GetBuffer(len);

	make_Postfix();
}

//소멸자
PrefixToPostfix::~PrefixToPostfix(){
	delete Prefix;
}

//후위표기식으로 만드는 함수
void PrefixToPostfix::make_Postfix(){
	int i = 0;
	//임시 에러번호 (가장 첫 에러를 출력하기 위한)
	int errnum_tmp;

	//prefix가 입력된 CStringA 끝까지 for문
	for (i = 0; i < len; i++){
		errnum = 0;
		errnum_tmp = 0;
		//라인의 시작 index
		linestart = i;
		//임시 tmp_str 초기화
		tmp_str.Format("");

		//괄호 수 확인
		int tmpj = i;
		int count_open_parenthness = 0;
		int count_close_parenthness = 0;
		for (; tmpj < len; tmpj++)
		{
			if (tmpbuf[tmpj] == '(')
				count_open_parenthness++;
			else if (tmpbuf[tmpj] == ')')
				count_close_parenthness++;
			else if (tmpbuf[tmpj] == '\n')
				break;
		}
		//괄호수 다를경우
		if (count_open_parenthness != count_close_parenthness)
		{
			i = tmpj;
			errnum = 2;
		}
		//괄호 수 같을 경우
		else{
			//공백 넘기기
			if (isspace(tmpbuf[i])){
				continue;
			}
			//괄호 시작
			else if (tmpbuf[i] == '('){
				i = Is_open_parentheses(i);
				errnum_tmp = errnum;
				i++;
				i = check_err_after_program(i);
			}
			//변수 인지 확인
			else if (isalpha(tmpbuf[i])){
				i = Is_val(i);
				errnum_tmp = errnum;
				i++;
				i = check_err_after_program(i);
			}
			//상수 인지 확인
			else if (isdigit(tmpbuf[i])){
				i = Is_con(i);
				errnum_tmp = errnum;
				i++;
				i = check_err_after_program(i);
			}
			//마이너스 상수 인지 확인
			else if (tmpbuf[i] == '-'){
				i = Is_minus_con(i);
				errnum_tmp = errnum;
				i++;
				i = check_err_after_program(i);
			}
			//알파벳 - 제외한 기호 들어 왔을 때
			else{
				errnum = 3;
				errnum_tmp = errnum;
				i = check_err_after_program(i);
			}
		}

		//라인의 끝 index
		lineend = i;
		//첫 에러를 메세지 해줌
		if (errnum_tmp)
			errnum = errnum_tmp;

		//에러시 에러 라인을 메세지에 넣어줌
		if (errnum)
		{
			int j;
			for (j = linestart; j < lineend; j++){
				message.AppendChar(tmpbuf[j]);
			}
			if (tmpbuf[j - 1] != '\n')
				message.Append("\r\n");
		}
		else{
			listofPostfix.push_back(tmp_str);
			message.Append(tmp_str);
		}

		//에러 출력, 없으면 아무것도 출력되지 않음
		printerror();

	}

}

//에러메세지 출력, 출력후 errno=0로 만듬
void PrefixToPostfix::printerror(){
	message.Append(errnum_string[errnum]);
	message.Append("\r\n");
	errnum = 0;
}

//하나의 프로그램이 끝난후 부터 엔터혹은 입력받은 Prefix의 끝일 경우 index를 리턴한다.
//만약 프로그램과 엔터 사이에 공백을 제외한 다른 문자가 있을경우 err 체크한다.
int PrefixToPostfix::check_err_after_program(int index){
	//개행문자가 나올때 까지 for문
	if (index < len)
	while (tmpbuf[index] != '\n' && (index != (len - 1))){
		//만약 그 뒤에 이상한것이 나오면 errnum=2 를 표시
		if (!isspace(tmpbuf[index])){
			errnum = 2;
		}
		++index;
	}
	return index;
}


//읽은 마지막 index 리턴 err시 해당 errnum 
//열린 괄호인가?
int PrefixToPostfix::Is_open_parentheses(int index){
	int i;
	//이 식의 op
	int this_op_num;
	//seq 0: <fun> 
	//seq 1: <term>
	//seq 2: <term>
	//seq 3: )
	tmp_str.AppendChar(tmpbuf[index]);


	//seq 1: <fun>
	for (i = index + 1; i < len; i++){

		//공백 넘기기
		if (tmpbuf[i] == ' ' || tmpbuf[i] == '\t'){
			continue;
		}
		else if (isalpha(tmpbuf[i])){
			i = Is_op(i);
			if (errnum){
				return i;
			}
			i++;
			//이 식의 op num 기록
			this_op_num = current_opnum;
			break;
		}
		//잘못된 위치의 괄호
		else if (tmpbuf[i] == ')'){
			errnum = 2;
			return i;
		}
		//잘못된 기호 들어옴
		else{
			if (isdigit(tmpbuf[i]))
				errnum = 4;
			else
				errnum = 3;
			return i;
		}
	}
	//term 안나오고 문장 끝
	if (i == len){
		errnum = 2;
		return i - 1;
	}
	//tmp_str.AppendChar(' ');

	//seq 1: <term> 
	for (; i < len; i++){

		//공백 넘기기
		if (tmpbuf[i] == ' ' || tmpbuf[i] == '\t'){
			continue;
		}
		//재귀적으로 호출
		else if (tmpbuf[i] == '('){
			i = Is_open_parentheses(i);
			if (errnum){
				return i;
			}
			i++;
			break;
		}
		//변수 인지 확인
		else if (isalpha(tmpbuf[i])){
			i = Is_val(i);
			if (errnum){
				return i;
			}
			i++;
			break;
		}
		//상수 인지 확인
		else if (isdigit(tmpbuf[i])){
			i = Is_con(i);
			if (errnum){
				return i;
			}
			i++;
			break;
		}
		//마이너스 상수 인지 확인
		else if (tmpbuf[i] == '-'){
			i = Is_minus_con(i);
			if (errnum){
				return i;
			}
			i++;
			break;
		}
		//잘못된 위치의 괄호
		else if (tmpbuf[i] == ')'){
			errnum = 2;
			return i;
		}
		//잘못된 기호 들어옴
		else{
			errnum = 3;
			return i;
		}
	}
	//term 안나오고 문장 끝
	if (i == len){
		errnum = 2;
		return i - 1;
	}
	tmp_str.AppendChar(' ');

	//seq 2: <term>
	for (; i < len; i++){

		//공백 넘기기
		if (tmpbuf[i] == ' ' || tmpbuf[i] == '\t'){
			continue;
		}
		//재귀적으로 호출
		else if (tmpbuf[i] == '('){
			i = Is_open_parentheses(i);
			if (errnum){
				return i;
			}
			i++;
			break;
		}
		//변수 인지 확인
		else if (isalpha(tmpbuf[i])){
			i = Is_val(i);
			if (errnum){
				return i;
			}
			i++;
			break;
		}
		//상수 인지 확인
		else if (isdigit(tmpbuf[i])){
			i = Is_con(i);
			if (errnum){
				return i;
			}
			i++;
			break;
		}
		//마이너스 상수 인지 확인
		else if (tmpbuf[i] == '-'){
			i = Is_minus_con(i);
			if (errnum){
				return i;
			}
			i++;
			break;
		}
		//잘못된 위치의 괄호
		else if (tmpbuf[i] == ')'){
			errnum = 2;
			return i;
		}
		//잘못된 기호 들어옴
		else{
			errnum = 3;
			return i;
		}
	}

	//) 없이 문장 끝
	if (i == len){
		errnum = 2;
		return i - 1;
	}

	//seq 3: )
	for (; i < len; i++){

		//공백 넘기기
		if (tmpbuf[i] == ' ' || tmpbuf[i] == '\t'){
			continue;
		}
		else if (tmpbuf[i] == ')'){
			tmp_str.AppendChar(' ');
			tmp_str.Append(oper[this_op_num]);
			tmp_str.AppendChar(tmpbuf[i]);
			return i;
		}
		//괄호 잘못됨
		else{
			errnum = 2;
			return i;
		}
	}

	errnum = 2;
	return index;
}

//index에 문자 들어와야함
//읽은 마지막 index 리턴 err시 해당 errnum 
//변수인가?
int PrefixToPostfix::Is_val(int index){
	//공백 나올때 까지 for문 돔
	//또는 문장 끝 날때 까지 for문
	for (; index < len; index++){
		//공백 나오거나 닫는 괄호나오면 for문 중지
		if (tmpbuf[index] == ' ' || tmpbuf[index] == '\t' || tmpbuf[index] == '\r' || tmpbuf[index] == '\n' || tmpbuf[index] == ')' || tmpbuf[index] == '(') {
			break;
		}
		//문자나 숫자 이외의것이 나오면 err
		else if (!isalpha(tmpbuf[index]) && !isdigit(tmpbuf[index])){
			errnum = 3;
			return index;
		}
		tmp_str.AppendChar(tmpbuf[index]);
	}

	return index - 1;


}

//index에 숫자 들어와야함
//읽은 마지막 index 리턴 err시 해당 errnum 
//상수인가?
int PrefixToPostfix::Is_con(int index){
	//공백 나올때 까지 for문 돔
	//또는 문장 끝 날때 까지 for문
	for (; index < len; index++){
		//공백 나오거나 닫는 괄호나오면 for문 중지
		if (tmpbuf[index] == ' ' || tmpbuf[index] == '\t' || tmpbuf[index] == '\r' || tmpbuf[index] == '\n' || tmpbuf[index] == ')' || tmpbuf[index] == '('){
			break;
		}
		//. 나오면 실수로 판단하고 err
		else if (tmpbuf[index] == '.'){
			errnum = 1;
			return index;
		}
		//숫자 아닌것 나오면 err
		else if (!isdigit(tmpbuf[index])){
			errnum = 6;
			return index;
		}
		tmp_str.AppendChar(tmpbuf[index]);
	}
	return index - 1;
}

//index에 문자 들어와야함
//읽은 마지막 index 리턴 err시 해당 errnum 
//연산자 인가?
int PrefixToPostfix::Is_op(int index){
	//연산자 맞으면 true 아니면 false
	bool correct;
	int i, j;

	//연산자 종류 만큼 for문 돔
	for (i = 0; i < (sizeof(oper) / 4); i++){
		//연산자의 길이
		int OP_len = strlen(oper[i]);

		//문장 끝에 연산자가 잘못 들어올경우 대비한 if문
		//연산자 이후 최소 3자가 더 있어야 식이 성립된다.
		if (index + OP_len + 2 < len){
			correct = true;
			for (j = 0; j < OP_len; j++){
				if (tmpbuf[index + j] != oper[i][j]){
					correct = false;
					break;
				}
			}
			if (correct){
				//연산자 뒤에 공백 혹은 여는 괄호 있어야한다.
				if (tmpbuf[index + j] == ' ' || tmpbuf[index + j] == '\t' || tmpbuf[index + j] == '('){
					current_opnum = i;
					return index + j - 1;
				}
				//연산자 뒤에 공백이 아닌 이상한것이 오면
				else{

					errnum = 4;
					if (tmpbuf[index + j] == ')')
						errnum = 2;
					return index + j - 1;
				}
			}
		}

	}
	//일치하는 연산자 못찾음
	errnum = 4;
	return index;

}

//index에 -들어와야함
//읽은 마지막 index 리턴 err시 해당 errnum 
//-로 시작되는 상수인가?
int PrefixToPostfix::Is_minus_con(int index){
	//-가 문장 끝 오는 것 방지하는 if문
	tmp_str.AppendChar(tmpbuf[index]);
	if (index + 1 < len){
		if (tmpbuf[index + 1] == '-'){
			errnum = 5;
			return index + 1;
		}
		else if (isdigit(tmpbuf[index + 1])){
			return Is_con(index + 1);
		}
		else{
			//-뒤에 다른 문자 나올때
			errnum = 3;
			return index;
		}
	}
	//-뒤에 아무것도 안올 때
	errnum = 1;
	return index;
}


//올바른 식들 후위식으로 넘겨줌
std::list<CStringA> PrefixToPostfix::getPostfix(){
	return listofPostfix;
}

//오류 메세지 + 올바른 식의 후위표기식
CString PrefixToPostfix::getMessage_Postfix(){
	CString tmptmp(message);
	return tmptmp;
}