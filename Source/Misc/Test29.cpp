#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <random>

#define SEPARATOR(X) std::cout << "-------- " << X << " --------" << std::endl
#define PRINT(X) std::cout << #X << " = " << X << std::endl

template<typename ForwardIt>
void PrintSeq(ForwardIt begin, ForwardIt end)
{
	while (begin != end)
		std::cout << *(begin++) << std::endl;
}

int main()
{
	{
		SEPARATOR(1);

		int tab[] = { 1, 2, 3, 4, 5, 6 };

		PrintSeq(tab, tab + 6);
	}

	{
		SEPARATOR(2);

		std::vector<std::string> stringvec;

		PRINT(sizeof(std::vector<std::string>::iterator));
		PRINT(sizeof(std::vector<int>::iterator));

		stringvec.emplace_back("Joe");
		stringvec.emplace_back("Jack");
		stringvec.emplace_back("William");
		stringvec.emplace_back("Averell");

		PrintSeq(stringvec.begin(), stringvec.end());
	}

	{
		SEPARATOR(3);

		std::list<std::string> stringlist;

		PRINT(sizeof(std::list<std::string>::iterator));

		stringlist.emplace_back("Matt");
		stringlist.emplace_back("Don");
		stringlist.emplace_back("Mel");

		PrintSeq(stringlist.begin(), stringlist.end());
	}	

	{
		SEPARATOR(4);

		int tab[] = { 1, 2, 3, 4, 5, 6, 7 };

		std::random_device rd;
		std::mt19937 g(rd());

		std::shuffle(tab, tab + 7, g);

		PrintSeq(tab, tab + 7);
	}	

	std::cin.get();
}