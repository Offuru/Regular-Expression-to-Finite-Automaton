#include <iostream>
#include <fstream>
#include <stack>
#include "AFD.h"

bool isValidRegex(std::string regex);

void printMenu();

int main()
{
	std::ifstream fin("input.txt");

	AFD afd1('a');
	AFD afd2('b');
	AFD afd3('c');

	afd1 &= afd2;
	afd1 |= afd3;
	afd1++;	

	AFD regex("aba(aa|bb)*c(ab)*");
	std::cout << regex << "\n";
	std::cout << regex.checkWord("abacab");
	//std::cout << afd1;

	std::string regex;
	fin >> regex;

	if (isValidRegex(regex))
	{
		int answear = 1;
		AFD automaton(regex);

		printMenu();
		std::ofstream fout("output.txt");
		while (answear)
		{
			std::cin >> answear;
			std::string word;

			switch (answear)
			{
				case 1:
					std::cout << automaton << "\n\n";
					break;

				case 2:
					fout << automaton << "\n\n";
					break;

				case 3:
					std::erase(regex, '.');
					std::cout << regex << "\n\n";
					break;

				case 4:
					std::cout << "Type the word: ";
					std::cin >> word;
					if (automaton.checkWord(word))
						std::cout << "The word is valid!\n\n";
					else
						std::cout << "The word is not valid!\n\n";
					break;

				case 0: 
					return 0;

				default:
					break;
			}
			printMenu();
		}
	}

	return 0;
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

void printMenu()
{
	std::cout << "1: Show the automaton in console;\n";
	std::cout << "2: Show the automaton in file;\n";
	std::cout << "3: Show intelligible regex;\n";
	std::cout << "4: Check word;\n";
	std::cout << "0: EXIT.\n";
}
