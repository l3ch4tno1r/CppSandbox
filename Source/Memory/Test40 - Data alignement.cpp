#include <iostream>

struct no_align
{
	char c;
	double d;
	int i;
	char c2[3];
};

struct align
{
	double d;
	int i;
	char c2[3];
	char c;
};

template<typename T>
void MemSet(T& var, char c)
{
	std::memset(&var, c, sizeof(T));
}

int main()
{
	{
		no_align var;

		MemSet(var, '_');

		MemSet(var.c,  'a');
		MemSet(var.d,  'b');
		MemSet(var.i,  'c');
		MemSet(var.c2, 'd');
	}

	{
		align var;

		MemSet(var, '_');

		MemSet(var.c,  'a');
		MemSet(var.d,  'b');
		MemSet(var.i,  'c');
		MemSet(var.c2, 'd');
	}

	std::cin.get();
}