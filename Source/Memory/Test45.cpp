#include <iostream>
#include <string>
#include <string_view>
#include <iterator>
#include <vector>
#include <regex>

#include "_MemTracker.h"

#define SEPARATOR(X) std::cout << "----------- " << #X << " -----------" << std::endl;

class SplitResultIterator;

class SplitResult
{
public:
	friend SplitResultIterator;

	using Iterator = SplitResultIterator;

	SplitResultIterator begin() const;
	SplitResultIterator end()   const;

	operator std::vector<std::string>() const;

	size_t Count() const;

private:
	SplitResult(const std::string& target) :
		m_Target(target)
	{}

	template<typename StrType>
	SplitResult(const std::string& target, StrType&& delimiter) :
		m_Target(target),
		m_Delimiter(std::forward<StrType>(delimiter))
	{}

	friend SplitResult Split(const std::string&);

	template<typename StrType>
	friend SplitResult Split(const std::string&, StrType&&);

private:
	const std::string& m_Target;
	std::string m_Delimiter{ " " };
};

class SplitResultIterator
{
public:
	SplitResultIterator(const SplitResultIterator& other) :
		m_Spliter(other.m_Spliter),
		m_Start(other.m_Start),
		m_End(other.m_End),
		m_View(other.m_View)
	{}

	SplitResultIterator(const SplitResult& spliter) :
		m_Spliter(spliter),
		m_Start(spliter.m_Target.size())
	{}

	SplitResultIterator(const SplitResult& spliter, bool) :
		m_Spliter(spliter)
	{
		const std::string& target{ m_Spliter.m_Target };
		const std::string& delimiter{ m_Spliter.m_Delimiter };

		m_End = target.find(delimiter);

		m_View = { target.begin(), target.begin() + m_End };
	}

	SplitResultIterator& operator++()
	{
		const std::string& target{ m_Spliter.m_Target };
		const std::string& delimiter{ m_Spliter.m_Delimiter};

		m_Start = std::min(m_End + delimiter.size(), target.size());
		m_End   = std::min(target.find(delimiter, m_Start), target.size());

		m_View = { target.begin() + m_Start, target.begin() + m_End };

		return *this;
	}

	SplitResultIterator operator++(int)
	{
		SplitResultIterator it{ *this };

		++(*this);

		return it;
	}

	std::string_view operator*() const
	{
		return m_View;
	}

	const std::string_view* operator->() const
	{
		return &m_View;
	}

	friend bool operator==(const SplitResultIterator&, const SplitResultIterator&);
	friend bool operator!=(const SplitResultIterator&, const SplitResultIterator&);

private:
	const SplitResult& m_Spliter;

	uint32_t m_Start{ 0 }, m_End{ std::string::npos };
	std::string_view m_View;
};

SplitResult Split(const std::string& str)
{
	return { str };
}

template<typename StrType>
SplitResult Split(const std::string& str, StrType&& delim)
{
	return { str, std::forward<StrType>(delim) };
}

bool operator==(const SplitResultIterator& a, const SplitResultIterator& b)
{
	return &a.m_Spliter == &b.m_Spliter && a.m_Start == b.m_Start;
}

bool operator!=(const SplitResultIterator& a, const SplitResultIterator& b)
{
	return !(a == b);
}

SplitResultIterator SplitResult::begin() const
{
	return { *this, true };
}

SplitResultIterator SplitResult::end() const
{
	return { *this };
}

SplitResult::operator std::vector<std::string>() const
{
	std::vector<std::string> result;
	result.reserve(this->Count());

	for (auto view : *this)
		result.emplace_back(view);

	return result;
}

size_t SplitResult::Count() const
{
	size_t count{ 0 };

	auto it = this->begin(), end = this->end();

	for(; it != end; ++it)
		++count;

	return count;
}

int main()
{
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		SEPARATOR(1);

		std::string str{ "Hello world ! My name is Matt !" };

		SEPARATOR(2);

		SplitResult splitView = Split(str);

		SEPARATOR(3);

		for (auto view : splitView)
			std::cout << view << std::endl;

		SEPARATOR(4);
		{
			auto it = splitView.begin(), end = splitView.end();

			while (it != end)
				std::cout << *(it++) << std::endl;
		}

		SEPARATOR(5);
		{
			for (auto it = splitView.begin(); it != splitView.end(); ++it)
				std::cout << *it << std::endl;
		}

		SEPARATOR(6);

		std::vector<std::string> vec = splitView;

		SEPARATOR(7);
		{
			std::regex regex{ "\\s+" };

			std::sregex_token_iterator it{ str.begin(), str.end(), regex, -1 }, end{};

			for (; it != end; ++it)
				std::cout << *it << std::endl;
		}

		SEPARATOR(End);
	}

	std::cin.get();
}
