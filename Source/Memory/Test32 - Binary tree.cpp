#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>

#include <cstdlib>
#include <ctime>

#include "Utilities/Source/ErrorHandling.h"
#include "Containers/BinarySearchTree.h"

#define SEPARATOR(X) std::cout << "\n----------- " << X << " -----------" << std::endl

std::vector<std::string> FillDico(const char* filename)
{
	std::vector<std::string> v;
	std::ifstream dico(filename);

	if (!dico)
		throw std::exception("File not found.");

	std::string line;

	while (std::getline(dico, line))
		v.emplace_back(std::move(line));

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

		LCN::BinarySearchTree<std::string> tree;

		for (std::string& e : v)
			tree.Insert(e);

		//tree.Display();

		std::cout << "\n---------- Size : " << tree.Size() << " ----------" << std::endl;

		SEPARATOR(1);

		auto it = tree.Begin();

		//it++;

		std::cout << *it << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}