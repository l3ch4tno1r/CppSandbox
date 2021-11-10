#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>

/////////////////////
//-- Test struct --//
/////////////////////

struct Point
{
	int X, Y;

	int  operator[](uint32_t idx) const { return *((int*)this + idx); }
	int& operator[](uint32_t idx)       { return *((int*)this + idx); }

	enum
	{
		Dim = 2
	};

	using ValType = int;
};

std::istream& operator>>(std::istream& is, Point& point)
{
	is >> point.X >> point.Y;

	return is;
}

std::ostream& operator<<(std::ostream& os, const Point& point)
{
	os << '(' << point.X << ", " << point.Y << ')';

	return os;
}

////////////////
//-- KDTree --//
////////////////

template<class PointType>
class KDTree
{
public:
	enum
	{
		Dim = PointType::Dim
	};

	using ValType       = typename PointType::ValType;
	using ConstIterator = std::vector<PointType>::const_iterator;

	KDTree() = delete;

	template<class ... Args>
	KDTree(Args&& ... args) :
		m_DataSet(std::forward<Args>(args)...)
	{}

	KDTree(const KDTree& other) :
		m_DataSet(other.m_DataSet)
	{}

	/*
	template<>
	KDTree(KDTree&& other) :
		m_DataSet(std::move(other.m_DataSet))
	{}
	*/

	KDTree(KDTree&&) = delete;

	ConstIterator begin() const { return m_DataSet.begin(); }
	ConstIterator end()   const { return m_DataSet.end(); }

	const PointType& operator[](uint32_t idx) const { return m_DataSet[idx]; }

private:
	std::vector<PointType> m_DataSet;
};

int main()
{
	using KDTreePoint = KDTree<Point>;

	std::ifstream ifs("Ressources/Dataset.txt", std::ios::in);

	if (!ifs)
		return EXIT_FAILURE;

	std::istream_iterator<Point> begin{ ifs }, end{};

	KDTreePoint tree{ begin, end };
	KDTreePoint t{ tree };

	std::cout << tree[12] << std::endl;

	std::cin.get();
}