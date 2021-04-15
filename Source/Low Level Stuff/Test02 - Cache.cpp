#include <iostream>

int main()
{
	char tab[5][5] = {
		'a', 'b', 'c', 'd', 'e',
		'f', 'g', 'h', 'i', 'j',
		'k', 'l', 'm', 'n', 'o',
		'p', 'q', 'r', 's', 't',
		'u', 'v', 'w', 'x', 'y'
	};

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			tab[i][j] = '*';

	for (int j = 0; j < 5; j++)
		for (int i = 0; i < 5; i++)
			tab[i][j] = '@';

	for (char* ptr = (char*)tab; ptr != (char*)tab + 25; ++ptr)
		*ptr = '#';

	std::cin.get();
}