#include <iostream>
#include <fstream>
#include <stack>
#include "AFD.h"

bool isValidRegex(std::string regex);

int main()
{
	AFD afd1('a');
	AFD afd2('b');
	AFD afd3('c');

	afd1 &= afd2;
	afd1 |= afd3;
	afd1++;	

	AFD regex("aba(aa|bb)*c(ab)*");
	//std::cout << regex;
	std::cout << regex.checkWord("abacab");
	//std::cout << afd1;
}

bool isValidRegex(std::string regex)
{
	std::stack<char> parenthesesStack;
	for (auto itChr = regex.cbegin(); itChr != regex.cend(); ++itChr)
	{
		if (*itChr == '(')
		{
			parenthesesStack.push(*itChr);

			if (itChr + 1 != regex.cend())
			{
				if (*(itChr + 1) == '|') return false;
				if (*(itChr + 1) == '*') return false;
				if (*(itChr + 1) == '.') return false;
			}
		}
		else if (*itChr == ')')
			parenthesesStack.pop();
		else
		{
			if (*itChr == '.' && itChr + 1 == regex.cend()) return false;
			if (*itChr == '|' && itChr + 1 == regex.cend()) return false;
			if (itChr + 1 != regex.cend())
			{
				if (*itChr == '.' && (*(itChr + 1) == '.' || *(itChr + 1) == '|' || *(itChr + 1) == '*' || *(itChr + 1) == ')' || itChr == regex.cbegin())) return false;
				if (*itChr == '*' && (*(itChr + 1) == '*' || itChr == regex.cbegin())) return false;
				if (*itChr == '|' && (*(itChr + 1) == '*' || *(itChr + 1) == ')' || *(itChr + 1) == '.' || itChr == regex.cbegin())) return false;
			}

		}
	}
	if (!parenthesesStack.empty()) return false;
	return true;
}
