#include <iostream>
#include "AFD.h"

int main()
{
	AFD afd1('a');
	AFD afd2('b');
	
	std::cout << afd1 << "\n\n\n";

	afd1 &= afd2;

	std::cout << afd1;
}
