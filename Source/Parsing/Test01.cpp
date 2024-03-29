#include <iostream>
#include <sstream>
#include <type_traits>
#include <algorithm>
#include <vector>

#define SEPARATOR(X) std::cout << "---------- " << X << " ----------\n"

namespace LCN::Parsing
{
#pragma region Check single char
	////////////////
	//-- CheckC --//
	////////////////

	struct CheckC
	{
		char c;
	};

	std::istream&
	operator>>(
		std::istream& is,
		const CheckC check)
	{
		char c;
		is >> c;

		if (check.c != c)
			is.setstate(std::ios::failbit);

		return is;
	}

	CheckC
	Check(
		char c)
	{
		return { c };
	}

	struct CheckNC
	{
		size_t n;
		CheckC c;
	};

	inline
	CheckNC
	operator*(
		const size_t n,
		const CheckC c)
	{
		return { n, c };
	}

	inline
	CheckNC
	operator*(
		const CheckC c,
		const size_t n)
	{
		return n * c;
	}

	std::istream&
	operator>>(
		std::istream& is,
		const CheckNC check)
	{
		for (size_t i = 0; i < check.n; ++i)
			is >> check.c;

		return is;
	}

#pragma endregion

#pragma region Check string
	////////////////
	//-- CheckS --//
	////////////////

	struct CheckS
	{
		std::string_view sv;
	};

	std::istream&
	operator>>(
		std::istream& is,
		const CheckS check)
	{
		for (char c : check.sv)
			is >> Check(c);

		return is;
	}

	template<typename _Str>
	CheckS
	Check(
		_Str&& str)
	{
		return { std::forward<_Str>(str) };
	}

#pragma endregion
}

namespace Test02
{
	template<typename _Derived>
	struct Expr
	{};

	template<typename _LHS, typename _RHS>
	struct PipeType;

	template<typename _LHS, typename _RHS>
	constexpr
	typename PipeType<_LHS, _RHS>::Type
	operator>>(
		const _LHS&,
		const Expr<_RHS>&)
	{
		return {};
	}

	template<typename _Type>
	struct PlaceHolder : Expr<PlaceHolder<_Type>>
	{};

	PlaceHolder<int>   Int;
	PlaceHolder<float> Float;

	template<typename _LHS, typename _Type>
	struct PlaceHolderPipe : Expr<PlaceHolderPipe<_LHS, _Type>>
	{};

	template<typename _LHS, typename _Type>
	struct PipeType<_LHS, PlaceHolder<_Type>>
	{
		using Type = PlaceHolderPipe<_LHS, _Type>;
	};
}

namespace Test03
{
	template<typename>
	struct Expr;

	template<typename _Expression, typename ... _Args>
	struct Packed
	{
		Expr<_Expression>& Expression;
		std::tuple<_Args&...> Parameters;

		Packed(Expr<_Expression>& e, _Args&... p)
			: Expression{ e }
			, Parameters{ std::make_tuple(std::ref(p)...) }
		{}

		inline
		void
		Extract(
			std::istream& is)
		{
			Expression.Extract(is, Parameters);
		}
	};

	template<typename _Derived>
	struct Expr
	{
		template<typename ... _Args>
		Packed<_Derived, _Args...>
		operator()(
			_Args& ... args)
		{
			return { *this, args... };
		}

		template<typename ... _Args>
		void
		Extract(
			std::istream& is,
			std::tuple<_Args&...>& target)
		{
			this->Derived().Extract(is, target);
		}

	protected:
		const _Derived& Derived() const { return static_cast<const _Derived&>(*this); }
		      _Derived& Derived()       { return static_cast<      _Derived&>(*this); }
	};

	template<typename _Expression, typename ... _Args>
	std::istream&
	operator>>(
		std::istream& is,
		Packed<_Expression, _Args...>&& packed)
	{
		packed.Extract(is);

		return is;
	}

	template<typename _LHS, typename _RHS>
	struct PipeType;

	template<typename _LHS, typename _RHS>
	constexpr
	typename PipeType<_LHS, _RHS>::Type
	operator>>(
		const Expr<_LHS>&,
		const Expr<_RHS>&)
	{
		return {};
	}

	template<size_t I>
	struct ArgType : Expr<ArgType<I>>
	{
		template<typename ... _Args>
		void
		Extract(
			std::istream& is,
			std::tuple<_Args&...>& target)
		{
			is >> std::get<I>(target);
		}
	};

	template<size_t I, typename _LHS>
	struct ArgTypeRightShift : Expr<ArgTypeRightShift<I, _LHS>>
	{
		using ArgType = ArgType<I>;

		template<typename ... _Args>
		void
		Extract(
			std::istream& is,
			std::tuple<_Args&...>& target)
		{
			_LHS{}.Extract(is, target);
			ArgType{}.Extract(is, target);
		}
	};

	template<typename _LHS, size_t I>
	struct PipeType<_LHS, ArgType<I>>
	{
		using Type = ArgTypeRightShift<I, _LHS>;
	};

	namespace PlaceHolders
	{
		using ArgType0 = ArgType<0>; ArgType0 Arg0;
		using ArgType1 = ArgType<1>; ArgType1 Arg1;
		using ArgType2 = ArgType<2>; ArgType2 Arg2;
		using ArgType3 = ArgType<3>; ArgType3 Arg3;
		using ArgType4 = ArgType<4>; ArgType4 Arg4;
		using ArgType5 = ArgType<5>; ArgType5 Arg5;
	}
}

namespace Test04
{
	namespace Hidden
	{
		template<typename _IStream>
		concept IStream = std::is_base_of_v<
			std::basic_istream<
				typename _IStream::char_type,
				typename _IStream::traits_type>,
			_IStream>;

		enum class Side
		{
			Left,
			Right
		};
	}

	//////////////
	//-- Expr --//
	//////////////

	template<typename _Derived>
	class Expr
	{
	public:
		template<Hidden::IStream _IStream>
		void
		ExtractData(
			_IStream& is) const
		{
			this->Derived().ExtractData(is);
		}

	private:
		_Derived&       Derived()       { return static_cast<_Derived&>(*this); }
		_Derived const& Derived() const { return static_cast<const _Derived&>(*this); }
	};

	template<
		Hidden::IStream _IStream,
		typename _Derived>
	void
	operator>>(
		_IStream& is,
		const Expr<_Derived>& expr)
	{
		expr.ExtractData(is);
	}

	///////////////////
	//-- CheckChar --//
	///////////////////

	template<typename _E, Hidden::Side _Side>
	struct CheckChar : Expr<CheckChar<_E, _Side>>
	{
	public:
		CheckChar(char c, const Expr<_E>& e)
			: m_char{ c }
			, m_expr{ static_cast<const _E&>(e) }
		{}

		template<Hidden::IStream _IStream>
		void
		ExtractData(
			_IStream& is) const
		{
			auto lambda = [&]()
			{
				const char c = is.peek();

				if (c != m_char)
					is.setstate(std::ios::failbit);

				is.get();
			};

			switch (_Side)
			{
			case Test04::Hidden::Side::Left:
				lambda();
				m_expr.ExtractData(is);
				break;
			case Test04::Hidden::Side::Right:
				m_expr.ExtractData(is);
				lambda();
				break;
			default:
				break;
			}
		}

	private:
		char m_char;
		const _E& m_expr;
	};

	template<typename _LHS>
	CheckChar<_LHS, Hidden::Side::Right>
	operator>>(
		const Expr<_LHS>& lhs,
		char c)
	{
		return { c, static_cast<const _LHS&>(lhs) };
	}

	template<typename _RHS>
	CheckChar<_RHS, Hidden::Side::Left>
	operator>>(
		char c,
		const Expr<_RHS>& lhs)
	{
		return { c, static_cast<const _RHS&>(lhs) };
	}

	////////////////
	//-- Reader --//
	////////////////

	template<typename _Target>
	class Reader : public Expr<Reader<_Target>>
	{
	public:
		Reader(_Target& target)
			: m_Target{ target }
		{}

		template<Hidden::IStream _IStream>
		void
		ExtractData(
			_IStream& is) const
		{
			is >> m_Target;
		}

	private:
		_Target& m_Target;
	};

	template<typename _Target>
	Reader<_Target>
	Read(
		_Target& target)
	{
		return { target };
	}
}

int main()
{
	SEPARATOR("Test 01");
	{
		using LCN::Parsing::Check;

		try
		{
			std::stringstream sstr{ "bbbb [1;2;3;4;5;6] eeee" };
			sstr.exceptions(std::ios::failbit | std::ios::badbit);

			int tab[6];
			auto it = std::begin(tab), end = std::end(tab);

			sstr >> Check('b') * 4 >> Check('[') >> *(it++);

			for (; it != end; ++it)
				sstr >> Check(';') >> *it;

			sstr >> Check(']') >> 4 * Check('e');
		}
		catch (const std::ios::failure& e)
		{
			std::cerr << "Exception : " << e.what() << " - " << e.code().message() << std::endl;
		}
	}

	SEPARATOR("Test 02");
	{
		using Test02::Int;
		using Test02::Float;

		//std::istringstream iss{ "(1;2;3)" };
		std::istringstream iss{ "(1)" };
		iss.exceptions(std::ios::failbit | std::ios::badbit);

		struct
		{
			int x, y, z;
		}
		p;

		//auto expr = '(' >> Int >> 2 * (';' >> Int) >> ')';
		auto expr = '(' >> Int >> Float;

		//iss >> expr(p.x);
	}

	SEPARATOR("Test 03");
	{
		using namespace Test03::PlaceHolders;

		//std::stringstream sstr{ "(1, 2, 3)" };
		std::stringstream sstr{ 
			"[[1, 0, 0, 0],"
			 "[0, 1, 0, 0],"
			 "[0, 0, 1, 0],"
			 "[0, 0, 0, 1]]"
		};

		//auto exprLine = '[' >> ArgIt >> 3 * ( ',' >> ++ArgIt ) >> ']';
		auto expr = Arg0 >> Arg1 >> Arg2;
		//auto expr2 = Arg0 >> expr;

		int a, b, c;
		sstr >> expr(a, b, c);

		std::vector<int> vec{ 3, 0, -2, 4, 6 };

		//auto it = std::ranges::min_element(std::move(vec));
		auto it = std::ranges::min_element(vec);

		std::cout << *it << '\n';
	}

	SEPARATOR("Test 04");
	{
		using namespace Test04;

		std::istringstream sstr{ "1;2;3" };

		uint64_t a, b, c;
		auto ra = Read(a), rb = Read(b), rc = Read(c);

		//auto expr = ';' >> Read(a) >> ';';
		sstr >> (ra >> ';' >> rb);
	}

	std::cin.get();
}