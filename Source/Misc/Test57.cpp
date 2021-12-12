#include <iostream>
#include <iterator>
#include <fstream>

struct Data
{
	char Id[4];
	float Latitude, Longitude;
};

std::istream& operator>>(std::istream& is, Data& line)
{
	for (char& c : line.Id)
		is >> c;

	is >> line.Latitude >> line.Longitude;

	return is;
}

template<typename DataType>
class Raw
{
private:
	const DataType& m_Data;

	Raw(const DataType& dataType) : m_Data{ dataType } {}

	template<typename _DataType>
	friend Raw<_DataType> ToRaw(const _DataType&);

	friend std::ostream& operator<<(std::ostream&, const Raw&);
};

template<typename DataType>
Raw<DataType> ToRaw(const DataType& dataType)
{
	return { dataType };
}

using RawData = Raw<Data>;

std::ostream& operator<<(std::ostream& os, const RawData& line)
{
	const char* ptr = (const char*)&line.m_Data;
	const char* end = ptr + sizeof(Data);

	while (ptr != end)
		os << *ptr++;

	return os;
}

int main()
{
	try
	{
		std::ifstream in{ "Ressources/DatasetIn.txt", std::ios::in };

		if (!in)
			throw std::exception{ "File Kaputt !!!" };

		std::ofstream out{ "Ressources/DatasetOut.txt", std::ios::out };

		std::istream_iterator<Data> it{ in }, end{};

		for (; it != end; ++it)
			out << ToRaw(*it);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}