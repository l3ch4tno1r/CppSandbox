#include <iostream>

#include "Utilities/Experimental/LiteralStringGenerator.h"

template<typename _Type>
struct GeneratorFor
{
	inline static constexpr std::string_view sv = _Type::GetStrView();

	static constexpr size_t Size() { return sv.size(); }

	static constexpr char Value(size_t Idx) { return sv[Idx]; }
};

struct PointSeparatorType
{
	static constexpr std::string_view GetStrView() { return { "." }; }

	using Generator = GeneratorFor<PointSeparatorType>;
};

template<typename _Parent, typename _Derived>
struct Property
{
	using PropName = LCN::JoinLiteral<PointSeparatorType, LCN::TypeList<_Parent, _Derived>>;
};

struct Contact
{
	struct PropFirstName : Property<Contact, PropFirstName> {} FirstName;
};

int main()
{
	Contact::PropFirstName::PropName
	std::cin.get();
}