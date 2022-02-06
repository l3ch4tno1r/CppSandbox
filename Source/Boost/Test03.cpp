#include <iostream>

#include <boost/stacktrace.hpp>

void f0() { std::cout << boost::stacktrace::stacktrace{}; }
void f1() { f0(); }
void f2() { f1(); }
void f3() { f2(); }
void f4() { f3(); }

int main()
{
	f4();

	std::cin.get();
}