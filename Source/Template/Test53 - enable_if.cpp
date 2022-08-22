#include <iostream>
#include <type_traits>

template<typename _Type, size_t _Dim>
class Rect
{
public:
	using ValType = _Type;

	Rect() = default;

	ValType Width() const;

private:
	ValType m_Dimensions[_Dim] = { 0 };
};

template<typename _Type, size_t _Dim>
_Type Rect<_Type, _Dim>::Width() const { static_assert(false); }

template<typename _Type>
_Type Rect<_Type, 2>::Width() const { return m_Dimensions[0]; }

template<typename _Type>
_Type Rect<_Type, 3>::Width() const { return m_Dimensions[1]; }

int main()
{
	Rect<int, 2> rect2;
	Rect<int, 3> rect3;

	rect2.Width();
	rect3.Width();

	std::cin.get();
}