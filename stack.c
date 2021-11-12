void init_stack(StackType* s)
{
	s->top = -1; //s의 top의 값을 –1로 함으로서 스택 s를 초기화한다.
}

int is_empty(StackType* s)
{
	return (s->top == -1); //s의 top과 –1이 같다면 비어있으므로 1을 반환하고 있으면 0을 반환한다.
}

int is_full(StackType* s)
{
	return (s->top == (MAX_STACK_SIZE – 1)); //꽉 차있으면 1을 반환하고 아니면 0을 반환한다.
}
void push(StackType* s, element item)
{
	if (is_full(s)) { //꽉 차있다면 에러 메시지 출력
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	else s->data[++(s->top)] = item; //s의 top 데이터에 item을 대입
}

element pop(StackType* s)
{
	if (is_empty(s)) { //비어있으면 에러 메시지 출력
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[(s->top)--]; //s의 top의 데이터를 반환하고 top--
}

element peek(StackType* s)
{
	if (is_empty(s)) { //비어있으면 에러 메시지 출력
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[s->top]; //s의 top의 데이터를 반환
}
// ===== 스택 코드의 끝 ===== 

// 후위 표기 수식 계산 함수
int eval(char exp[])
{
	int op1, op2, value, i = 0; //op1(앞 숫자), op2(뒤 숫자), value(계산하기 위한 변수)
	int len = strlen(exp); //exp[]의 길이를 len에 대입
	char ch;
	StackType s;

	init_stack(&s); //스택s 초기화
	for (i = 0; i < len; i++) {
		ch = exp[i];
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/') {
			value = ch - '0';	// 입력이 피연산자이면
			push(&s, value); //s에 값을 넣음
		}
		else {	//연산자이면 피연산자를 스택에서 제거
			op2 = pop(&s);
			op1 = pop(&s);
			switch (ch) { //연산을 수행하고 스택에 저장 
			case '+': push(&s, op1 + op2); break;
			case '-': push(&s, op1 - op2); break;
			case '*': push(&s, op1 * op2); break;
			case '/': push(&s, op1 / op2); break;
			}
		}
	}
	return pop(&s); //최종 계산값을 반환
}

// 연산자의 우선순위를 반환한다.
int prec(char op)
{
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1;
}

// 수식 변환함수
element* infix_to_postfix(element exp[])
{
	int i = 0, idx = 0; //i는 for문의 제어변수, idx는 post_arr의 index
	int num = 0; //한자리 수 이상의 입력 오류 체크 할때 필요한 변수
	int sum = 0; //괄호 짝이 안맞는 경우를 확인하는 변수
	int num_ch = 0, num_op = 0; //숫자 개수랑 연산자 개수 비교할 때 필요한 변수
	int len = strlen(exp);
	char ch, op;
	StackType s;	
	element* postfix_arr = (element*)malloc(MAX_STACK_SIZE); //메모리 동적할당
	if (postfix_arr == NULL) {
		fprintf(stderr, "메모리 할당 에러\n");
		exit(1);
	}	

	init_stack(&s);  //스택 초기

	memset(postfix_arr, 0, MAX_STACK_SIZE); //memset으로 동적할당 메모리 초기화

	//0으로 나눠주는 경우 예외처리 error0
	for (i = 0; i < len; i++)
	{
		if (exp[i] == '/')
		{ //나누기 연산인데
			if (exp[i + 1] == '0') //그 다음이 0이면 에러발생
			{
				printf("<<error 발생>>\n");
				printf("infix_to_postfix error0 : divided by 0\n\n");
				err = 1; //err 발생했으므로 1대입
				break;	//오류 발생해서 더 이상 연산이 필요 없으므로 break
			}
		}
		else if (exp[i] == 0) //배열의 끝을 만나면 break
			break;
	}

	//괄호매칭이 안되는 경우 예외처리 error1
	for (i = 0; i < len; i++)
	{
		if (exp[i] == '(') //왼쪽괄호(를 만나면 sum을 1증가
			sum++;
		else if (exp[i] == ')') //오른쪽괄호)를 만나면 sum을 1감소
		{
			sum--;
			if (sum < 0) //sum이 음수가 됐다는 것은 오른쪽괄호를 먼저 만난 것이므로 에러발생
			{
				printf("<<error 발생>>\n");
				printf("infix_to_postfix error1 : 괄호 매칭 불가능\n\n");
				err = 1; //에러 발생했으므로 1대입하고 break
				break;
			}
		}			
	}		
	if (sum > 0) //sum이 양수이면 왼쪽괄호(이 더 많다는 것이므로 에러 발생
	{
		printf("<<error 발생>>\n");
		printf("infix_to_postfix error1 : 괄호 매칭 불가능2\n\n");
		err = 1; //에러 발생했으므로 1대입
	}
	
	
	//예외문자 포함 error2 
	for (i = 0; i < len; i++)
	{				
		if (exp[i] - '0' >= 0 && exp[i] - '0' <= 9) //exp값이 0~9이면 계속 반복
			continue;
		else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/' || exp[i] == ')' || exp[i] == '(') //exp값이 연산기호이거나 괄호이면 계속 반복
			continue;
		else //위의 두 가지 경우가 모두 아닌 경우는 예외문자이므로 에러 발생
		{
			printf("<<error 발생>>\n");
			printf("infix_to_postfix error2 : 예외문자 포함\n\n");
			err = 1;//에러 발생했으므로 1대입 break
			break;
		}
	}


	//한자리 수 이상의 입력 포함 error3	
	for (i = 0; i < len; i++)
	{
		if (exp[i] - '0' >= 0 && exp[i] - '0' <= 9 && exp[i + 1] - '0' >= 0 && exp[i + 1] - '0' <= 9) 
//i번째 값이 0~9사이이고 i+1 값이 0~9사이이면 두 자리수 이상이므로 연속된 숫자가 끝날 때까지 반복
		{
			num++;
			continue;
		}
		else if (num > 0) //num이 양수라는 것은 두 자리수 이상이므로 에러 발생
		{
			printf("<<error 발생>>\n");
			printf("infix_to_postfix error3 : 한자리 수 이상의 입력 포함\n\n");
			err = 1; 
			return postfix_arr; //마지막오류이고 뒤의 연산이 필요 없으므로 반환하고 종료
		}
	}
	
	//추가적으로 위의 오류는 걸리지 않았지만 연산자의 개수가 숫자의 개수보다 많을 경우 오류 error4
	for (i = 0; i < len; i++)
	{
		if (exp[i] - '0' >= 0 && exp[i] - '0' <= 9) //0~9사이인 경우 숫자개수 +1
			num_ch++; 
		else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/')
			num_op++; //연산기호인 경우 연산자개수 +1
	}
	if (num_ch <= num_op) //숫자 개수가 연산자 개수보다 적거나 같다면 연산이 안 되므로 에러 발생
	{
		printf("<<error 발생>>\n");
		printf("infix_to_postfix error4 : 중위표기식 입력이 잘못됨\n\n");
		err = 1;
		return postfix_arr; //마지막 오류이므로 바로 반환하고 종료
	}

	//중위 표기식을 후위 표기식으로 변환
	for (i = 0; i < len; i++)
	{
		ch = exp[i];
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/' && ch != ')' && ch != '(')
			postfix_arr[idx++] = ch; //숫자이면 후위 표기식 배열에 대입

		else //연산자이면
		{
			if (is_empty(&s)) //비어있으면 바로 ch를 push
			{
				push(&s, ch);
			}
				
			else if (prec(ch) > prec(peek(&s))) 
	//비어있지는 않은데 ch의 우선순위가 s->top보다 높은 경우는 우선순위에 의해서 바로 ch를 push
				push(&s, ch);
			else //ch의 우선순위가 더 작거나 같을 경우
			{
				if (ch == ')') // 이 경우에서도 ch가 ) 이면 (를 찾아야한다
				{
					op = pop(&s); //우선 s에서 하나 pop하고 op에 저장
					while (op != '(' && s.top!=-1) 
				//op가 (도 아니고 s가 비어있지 않으면 계속 반복해서 pop한다
					{
						postfix_arr[idx++] = op; 
				//중간중간 op값은 후위 표기식 배열에 대입
						op = pop(&s); //한번 더 pop해서 op에 대입
					}				
					
				}
				else if (ch == '(') //(인 경우는 우선순위가 가장 낮으므로 바로 넣음
					push(&s, ch);
				else //비어 있지 않은데 괄호도 아닌 경우
				{
					while (!is_empty(&s) && prec(ch) <= prec(peek(&s)))
					{ 
				//s가 비어있지 않고 ch의 우선순위가 s->top의 우선순위보다 작거나 같을 경우 
						postfix_arr[idx++] = pop(&s); 
				//s의 top을 pop해서 후위 표기식 배열에 대입 반복
					}
					push(&s, ch); //다 꺼냈으면 이제 ch를 s에 push
				}

			}
		}
	}
	while (is_empty(&s) != 1) //s가 비어있을 때까지 반복
	{
		postfix_arr[idx++] = pop(&s); //반복해서 pop하고 그 값을 후위 표기식에 대입
	}	
	
	return postfix_arr; //최종 후위 표기식 변환 배열을 반환
}

int main(void)
{		
	element arr[MAX_STACK_SIZE] = { 0 }; //사용자로부터 중위 표기식을 입력받기 위한 배열선언하고 0으로 초기화
	element arr2[MAX_STACK_SIZE] = { 0 }; //후위 표기식으로 변환된 식을 넣기 위한 배열선언하고 0으로 초기화
	element* temp; //반환받은 postfix_arr은 동적할당 된 것이므로 해제 시켜주기 위해 데이터를 받아주는 포인터 변수
	element check[100] = { 0 }; // 사용자가 다시 입력할 것인지 아닌지 yes or no를 입력받는 배열 선언하고 0으로 초기화
	int flag = 1; //이중 반복문 while을 효율적으로 탈출하기 위한 변수 선언

	while (1) //무한 반복
	{		
		err = 0; //반복될 때마다 err=0으로 초기화함으로써 반복적으로 입력할 수 있게 함
		printf("중위표기식 입력 : ");
		scanf("%s", arr); //%s로 문자열을 입력받아서 그대로 arr에 넣음
		
		temp = infix_to_postfix(arr); //반환받은 데이터를 같은 그릇인 포인터 변수 temp에 대입
		printf("\n<중위표기식을 후위표기식으로 변환>\n\n");
		strcpy(arr2, temp); //temp는 해제해야 되기 때문에 그대로 arr2에 문자열 복사

		if (!err) //err가 0이면(에러가 없으면) 조건문 실행, err가 있으면 다음 while문 실행
		{
			printf("후위표기수식: %s\n", arr2);
			printf("결과값: %d\n", eval(arr2));
		}

		while (flag) //flag가 1일 때만 계속 반복
		{
			printf("다시 입력하시겠습니까?(yes/no)  ");
			scanf("%s", check); //yes or no를 문자열로 입력받아서 check에 넣음
			if (strcmp(check, "yes") == 0) //문자열 비교함수를 통해서 같으면 0이므로
			{
				printf("\n");
				break; //첫 번째 while문으로 돌아가기 위해 break
			}

			else if (strcmp(check, "no") == 0) //no를 입력했으면 프로그램이 종료되는 것을 원하므로
				flag = 0; //반복문 탈출을 위해 flag에 0대입
			else //프로그램이 정상적이지 않게 종료되지 않으려면 yes, no가 입력되지 않은 경우에 대해서도 처리를 해주어야한다.
			{
				printf("잘못 입력하셨습니다.\n\n");				
				continue; //yes or no를 다시 입력해야 하므로 두 번째 while문으로 continue
			}
		}
		if (!flag) //no를 입력해서 while문을 탈출한 경우는 flag가 0이므로 조건문이 실행됨
			break; //첫 번째 while문도 탈출
	}
	free(temp); //동적할당된 것을 temp가 포인터로써 가리키고 있으므로 해제 해주어야 한다.
	return 0; //main함수 종료
}