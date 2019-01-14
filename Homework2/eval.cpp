#include "Map.h"
#include <stack>
#include <iostream>
using namespace std;

int evaluate(string infix, const Map& values, string& postfix, int& result);
bool precedes(char first, char second);
//says weather or not first operator precedes the second one

int val(Map map, char key);
//returns the corresponding value of given letter (key) in the Map




int evaluate(string infix, const Map& values, string& postfix, int& result)
{
	bool doesntexist = 0;
	bool invalid = 0;
	int onum = 0;	//# of open paranthesis
	int cnum = 0;	//# of closed paranthesis
	postfix = "";	//empty postfix string
	stack<char> operators;

//empty string is syntactically invalid:
	if (infix.size() == 0)		
		return 1;

//must not start with operator:
	unsigned int k = 0;
	while (k < infix.size() && infix[k] == ' ' || infix[k] == '(' || infix[k] == ')')	//find first non-blank char
		k++;
	if (infix[k] == '+' || infix[k] == '-' || infix[k] == '*' || infix[k] == '/')
		return 1;				

//must not end with operator:
	unsigned int j = infix.size() - 1;
	while (j >= 0 && infix[j] == ' ' || infix[j] == ')')	//start at end and go backwards till first item is found
		j--;
	if (infix[j] == '+' || infix[j] == '-' || infix[j] == '/' || infix[j] == '*' || infix[j] == '(')
		return 1;


		//I saw the infix to postfix conversion and the calculation pseudocode
		//after I've already written my program
		//that's why I'm not using the exact syntax given
		//(using if statements instead of switch)



	for (unsigned int i = 0; i < infix.size(); i++)		//go through every character of infix one by one
	{
		if (infix[i] >= 'a' && infix[i] <= 'z')		//if character is a letter
		{
			postfix += infix[i];		//append all letters to postfix string

			//check for validity / letters must be single 
			unsigned int j = i + 1;
			while (j < infix.size() && infix[j] == ' ' || infix[j] == '(' || infix[j] == ')')		//find next non-blank char
				j++;
			if (infix[j] >= 'a' && infix[j] <= 'z')		//if next character is a letter
				invalid = 1;
		}

		else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/')		//if character is an operator
		{
			//if operator has higher precedence than last operator on stack, push it on stack
			//else, pop operators off stack and add them to postfix
			//till an operator is found that has lower precedence
			while (!operators.empty() && !precedes(infix[i], operators.top()))
			{
				if (operators.top() != '(')		//to prevent appending '(' to postfix string
				{
					postfix += operators.top();
					operators.pop();
				}
				else
					break;
			}
			operators.push(infix[i]);

			//check for validity / there can't be two operators withought an operand in between
			//there must not be ')' after an operator
			unsigned int j = i + 1;
			while (j < infix.size() && infix[j] == ' ' || infix[j] == '(')		//get to next non-blank character
				j++;
			if (infix[j] == '+' || infix[j] == '-' || infix[j] == '*' || infix[j] == '/' || infix[j] == ')')
				invalid = 1;
		}

		else if (infix[i] == '(')		//if character is '('
		{
			operators.push('(');		//always push '('

			onum++;		//count number of '('

			//check for validity / can't have operator after '(':
			//can't have "()"
			unsigned int j = i + 1;
			while (j < infix.size() && infix[j] == ' ')		//find next non-blank character
				j++;
			if (infix[j] == '+' || infix[j] == '-' || infix[j] == '*' || infix[j] == '/' || infix[j] == ')')
				invalid = 1;
		}

		else if (infix[i] == ')')		//if character is ')'
		{
			cnum++;		//count number of '('

			//pop items off stack while adding them to postfix untill '(' is encountered
			//pop '(' when encountered
			while (!operators.empty() && operators.top() != '(')
			{
				postfix += operators.top();
				operators.pop();
			}
			if (!operators.empty() && operators.top() == '(')
				operators.pop();
		}

		//do nothing if character is a space
		else if (infix[i] == ' ')
			;

		//if character is not a letter or operator or paranthesis or space then it's invalid syntax
		//e.g. uppercase letters are invalid
		else				
		{
			invalid = 1;
			break;
		}

		if (infix[i] >= 'a' && infix[i] <= 'z' && !values.contains(infix[i]))	//if letter not in map
			doesntexist = 1;
		
	}


	//append to postfix whatever is left on operators stack until stack is empty:
	while (!operators.empty())
	{
		postfix += operators.top();
		operators.pop();
	}


	//if infix is syntactically invalid
	//or # of '(' isn't the same as # of ')':
	if (invalid == 1 || cnum != onum)
		return 1;

	//if infix is syntactically valid but a letter is used that's not in the Map:
	if (invalid == 0 && doesntexist == 1)
		return 2;

	//if infix is valid and all letters used exist in the map:
	if (invalid == 0 && cnum == onum && doesntexist == 0)
	{
		stack<int> operands;		//create an empty stack for operands
		int o1;
		int o2;

		//go through characters of postfix one by one:
		for (unsigned int i = 0; i < postfix.size(); i++)
		{
			if (postfix[i] >= 'a' && postfix[i] <= 'z')		//if character an operand
				operands.push(val(values, postfix[i]));		//push corresponding value on operands stack
			

			if (postfix[i] == '+' || postfix[i] == '-' || postfix[i] == '*' || postfix[i] == '/')	//if character is an operator,
			{
				o2 = operands.top();
				operands.pop();
				o1 = operands.top();
				operands.pop();			//pop its operands off the stack,

				//and calculate the result:
				if (postfix[i] == '+')
					operands.push(o1 + o2);
				if (postfix[i] == '-')
					operands.push(o1 - o2);
				if (postfix[i] == '*')
					operands.push(o1*o2);
				if (postfix[i] == '/')
				{
					if (o2 == 0)				//if attempting to divide by zero
						return 3;
					operands.push(o1 / o2);		//push the result back on the stack
				}
			}
		}

		result = operands.top();		//only one item is left on stack and that's the result
		return 0;
	}
}


bool precedes(char first, char second)
{
	if (first == '+' && second == '-' ||
		first == '-' && second == '+' ||
		first == '*' && second == '/' ||
		first == '/' && second == '*' ||
		first == '+' && second == '*' ||
		first == '-' && second == '*' ||
		first == '+' && second == '/' ||
		first == '-' && second == '/' ||
		first == second)
		return false;

	if (second == '(')
		return false;

	return true;
}


int val(Map map, char key)
{
	int val;
	map.get(key, val);
	return val;
}




int main()
{
	Map m;
	int result=-1;
	string postfix;
	char alphabet = 'a';
	for (int i = 0; i < 15; i++)
	{
		m.insert(alphabet, i);
		alphabet++;
	}


	cout << evaluate("d* ( c* ( e - b ) +d ) - f", m, postfix, result) << endl;
	cout << postfix << endl;
	cout << result << endl;
}