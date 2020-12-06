#include <iostream>
#include <sstream>

#include "_MemTracker.h"

class LogHelper {
public:
	~LogHelper() {
		std::cout << out.str() << '\n';
	}

	std::ostream &stream() {
		return out;
	}

private:
	std::ostringstream out;
};

#define LOG() LogHelper().stream() << __FUNCTION__ << '(' << __LINE__ << ")"
#define VAR(x) ", " #x "[" << x << ']'

class MyAllocator : public std::allocator<char> {
public:
	using base = allocator<value_type>;
	using base::allocator;

	value_type* allocate(std::size_t n, const void * hint) {
		LOG() << VAR(n);
		return base::allocate(n, hint);
	}

	value_type* allocate(std::size_t n) {
		LOG() << VAR(n);
		return base::allocate(n);
	}

	void deallocate(value_type* p, std::size_t n) {
		LOG() << VAR(n);
		base::deallocate(p, n);
	}
};

using MySStream = std::basic_stringstream<char, std::char_traits<char>, MyAllocator>;
using MyString = std::basic_string<char, std::char_traits<char>, MyAllocator>;

int main() {
	auto session = MemTracker::Get().BeginScopeBasedSession();

	MySStream ss; // (MyString(255, '\0'));
	ss.clear();

	int x;
	ss << "423";
	ss << " 423";

	LOG();
	ss << " 423jlfskdfjl jfsd sdfdsfkdf dsfg dsfg dfg dfg dsfg df gdf gdfg dsfg dsfgdsfgds";
	LOG();
	ss >> x;

	LOG();
	ss.clear();
	ss.str({});

	ss.seekg(0);
	ss.seekp(0);

	LOG();
	ss << "1";
	ss >> x;

	std::cout << x << std::endl;
	LOG();

	std::cin.get();

	return 0;
}