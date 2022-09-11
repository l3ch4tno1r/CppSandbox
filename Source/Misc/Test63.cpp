#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <atomic>

using namespace std::chrono_literals;

namespace Test
{
	template<class _Expr, class _Duration>
	class FormatExec;

	template<class _Derived>
	class FormatExpr
	{
	public:
		template<typename _Rep, typename _Period>
		void Format(std::ostream& os, const std::chrono::duration<_Rep, _Period>& duration) const
		{
			this->Derived().Format(os, duration);
		}

		template<typename _Rep, typename _Period>
		FormatExec<FormatExpr, std::chrono::duration<_Rep, _Period>>
		operator()(
			const std::chrono::duration<_Rep, _Period>&) const;

	protected:
		const _Derived& Derived() const { return static_cast<const _Derived&>(*this); }
		      _Derived& Derived()       { return static_cast<_Derived&>(*this); }
	};

	template<class _Derived, typename _Rep, typename _Period>
	class FormatExec<FormatExpr<_Derived>, std::chrono::duration<_Rep, _Period>>
	{
	public:
		using ExpressionType = FormatExpr<_Derived>;
		using DurationType   = std::chrono::duration<_Rep, _Period>;

	private:
		FormatExec(const ExpressionType& expr, const DurationType& dur) :
			expr{ expr },
			dur{ dur }
		{}

		friend ExpressionType;

		template<class _Expr, class _Duration>
		friend std::ostream& operator<<(std::ostream&, const FormatExec<_Expr, _Duration>&);

	private:
		const ExpressionType& expr;
		const DurationType& dur;
	};

	template<class _Expr, class _Duration>
	std::ostream&
	operator<<(
		std::ostream& os,
		const FormatExec<_Expr, _Duration>& formatExec)
	{
		formatExec.expr.Format(os, formatExec.dur);
		return os;
	}

	template<class _Derived>
	template<typename _Rep, typename _Period>
	FormatExec<FormatExpr<_Derived>, std::chrono::duration<_Rep, _Period>>
	FormatExpr<_Derived>::operator()(
		const std::chrono::duration<_Rep, _Period>& duration) const
	{
		return { *this, duration };
	}

	/*
	template<class _Derived>
	std::ostream&
	operator<<(
		std::ostream& os,
		const FormatExpr<_Derived>& expr)
	{
		return *this;
	}
	*/

	template<class _LHS, class _RHS>
	class SepExpr : public FormatExpr<SepExpr<_LHS, _RHS>>
	{
	public:
		template<typename _Rep, typename _Period>
		void Format(std::ostream& os, const std::chrono::duration<_Rep, _Period>& duration) const
		{
			lhs.Format(os, duration);
			os << ':';
			rhs.Format(os, duration);
		}

	private:
		SepExpr(const _LHS& left, const _RHS& right) :
			lhs{ left },
			rhs{ right }
		{}

		template<class _LHS2, class _RHS2>
		friend SepExpr<_LHS2, _RHS2> operator|(const FormatExpr<_LHS2>&, const FormatExpr<_RHS2>&);

	private:
		const _LHS& lhs;
		const _RHS& rhs;
	};

	template<class _LHS, class _RHS>
	SepExpr<_LHS, _RHS>
	operator|(
		const FormatExpr<_LHS>& lhs,
		const FormatExpr<_RHS>& rhs)
	{
		return { static_cast<const _LHS&>(lhs), static_cast<const _RHS&>(rhs) };
	}

	class HHType : public FormatExpr<HHType>
	{
	public:
		template<typename _Rep, typename _Period>
		void Format(std::ostream& os, const std::chrono::duration<_Rep, _Period>& duration) const
		{
			os << std::chrono::duration_cast<std::chrono::hours>(duration).count();
		}
	};

	constexpr HHType hh;

	class MMType : public FormatExpr<MMType>
	{

	public:
		template<typename _Rep, typename _Period>
		void Format(std::ostream& os, const std::chrono::duration<_Rep, _Period>& duration) const
		{
			os << std::setw(2) << std::setfill('0') << std::chrono::duration_cast<std::chrono::minutes>(duration % 1h).count();
		}
	};

	constexpr MMType mm;

	class SSType : public FormatExpr<SSType>
	{
	public:
		template<typename _Rep, typename _Period>
		void Format(std::ostream& os, const std::chrono::duration<_Rep, _Period>& duration) const
		{
			os << std::setw(2) << std::setfill('0') << std::chrono::duration_cast<std::chrono::seconds>(duration % 1min).count();
		}
	};

	constexpr SSType ss;

	class MSType : public FormatExpr<MSType>
	{
	public:
		template<typename _Rep, typename _Period>
		void Format(std::ostream& os, const std::chrono::duration<_Rep, _Period>& duration) const
		{
			os << std::setw(3) << std::setfill('0') << std::chrono::duration_cast<std::chrono::milliseconds>(duration % 1s).count();
		}
	};

	constexpr MSType ms3;
}

int main()
{
	auto hhmmss = Test::hh | Test::mm | Test::ss;

	std::atomic_bool run{ true };
	auto tp = std::chrono::high_resolution_clock::now(), start = tp;
	std::chrono::seconds ellapsed{ 0 };

	std::thread t([&]()
	{
		for(; run; ++ellapsed, std::this_thread::sleep_until(tp += 1s))
			std::cout << '\r' << hhmmss(ellapsed);

		std::cout << "\nDone !\n";
	});

	std::cin.get();

	run = false;
	auto end = std::chrono::high_resolution_clock::now();

	t.join();

	auto mmssms3 = Test::mm | Test::ss | Test::ms3;

	std::cout << "Ellapsed : " << mmssms3(end - start) << '\n';

	constexpr size_t i = 0xFFFFFFFF;
	constexpr std::chrono::seconds sec{ i };

	constexpr auto min   = std::chrono::duration_cast<std::chrono::minutes>(sec);
	constexpr auto hours = std::chrono::duration_cast<std::chrono::hours>(sec);
	constexpr auto days  = std::chrono::duration_cast<std::chrono::days>(sec);
	constexpr auto weeks = std::chrono::duration_cast<std::chrono::weeks>(sec);
	constexpr auto years = std::chrono::duration_cast<std::chrono::years>(sec);
	

	std::cin.get();
}