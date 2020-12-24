#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <list>

struct ColumnInfo
{
	std::string Name;
	std::string Type;
};

bool operator==(const ColumnInfo& a, const ColumnInfo& b)
{
	return 
		a.Name == b.Name &&
		a.Type == b.Type;
}

class ColumInfoHash
{
public:
	size_t operator()(const ColumnInfo& ci) const { return std::hash <std::string> ()(ci.Name); }
};

int main()
{
	try
	{
		std::ifstream file("Ressources/Table.th", std::ios::in);

		if (!file)
			throw std::exception("File not found");

		std::string line;

		const char* tagstofind[] = {
			"TableName",
			"PropertyAccessorList",
			"PropertyList",
			"StaticPropertyList",
			"StaticPropertyInitializer"
		};

		const std::unordered_set<ColumnInfo, ColumInfoHash> columns = {
			{ "Id",        "int" },
			{ "FirstName", "std::string" },
			{ "LastName",  "std::string" }
		};

		std::list<std::string> generated;

		using StrListIterator = std::list<std::string>::iterator;

		std::list<StrListIterator> iterators;

		while (std::getline(file, line))
		{
			generated.push_back(std::move(line));

			StrListIterator last = std::prev(generated.end());

			for (const char* tag : tagstofind)
			{
				if (last->find(tag) != std::string::npos)
					iterators.push_back(last);
			}
		}

		for (const auto& e : iterators)
			std::cout << *e << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}