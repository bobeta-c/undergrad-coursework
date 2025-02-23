#include <iostream>
#include <string>
#include "Map.h"
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;


bool precedence(char op1, char op2);

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
	postfix = "";
	int return_val = 0;
	stack<char> operators;
	char previous = 0;
	int num_parens = 0;
	for(int i = 0; infix[i] != 0; i++)
	{
		char ch = infix[i];
		cerr << i << ' ';
		if(ch >= 'a' && ch <= 'z'){
			if(previous >= 'a' && previous <= 'z' || previous == ')' ){
				cerr << "two numbers followed by eachother" << endl;
				return 1;
			}
			int tempv;
			if(!values.get(ch,tempv)){
				cerr << "character not in Map" << endl;
			       	return_val = 2;
			}
			postfix += ch;
			cerr << ch << " : ";
		}
		else if(ch == '('){
			if(!(previous == 0 || previous == '*' || previous == '(' || previous == '/' || previous == '+' || previous == '-')){
				cerr << "open paren following character" << endl;
				return 1;
			}
			operators.push(ch);
			num_parens++;
			cerr << ch << " : ";
		}
		else if(ch == ')'){
			if(num_parens <= 0){
				cerr << "solo closed paren" << endl;
				return 1;
			}
			ch = operators.top();
			operators.pop();
			while(ch != '('){
				postfix += ch;
				ch = operators.top();
				operators.pop();
			}
		}
		else if(ch == '*' || ch == '/' || ch == '+' || ch == '-'){
			if(!(previous >= 'a' && previous <= 'z' || previous == ')')){
				cerr << previous << ' ' << ch << endl;
				cerr << "operator not followed by operand" << endl;
				return 1;
			}
			while(!operators.empty() && operators.top() != '(' && precedence(ch,operators.top())){

				postfix += operators.top();
				operators.pop();
			}
			operators.push(ch);
			cerr << operators.top() << " : ";
		}
		else if(ch != ' '){
			return 1;
		}
		cerr << postfix << endl;
		if(ch != ' '){
			previous = infix[i];
		}
	}
	while(!operators.empty()){
		postfix += operators.top();
		operators.pop();
	}

	cerr << "Evaluating Postfix: " << postfix << endl << "with return val: " << return_val << endl;
	if(postfix[0] == 0){
		cerr << "no postfix";
		return 1;
	}
	if(return_val == 2){
		cerr << "value not in map" << endl;
		return 2;
	}
	stack<int> operands;
	for(int i = 0; postfix[i] != 0; i++){
		if(postfix[i] >= 'a' && postfix[i] <= 'z'){
			int value;
			values.get(postfix[i], value);
			operands.push(value);
		}
		else{

			int v2 = operands.top();
			operands.pop();
			if(operands.empty()){
				cerr << "only 1 operand" << endl;
				return 1;
			}
			int v1 = operands.top();
			operands.pop();
			int v3;
			if(postfix[i] == '*') v3 = v1*v2;
			else if(postfix[i] == '/')
			{
				if(v2 == 0){
					cerr << "error: divide by zero" << endl;
					return 3;
				}
				v3 = v1/v2;
			}
			else if(postfix[i] == '+') v3 = v1+v2;
			else if(postfix[i] == '-') v3 = v1-v2;
			operands.push(v3);
		}
	}
	result = operands.top();
	return return_val;


	
}

bool precedence(char op1, char op2){
	//returns true if op1 <= op2
	//might be buggy
	if(op1 == '+' || op1 == '-'){
		return true;
	}
	else if(op2 == '*' || op2 == '/'){
		return true;
	}
	return false;
}



int main()
{
	    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
	    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
	    Map m;
	    for (int k = 0; vars[k] != '#'; k++)
		m.insert(vars[k], vals[k]);
	    string pf;
	    int answer;
	    assert(evaluate("a+ e", m, pf, answer) == 0  &&
				    pf == "ae+"  &&  answer == -6);
	    answer = 999;
	    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
	    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
	    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
	    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
	    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
	    assert(evaluate("()o", m, pf, answer) == 1  &&  answer == 999);
	    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
	    assert(evaluate("y(*o)", m, pf, answer) == 1  &&  answer == 999);
	    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
	    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
	      // unary operators not allowed:
	    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
	    assert(evaluate("a*b", m, pf, answer) == 2  &&
				    pf == "ab*"  &&  answer == 999);
	    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
				    pf == "yoau-*+"  &&  answer == -1);
	    answer = 999;
	    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
				    pf == "oyy-/"  &&  answer == 999);
	    assert(evaluate(" a  ", m, pf, answer) == 0  &&
				    pf == "a"  &&  answer == 3);
	    assert(evaluate("((a))", m, pf, answer) == 0  &&
				    pf == "a"  &&  answer == 3);
	    assert(evaluate("() + a", m, pf, answer) == 1);
	    assert(evaluate("a-(a)", m, pf, answer) == 0 && pf == "aa-" && answer == 0);
	    assert(evaluate("a+e+(i+)",m, pf, answer) == 1);
	    cout << "Passed all tests" << endl;
}
