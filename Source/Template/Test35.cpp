#include <iostream>
#include <type_traits>

#include <Utilities/Source/ErrorHandling.h>

template<class ObjType>
class Traits {};

template<class E>
class MatrixExpression
{
public:
	enum
	{
		DimAtCT    = Traits<E>::DimAtCT,
		Dim        = Traits<E>::Dim,
		LineAtCT   = Traits<E>::LineAtCT,
		ColumnAtCT = Traits<E>::ColumnAtCT
	};

	using ValType = typename Traits<E>::ValType;

	int Line()   const { return this->Derived().Line(); }
	int Column() const { return this->Derived().Column(); }

	ValType operator()(int i, int j) const { return this->Derived()(i, j); }

protected:
	E& Derived() { return static_cast<E&>(*this); }
	const E& Derived() const { return static_cast<const E&>(*this); }
};

template<class E>
std::ostream& operator<<(std::ostream& stream, const MatrixExpression<E>& mat)
{
	for (int i = 0; i < mat.Line(); i++)
	{
		for (int j = 0; j < mat.Column(); j++)
		{
			stream << mat(i, j) << ' ';
		}

		stream << std::endl;
	}

	return stream;
}

template<typename T, int L, int C>
class Matrix : public MatrixExpression<Matrix<T, L, C>>
{
public:
	using ValType = T;
	using RefType = T&;

	Matrix() = default;

	Matrix(const std::initializer_list<ValType>& values)
	{
		int idx = 0;

		for (auto& e : values)
		{
			if (idx > L * C)
				break;

			int i = idx / C;
			int j = idx % C;

			m_Data[i][j] = e;

			++idx;
		}
	}

	template<class E>
	Matrix(const MatrixExpression<E>& other)
	{
		for (int i = 0; i < L; i++)
			for (int j = 0; j < C; j++)
				m_Data[i][j] = other(i, j);
	}

	constexpr int Line()   const { return L; }
	constexpr int Column() const { return C; }

	RefType operator()(int i, int j) { return m_Data[i][j]; }
	ValType operator()(int i, int j) const { return m_Data[i][j]; }

private:
	ValType m_Data[L][C];
};

template<typename T, int L, int C>
class Traits<Matrix<T, L, C>>
{
public:
	using ValType = T;

	enum
	{
		DimAtCT    = true,
		Dim        = L,
		LineAtCT   = L,
		ColumnAtCT = C
	};
};

template<class EL, class ER>
class BinaryOpReturnType
{
	static_assert(std::is_same<typename Traits<EL>::ValType, typename Traits<ER>::ValType>::value, "Types must match.");

public:
	using ValType = typename Traits<EL>::ValType;
};

template<class Derived, class EL, class ER>
class BinaryOperation : public MatrixExpression<Derived>
{
public:
	using ValType = typename BinaryOpReturnType<EL, ER>::ValType;

protected:
	const EL& el;
	const ER& er;

	BinaryOperation(const EL& el, const ER& er) :
		el(el),
		er(er)
	{}
};

template<class EL, class ER>
class AddOperation : public BinaryOperation<AddOperation<EL, ER>, EL, ER>
{
public:
	using ThisType = AddOperation<EL, ER>;
	using Base     = BinaryOperation<ThisType, EL, ER>;
	using ValType  = typename Base::ValType;
	
	constexpr int Line()   const { return this->el.Line(); }
	constexpr int Column() const { return this->el.Column(); }

	ValType operator()(int i, int j) const { return this->el(i, j) + this->er(i, j); }

private:
	AddOperation(const EL& el, const ER& er) :
		Base(el, er)
	{}

	template<class _EL, class _ER>
	friend AddOperation<_EL, _ER> operator+(const MatrixExpression<_EL>& el, const MatrixExpression<_ER>& er);
};

template<class Derived, class EL, class ER>
class Traits<BinaryOperation<Derived, EL, ER>>
{
public:
	enum
	{
		DimAtCT = Traits<EL>::DimAtCT && Traits<EL>::DimAtCT,
		Dim     = (Traits<EL>::Dim == Traits<ER>::Dim ? Traits<EL>::Dim : -1)
	};

	using ValType = typename BinaryOpReturnType<EL, ER>::ValType;
};

template<class EL, class ER>
class Traits<AddOperation<EL, ER>> : public Traits<BinaryOperation<AddOperation<EL, ER>, EL, ER>>
{
public:
	enum
	{
		LineAtCT   = (Traits<EL>::LineAtCT   == Traits<ER>::LineAtCT   ? Traits<EL>::LineAtCT   : -1),
		ColumnAtCT = (Traits<EL>::ColumnAtCT == Traits<ER>::ColumnAtCT ? Traits<EL>::ColumnAtCT : -1)
	};
};

template<class _EL, class _ER>
AddOperation<_EL, _ER> operator+(const MatrixExpression<_EL>& el, const MatrixExpression<_ER>& er)
{
	if constexpr (Traits<AddOperation<_EL, _ER>>::DimAtCT)
		static_assert(Traits<AddOperation<_EL, _ER>>::LineAtCT != -1 && Traits<AddOperation<_EL, _ER>>::ColumnAtCT != -1);
	else
		ASSERT(el.Line() == er.Line() && el.Column() == er.Column());

	return AddOperation<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
}

typedef Matrix<float, 3, 1> Vector3Df;

int main()
{
	Vector3Df vec1 = { 1, 2, 3 };
	Vector3Df vec2 = { 4, 5, 6 };

	auto sum = vec1 + vec2;

	bool a = Traits<AddOperation<Vector3Df, Vector3Df>>::DimAtCT;

	std::cout << std::boolalpha << a << std::endl;

	std::cout << sum << std::endl;

	std::cin.get();
}