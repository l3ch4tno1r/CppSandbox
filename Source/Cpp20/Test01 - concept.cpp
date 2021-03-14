#include <iostream>
#include <string>
#include <cstddef>
#include <concepts>
#include <type_traits>
#include <list>
#include <algorithm>

template<typename C>
concept HasValType = requires { typename C::ValType; };

template<typename C>
concept IsPointer = std::is_pointer_v<C>;

template<typename C>
concept IsReference = std::is_reference_v<C>;

template<typename C, typename T>
concept HasParenthesisOperator = requires(C c, size_t i, size_t j, T ret)
{
	{ c.operator()(i, j) } -> std::convertible_to<T>;
};

template<typename C>
concept HasPrintMethod = requires(C c)
{
	{ c.Print() };
};

//---------------------//

template<class Derived>
class Traits;

template<typename Derived>
concept ExpressionRequirement = 
	HasValType<Traits<Derived>> &&
	!IsPointer<typename Traits<Derived>::ValType> &&
	!IsReference<typename Traits<Derived>::ValType>;

template<ExpressionRequirement Derived>
class Expression
{
public:
	using ValType = typename Traits<Derived>::ValType;
};

//---------------------//

template<typename T>
class Vector : public Expression<Vector<T>>
{
public:
	float operator()(size_t i, size_t j) { return 0.0f; };

	void Print() { std::cout << "Hello world !" << std::endl; }
};

template<typename T> requires
	HasPrintMethod<Vector<T>> &&
	HasParenthesisOperator<Vector<T>, T>
class Traits<Vector<T>>
{
public:
	using ValType = T;
};

int main()
{
	Vector<float> v;
	//Vector<float>::ValType a = 1.0f;
	std::cout << v(0, 0) << std::endl;;

	std::cin.get();
}