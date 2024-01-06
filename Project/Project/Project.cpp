#include <iostream>
#include "AFD.h"

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
