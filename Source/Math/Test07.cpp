#include <iostream>

#include <LCN_Math/Source/Matrix/MatrixBase.h>

template<typename T, size_t L, size_t C>
class StackAllocatedMatrix
{
public:
	using ValType = T;
	using RefType = ValType&;

	ValType operator()(size_t i, size_t j) const { return m_Data[i][j]; }
	RefType operator()(size_t i, size_t j) { return m_Data[i][j]; }

	constexpr size_t Line()   const { return L; }
	constexpr size_t Column() const { return C; }

private:
	ValType m_Data[L][C];
};

template<typename T, size_t L, size_t C>
class Traits<StackAllocatedMatrix<T, L, C>>
{
public:
	enum
	{
		LineAtCT   = L,
		ColumnAtCT = C,
		SizeAtCT   = true
	};

	using ValType = T;
};

template<class MatrixAllocationPolicy>
class _Matrix : public MatrixBase<_Matrix<MatrixAllocationPolicy>>
{};

template<class MatrixAllocationPolicy>
class Traits<_Matrix<MatrixAllocationPolicy>> : public Traits<MatrixAllocationPolicy>
{};

template<typename T, size_t L, size_t C>
using Matrix = _Matrix<StackAllocatedMatrix<T, L, C>>;

int main()
{
	Matrix<float, 3, 3> mat;

	std::cin.get();
}