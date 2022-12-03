#include <iostream>
#include <utility>

template<typename _ValType, size_t L, size_t C>
class BaseGenerator
{
public:
	using ValType = _ValType;

protected:
	static constexpr
	std::pair<size_t, size_t>
	GetCoordFromIdx(
		size_t Idx)
	{
		return { Idx / C, Idx % C };
	}
};

template<typename _ValType, size_t L, size_t C>
struct IdentityGenerator : BaseGenerator<_ValType, L, C>
{
	using BaseType = BaseGenerator<_ValType, L, C>;
	using ValType  = typename BaseType::ValType;

	static constexpr ValType Value(size_t Idx)
	{
		auto [I, J] = BaseType::GetCoordFromIdx(Idx);

		bool diag = I == J;

		return diag * ValType{ 1 } + (!diag) * ValType { 0 };
	}
};

template<typename _ValType, size_t L, size_t C>
struct TestGenerator : BaseGenerator<_ValType, L, C>
{
	using BaseType = BaseGenerator<_ValType, L, C>;
	using ValType  = typename BaseType::ValType;

	static constexpr ValType Value(size_t Idx)
	{
		auto [I, J] = BaseType::GetCoordFromIdx(Idx);

		return static_cast<ValType>((I + J) % 2 );
	}
};

struct ZeroInitType{};

constexpr ZeroInitType ZeroInit;

struct IdInitType{};

constexpr IdInitType IdInit;

struct TestInitType{};

constexpr TestInitType TestInit;

template<typename _ValType, size_t L, size_t C>
struct Matrix
{
	enum : uint8_t
	{
		IsSquare = (L == C)
	};

	using ValType       = _ValType;
	using IdGenerator   = std::conditional_t<IsSquare, IdentityGenerator<ValType, L, C>, void>;
	using TestGenerator = TestGenerator<ValType, L, C>;

	union
	{
		ValType Data[L][C];
		ValType Array[L * C];
	};

	constexpr ValType operator()(size_t i, size_t j) const { return Data[i][j]; }

	constexpr
	Matrix() = default;

	constexpr
	Matrix(ZeroInitType)
		: Array{ ValType(0) }
	{}

	template<typename _Generator, size_t ... Idx>
	constexpr
	Matrix(_Generator, std::index_sequence<Idx...>)
		: Array{ (Idx, _Generator::Value(Idx))... }
	{}

	constexpr
	Matrix(IdInitType)
		: Matrix(IdGenerator{}, std::make_index_sequence<L * C>{})
	{}

	constexpr
	Matrix(TestInitType)
		: Matrix(TestGenerator{}, std::make_index_sequence<L * C>{})
	{}
};

template<typename _IdxSeq>
struct _Test;

template<
	typename _Integral,
	_Integral ... Idx>
struct _Test<std::integer_sequence<_Integral, Idx...>>
{
	static constexpr _Integral Array[] = { (Idx, 2 * Idx)... };
};

template<typename _Integral, size_t Size>
struct Test : _Test<std::make_integer_sequence<_Integral, Size>>
{};

template<
	typename _ValType,
	size_t L,
	size_t C>
auto&
operator<<(
	std::ostream& os,
	const Matrix<_ValType, L, C>& mat)
{
	for (size_t i = 0; i < L; ++i)
	{
		for (size_t j = 0; j < C; ++j)
			os << mat(i, j) << ' ';

		os << '\n';
	}

	return os;
}

namespace Test01
{
	template<size_t _Size, size_t _Idx = 0, size_t ... I>
	struct IdxSeq : IdxSeq<_Size, _Idx + 1, I..., _Idx>
	{
		using BaseType = IdxSeq<_Size, _Idx + 1, I..., _Idx>;
	};

	template<size_t _Size, size_t ... I>
	struct IdxSeq<_Size, _Size, I...>
	{
		static constexpr size_t Tab[_Size] = { I... };
	};
}

namespace Test02
{
	template<
		std::integral _Integral,
		_Integral ... N>
	struct IntSeq
	{
		using ValType = _Integral;
	};

	namespace Hidden
	{
		template<
			typename _T,
			typename ... _Types>
		struct AddSequence;

		template<
			typename _T,
			_T ... _Ints>
		struct AddSequence<IntSeq<_T, _Ints...>>
		{
			using Type = IntSeq<_T, _Ints...>;
		};

		template<
			typename _T,
			_T ... _Ints1,
			_T ... _Ints2>
		struct AddSequence<IntSeq<_T, _Ints1...>, IntSeq<_T, _Ints2...>>
		{
			using Type = IntSeq<_T, _Ints1..., _Ints2...>;
		};
	}

	template<
		typename _Seq1,
		typename _Seq2>
	using AddSequenceType = typename Hidden::AddSequence<_Seq1, _Seq2>::Type;

	namespace Hidden
	{
		template<
			typename _T,
			_T N,
			_T S>
		struct MakeSequence
		{
			using Type = AddSequenceType<IntSeq<_T, S>, typename MakeSequence<_T, N, S + 1>::Type>;
		};

		template<
			typename _T,
			_T N>
		struct MakeSequence<_T, N, N>
		{
			using Type = IntSeq<_T, N>;
		};
	}

	template<
		typename _T,
		_T N>
	using MakeSequence = typename Hidden::MakeSequence<_T, N, 0>::Type;

	template<size_t N>
	using MakeIdxSeq = MakeSequence<size_t, N>;

	template<
		typename _T,
		_T Id0,
		_T ... Ids>
	void PrintSequence(
		IntSeq<_T, Id0, Ids...>)
	{
		std::cout << "{ " << Id0;
		((std::cout << ", " << Ids), ...);
		std::cout << " }\n";
	}

	template<typename _T>
	void PrintSequence(IntSeq<_T>)
	{
		std::cout << "{}\n";
	}
}

int main()
{
	constexpr Matrix<float, 3, 4> mat{ ZeroInit };
	constexpr Matrix<float, 3, 3> identity{ IdInit };
	constexpr Matrix<float, 5, 4> test{ TestInit };

	std::cout << mat      << '\n';
	std::cout << identity << '\n';
	std::cout << test     << '\n';

	constexpr auto size = sizeof(std::make_index_sequence<15>);

	Test<int, 4>::Array;

	using IdxSeqLocal = Test01::IdxSeq<10>;

	auto it = std::begin(IdxSeqLocal::Tab), end = std::end(IdxSeqLocal::Tab);

	std::cout << "{ " << *it++;

	for (; it != end; ++it)
		std::cout << ", " << *it;

	std::cout << " }\n";

	//Test02::MakeIntSeq<int, 10>::MyIntSeq{};
	using IntSeq1 = Test02::IntSeq<int, 0, 1, 2>;
	using IntSeq2 = Test02::IntSeq<int, 3, 4, 5, 6, 7>;

	Test02::Hidden::AddSequence<IntSeq2>::Type;
	Test02::Hidden::AddSequence<IntSeq1, IntSeq2>::Type;
	Test02::AddSequenceType<IntSeq1, IntSeq2>;

	Test02::PrintSequence(Test02::MakeIdxSeq<10>{});
	//Test02::PrintSequence(Test02::MakeIdxSeq<0>{});

	std::cin.get();
}