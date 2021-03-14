#pragma once

namespace LCN::SQLiteEntity
{
#pragma region Expression
	////////////////////
	//-- Expression --//
	////////////////////

	template<class E>
	class Expression
	{
	public:
		// Must override
		inline size_t Size() const { return this->Derived().Size(); }

		inline char operator[](size_t i) const { return this->Derived()[i]; }

		// Methods
		std::string ToString() const
		{
			std::string result(this->Size(), '\0');

			for (size_t i = 0; i < this->Size(); ++i)
				result[i] = (*this)[i];

			return result;
		}

		const char* ToCString() const
		{
			char* result = new char[this->Size() + 1];

			for (size_t i = 0; i < this->Size(); ++i)
				result[i] = (*this)[i];

			result[this->Size()] = '\0';

			return result;
		}

		inline operator std::string() const { return this->ToString(); }

		inline operator char*() const { return this->ToCString(); }

	protected:
		inline E& Derived() { return static_cast<E&>(*this); }
		inline const E& Derived() const { return static_cast<const E&>(*this); }
	};

#pragma endregion

#pragma region Variable
	//////////////////
	//-- Entities --//
	//////////////////

	template<class Derived>
	class Variable : public Expression<Derived>
	{};

#pragma endregion

#pragma region Predicate

#pragma region Bases
	///////////////////
	//-- Predicate --//
	///////////////////

	template<class Derived>
	class Predicate : public Expression<Derived>
	{};

	//////////////////
	//-- Unary Op --//
	//////////////////

	template<class Derived, class E>
	class UnaryPredicate : public Predicate<Derived>
	{
	public:
		inline size_t Size() const { return 2 + Derived::OperatorName().size() + this->e.Size(); }

		inline char operator[](size_t i) const
		{
			size_t i_off = i;

			const std::string& operatorname = Derived::OperatorName();

			if (i_off < operatorname.size())
				return operatorname[i_off];

			i_off -= operatorname.size();

			if (0 == i_off)
				return '(';

			--i_off;

			if (i_off < this->e.Size())
				return this->e[i_off];

			i_off -= this->e.Size();

			if (0 == i_off)
				return ')';

			throw std::out_of_range("Index out of range.");
		}

	protected:
		const E& e;

		UnaryPredicate(const E& e) :
			e(e)
		{}
	};

	///////////////////
	//-- Binary Op --//
	///////////////////

	template<class Derived, class EL, class ER>
	class BinaryPredicate : public Predicate<Derived>
	{
	public:
		inline size_t Size() const { return 2 + Derived::OperatorName().size() + this->el.Size() + this->er.Size(); }

		inline char operator[](size_t i) const
		{
			if (0 == i)
				return '(';

			if (this->Size() - 1 == i)
				return ')';

			size_t offseted = i - 1;

			if (offseted < this->el.Size())
				return this->el[offseted];

			offseted -= this->el.Size();

			const std::string& operatorname = Derived::OperatorName();

			if (offseted < operatorname.size())
				return operatorname[offseted];

			offseted -= operatorname.size();

			if (offseted < this->er.Size())
				return this->er[offseted];

			throw std::out_of_range("Index out of range.");
		}

	protected:
		const EL& el;
		const ER& er;

		BinaryPredicate(const EL& el, const ER& er) :
			el(el),
			er(er)
		{}
	};
#pragma endregion

#pragma region Comparison
	////////////////////
	//-- Comparison --//
	////////////////////

	template<class Derived, class EL, class ER>
	class ComparisonStatement : public BinaryPredicate<Derived, EL, ER>
	{
	public:
		using Base = BinaryPredicate<Derived, EL, ER>;

	protected:
		ComparisonStatement(const EL& el, const ER& er) :
			Base(el, er)
		{}
	};

	///////////////
	//-- Equal --//
	///////////////

	template<class EL, class ER>
	class EqualStatement : public ComparisonStatement<EqualStatement<EL, ER>, EL, ER>
	{
	public:
		using Base = ComparisonStatement<EqualStatement<EL, ER>, EL, ER>;

		static const std::string& OperatorName()
		{
			static std::string name(" = ");
			return name;
		}

	private:
		EqualStatement(const EL& el, const ER& er) :
			Base(el, er)
		{}

		template<class _EL, class _ER>
		friend EqualStatement<_EL, _ER> operator==(const Variable<_EL>&, const Variable<_ER>&);
	};

	template<class _EL, class _ER>
	EqualStatement<_EL, _ER> operator==(const Variable<_EL>& el, const Variable<_ER>& er)
	{
		return EqualStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
	}

	///////////////////
	//-- Not Equal --//
	///////////////////

	template<class EL, class ER>
	class NotEqualStatement : public ComparisonStatement<NotEqualStatement<EL, ER>, EL, ER>
	{
	public:
		using Base = ComparisonStatement<NotEqualStatement<EL, ER>, EL, ER>;

		static const std::string& OperatorName()
		{
			static std::string name(" <> ");
			return name;
		}

	private:
		NotEqualStatement(const EL& el, const ER& er) :
			Base(el, er)
		{}

		template<class _EL, class _ER>
		friend NotEqualStatement<_EL, _ER> operator!=(const Variable<_EL>&, const Variable<_ER>&);
	};

	template<class _EL, class _ER>
	NotEqualStatement<_EL, _ER> operator!=(const Variable<_EL>& el, const Variable<_ER>& er)
	{
		return NotEqualStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
	}

	/////////////////
	//-- Greater --//
	/////////////////

	template<class EL, class ER>
	class GreaterStatement : public ComparisonStatement<GreaterStatement<EL, ER>, EL, ER>
	{
	public:
		using Base = ComparisonStatement<GreaterStatement<EL, ER>, EL, ER>;

		static const std::string& OperatorName()
		{
			static std::string name(" > ");
			return name;
		}

	private:
		GreaterStatement(const EL& el, const ER& er) :
			Base(el, er)
		{}

		template<class _EL, class _ER>
		friend GreaterStatement<_EL, _ER> operator>(const Variable<_EL>&, const Variable<_ER>&);
	};

	template<class _EL, class _ER>
	GreaterStatement<_EL, _ER> operator>(const Variable<_EL>& el, const Variable<_ER>& er)
	{
		return GreaterStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
	}

	//////////////////////////
	//-- Greater or equal --//
	//////////////////////////

	template<class EL, class ER>
	class GreaterOrEqualStatement : public ComparisonStatement<GreaterOrEqualStatement<EL, ER>, EL, ER>
	{
	public:
		using Base = ComparisonStatement<GreaterOrEqualStatement<EL, ER>, EL, ER>;

		static const std::string& OperatorName()
		{
			static std::string name(" >= ");
			return name;
		}

	private:
		GreaterOrEqualStatement(const EL& el, const ER& er) :
			Base(el, er)
		{}

		template<class _EL, class _ER>
		friend GreaterOrEqualStatement<_EL, _ER> operator>=(const Variable<_EL>&, const Variable<_ER>&);
	};

	template<class _EL, class _ER>
	GreaterOrEqualStatement<_EL, _ER> operator>=(const Variable<_EL>& el, const Variable<_ER>& er)
	{
		return GreaterOrEqualStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
	}

	//////////////
	//-- Less --//
	//////////////

	template<class EL, class ER>
	class LessStatement : public ComparisonStatement<LessStatement<EL, ER>, EL, ER>
	{
	public:
		using Base = ComparisonStatement<LessStatement<EL, ER>, EL, ER>;

		static const std::string& OperatorName()
		{
			static std::string name(" < ");
			return name;
		}

	private:
		LessStatement(const EL& el, const ER& er) :
			Base(el, er)
		{}

		template<class _EL, class _ER>
		friend LessStatement<_EL, _ER> operator<(const Variable<_EL>&, const Variable<_ER>&);
	};

	template<class _EL, class _ER>
	LessStatement<_EL, _ER> operator<(const Variable<_EL>& el, const Variable<_ER>& er)
	{
		return LessStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
	}

	///////////////////////
	//-- Less or equal --//
	///////////////////////

	template<class EL, class ER>
	class LessOrEqualStatement : public ComparisonStatement<LessOrEqualStatement<EL, ER>, EL, ER>
	{
	public:
		using Base = ComparisonStatement<LessOrEqualStatement<EL, ER>, EL, ER>;

		static const std::string& OperatorName()
		{
			static std::string name(" <= ");
			return name;
		}

	private:
		LessOrEqualStatement(const EL& el, const ER& er) :
			Base(el, er)
		{}

		template<class _EL, class _ER>
		friend LessOrEqualStatement<_EL, _ER> operator<=(const Variable<_EL>&, const Variable<_ER>&);
	};

	template<class _EL, class _ER>
	LessOrEqualStatement<_EL, _ER> operator<=(const Variable<_EL>& el, const Variable<_ER>& er)
	{
		return LessOrEqualStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
	}

#pragma endregion

#pragma region Logical
	//////////////////////////
	//-- Logical negation --//
	//////////////////////////

	template<class E>
	class NotStatment : public UnaryPredicate<NotStatment<E>, E>
	{
	public:
		using Base = UnaryPredicate<NotStatment<E>, E>;

		static const std::string& OperatorName()
		{
			static std::string name("not ");
			return name;
		}

	private:
		NotStatment(const E& e) :
			Base(e)
		{}

		template<class _E>
		friend NotStatment<_E> operator!(const Predicate<_E>&);
	};

	template<class _E>
	NotStatment<_E> operator!(const Predicate<_E>& e)
	{
		return NotStatment<_E>(static_cast<const _E&>(e));
	}

	/////////////////
	//-- Logical --//
	/////////////////

	template<class Derived, class EL, class ER>
	class LogicalStatement : public BinaryPredicate<Derived, EL, ER>
	{
	public:
		using Base = BinaryPredicate<Derived, EL, ER>;

	protected:
		LogicalStatement(const EL& el, const ER& er) :
			Base(el, er)
		{}
	};

	/////////////////////
	//-- Logical and --//
	/////////////////////

	template<class EL, class ER>
	class AndStatement : public LogicalStatement<AndStatement<EL, ER>, EL, ER>
	{
	public:
		using Base = LogicalStatement<AndStatement<EL, ER>, EL, ER>;

		static const std::string& OperatorName()
		{
			static std::string name(" and ");
			return name;
		}

	private:
		AndStatement(const EL& el, const ER& er) :
			Base(el, er)
		{}

		template<class _EL, class _ER>
		friend AndStatement<_EL, _ER> operator&&(const Predicate<_EL>&, const Predicate<_ER>&);
	};

	template<class _EL, class _ER>
	AndStatement<_EL, _ER> operator&&(const Predicate<_EL>& el, const Predicate<_ER>& er)
	{
		return AndStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
	}

	////////////////////
	//-- Logical or --//
	////////////////////

	template<class EL, class ER>
	class OrStatement : public LogicalStatement<OrStatement<EL, ER>, EL, ER>
	{
	public:
		using Base = LogicalStatement<OrStatement<EL, ER>, EL, ER>;

		static const std::string& OperatorName()
		{
			static std::string name(" or ");
			return name;
		}

	private:
		OrStatement(const EL& el, const ER& er) :
			Base(el, er)
		{}

		template<class _EL, class _ER>
		friend OrStatement<_EL, _ER> operator||(const Predicate<_EL>&, const Predicate<_ER>&);
	};

	template<class _EL, class _ER>
	OrStatement<_EL, _ER> operator||(const Predicate<_EL>& el, const Predicate<_ER>& er)
	{
		return OrStatement<_EL, _ER>(static_cast<const _EL&>(el), static_cast<const _ER&>(er));
	}
#pragma endregion

#pragma endregion
}