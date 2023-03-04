#include <iostream>

#include "Utilities/Source/BitwiseEditor.h"

int main()
{
	int a = 0;
	LCN::BitwiseEditor bwa{ a };

	const int b = 1;
	LCN::BitwiseEditor bwb{ b };

	bwb[0] = true;

	std::cin.get();
}