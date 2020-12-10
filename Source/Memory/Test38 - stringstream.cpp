#include <iostream>
#include <sstream>

#include "_MemTracker.h"

#include <Utilities/Source/Benchmarking.h>

class LogHelper
{
public:
	~LogHelper()
	{
		std::cout << '\n';
	}

	std::ostream& stream()
	{
		return std::cout;
	}
};

#define LOG() LogHelper().stream() << "---- " << __FUNCTION__ << " @ " << __LINE__
#define VAR(x) ", " #x "[" << x << ']'

namespace LCN
{
	class Allocator : public std::allocator<char>
	{
	public:
		using base = allocator<value_type>;
		using char_allocator_traits = std::allocator_traits<std::allocator<char>>;
		using pointer = char_allocator_traits::pointer;

		Allocator()
		{}

		~Allocator()
		{
			for (Buffer buffer : m_Buffers)
			{
				if(buffer.m_Buffer)
					base::deallocate(buffer.m_Buffer, buffer.m_Size);

				buffer.m_Buffer = 0;
				buffer.m_Size   = 0;
			}
		}

		pointer allocate(std::size_t n, const void* hint)
		{
			throw std::exception("Not implemented");
		}

		pointer allocate(std::size_t n)
		{
			size_t otherbufferidx = (m_CurrentBufferIdx + 1) % 2;

			Buffer& currentbuffer = m_Buffers[m_CurrentBufferIdx];
			Buffer& secondbuffer  = m_Buffers[otherbufferidx];

			if (n > currentbuffer.m_Size)
			{
				if (secondbuffer.m_Buffer)
					base::deallocate(secondbuffer.m_Buffer, secondbuffer.m_Size);

				secondbuffer.m_Buffer = base::allocate(n);
				secondbuffer.m_Size   = n;

				m_CurrentBufferIdx = otherbufferidx;
			}

			return m_Buffers[m_CurrentBufferIdx].m_Buffer;
		}

		void deallocate(pointer p, std::size_t n) {}

	private:
		struct Buffer
		{
			std::size_t m_Size = 0;
			pointer m_Buffer   = nullptr;
		} m_Buffers[2];

		size_t m_CurrentBufferIdx = 0;
	};

	class stringstream : public std::basic_stringstream<char, std::char_traits<char>, Allocator>
	{
	public:
		using base = std::basic_stringstream<char, std::char_traits<char>, Allocator>;

		void Reset()
		{
			base::clear();
			base::str(sm_Empty);
			base::seekg(0);
			base::seekp(0);
		}

	private:
		static const base::_Mystr sm_Empty;
	};

	//using stringstream = std::basic_stringstream<char, std::char_traits<char>, Allocator>;
	//using string       = std::basic_string<char, std::char_traits<char>, Allocator>;
	//using string = stringstream::_Mystr;
}

template<class StringStream>
void TestFunction1()
{
	auto session = MemTracker::Get().BeginScopeBasedSession();

	LOG();
	StringStream ss; // (MyString(255, '\0'));
	ss.clear();

	LOG();
	int x;
	ss << "423";
	ss << " 423";

	LOG();
	ss << " 423jlfskdfjl jfsd sdfdsfkdf dsfg dsfg dfg dfg dsfg df gdf gdfg dsfg dsfgdsfgds";
	LOG();
	ss >> x;
	std::cout << "x = " << x << std::endl;

	LOG();
	ss.clear();
	LOG();
	ss.str({});

	ss.seekg(0);
	ss.seekp(0);

	LOG();
	ss << "1";
	ss >> x;

	std::cout << x << std::endl;
	LOG();
}

template<class StringStream>
void TestFunction2()
{
	auto session = MemTracker::Get().BeginScopeBasedSession();

	size_t testarray[] = { 20, 4, 3, 1, 2, 8, 5, 2, 40, 1, 10, 3, 4, 30, 1, 10, 15, 4 };

	StringStream sstr;

	for (size_t iter : testarray)
	{
		sstr.str({}); sstr.seekg(0); sstr.seekp(0);

		for (size_t i = 0; i < iter; ++i)
			sstr << i;

		//std::cout << sstr.str() << std::endl;
	}
}

template<class StringStream>
void TestFunction3()
{
	//auto session = MemTracker::Get().BeginScopeBasedSession();

	const int iterations = 1000;

	StringStream sstr;
	typename StringStream::_Mystr empty;

	for (int i = 0; i < iterations; ++i)
	{
		sstr.str(empty); sstr.seekg(0); sstr.seekp(0);

		sstr << "Test stringstream " << (i % 10);

		//std::cout << sstr.str() << std::endl;
	}
}

int main()
{
	const size_t iterations = 1000;

	Benchmark::TimePerformance(iterations, TestFunction3<std::stringstream>);
	
	std::cout << "\n///////////////////////////////\n\n";
	
	Benchmark::TimePerformance(iterations, TestFunction3<LCN::stringstream>);

	//LCN::string str("Hello world ! Chaine trop petite ?");
	//
	//auto allocator = str.get_allocator();
	//
	//std::cout << str << std::endl;
	//
	//LCN::string str2 = std::move(str);

	std::cin.get();

	return 0;
}