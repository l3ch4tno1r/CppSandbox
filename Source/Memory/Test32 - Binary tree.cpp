#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>

#include <cstdlib>
#include <ctime>

#include "Utilities/ErrorHandling.h"
#include "Containers/BinarySearchTree.h"

std::vector<std::string> FillDico(const char* filename)
{
	std::vector<std::string> v;
	std::ifstream dico(filename);

	if (!dico)
		throw std::exception("File not found.");

	std::string line;

	while (std::getline(dico, line))
		v.emplace_back(line.c_str());

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(v.begin(), v.end(), g);

	return v;
}

int main()
{
	try
	{
		std::vector<std::string> v = FillDico("Ressources/liste_francais.txt");

		Tree<std::string> tree;

		for (std::string& e : v)
			tree.Insert(e);

		tree.Display();

		std::cout << "\n---------- Size : " << tree.Size() << " ----------\n" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}