#include <iostream>
#include <iterator>

#pragma warning(disable : 4996)

template<int From, int To>
class Range
{
public:
	class Iterator : public std::iterator<
		std::input_iterator_tag,
		int,
		int,
		const int*,
		int>
	{
	public:
		explicit Iterator(int num) : m_Num(num) {}

		Iterator& operator++()
		{
			m_Num += To >= From ? 1 : -1;

			return *this;
		}

		Iterator operator++(int)
		{
			Iterator it = *this;

			++(*this);

			return it;
		}

		bool operator==(Iterator other) { return this->m_Num == other.m_Num; }
		bool operator!=(Iterator other) { return !(*this == other); }

		reference operator*() const { return m_Num; }

	private:
		int m_Num = From;
	};

	Iterator begin() { return Iterator(From); }
	Iterator end() { return Iterator(To >= From ? To + 1 : To - 1); }
};

int main()
{
	for (int i : Range<1, 12>())
		std::cout << i << ' ';

	std::cout << std::endl;

	for (int i : Range<-3, -9>())
		std::cout << i << ' ';

	std::cin.get();
}