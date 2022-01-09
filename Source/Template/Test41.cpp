#include <iostream>
#include <vector>
#include <string>

#include <Source/Memory/_MemTracker.h>

#define SEPARATOR(X) std::cout << "------------- " << X << " -------------" << std::endl

namespace Naive
{
	struct Vector
	{
		std::vector<int> Data;
	};

	Vector operator+(const Vector& a, const Vector& b)
	{
		const auto& va = a.Data;
		const auto& vb = b.Data;

		if (va.size() != vb.size())
			throw std::exception("Ded");

		Vector result;
		auto& vr = result.Data;

		vr.reserve(va.size());

		for (size_t i = 0; i < va.size(); ++i)
			vr.emplace_back(va[i] + vb[i]);

		return result;
	}
}

namespace CRTP
{
	template<class _Derived>
	class Expression
	{
	public:
		const _Derived& Derived() const { return static_cast<const _Derived&>(*this); }
		      _Derived& Derived()       { return static_cast<      _Derived&>(*this); }

		inline size_t Size() const {return this->Derived().Size(); }

		inline int operator[](size_t idx) const { return this->Derived()[idx]; }
	};

	struct Vector : public Expression<Vector>
	{
		std::vector<int> Data;

		Vector(const std::initializer_list<int> list) :
			Data(list)
		{}

		Vector() = default;

		size_t Size() const { return Data.size(); }

		int operator[](size_t idx) const { return Data[idx]; }
	};

	template<class _EL, class _ER>
	class AddVec : public Expression<AddVec<_EL, _ER>>
	{
	public:
		size_t Size() const
		{
			if (m_EL.Size() != m_ER.Size())
				throw std::exception("Ded");

			return m_EL.Size();
		}

		int operator[](size_t idx) const { return m_EL[idx] + m_ER[idx]; }

		operator Vector() const
		{
			Vector result;
			auto& vr = result.Data;

			vr.reserve(this->Size());

			for (size_t i = 0; i < this->Size(); ++i)
				vr.emplace_back(m_EL[i] + m_ER[i]);

			return result;
		}

	private:
		AddVec(const _EL& el, const _ER& er) :
			m_EL{el},
			m_ER{er}
		{}

		template<class _OEL, class _OER>
		friend AddVec<_OEL, _OER> operator+(const Expression<_OEL>&, const Expression<_OER>&);

	private:
		const _EL& m_EL;
		const _ER& m_ER;
	};

	template<class _OEL, class _OER>
	AddVec<_OEL, _OER> operator+(const Expression<_OEL>& el, const Expression<_OER>& er)
	{
		return { el.Derived(), er.Derived() };
	}
}

int main()
{
	std::cout << R"--(///////////////)--" << '\n';
	std::cout << R"--(//-- Naive --//)--" << '\n';
	std::cout << R"--(///////////////)--" << '\n';
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		SEPARATOR("va");
		Naive::Vector va{ {1, 2, 3} };
		SEPARATOR("vb");
		Naive::Vector vb{ {3, 2, 1} };
		SEPARATOR("vc");
		Naive::Vector vc{ {1, 1, 1} };

		SEPARATOR("vr");
		auto vr = va + vb + vc + va;

		SEPARATOR("~");
	}

	std::cout << '\n';

	std::cout << R"--(//////////////)--" << '\n';
	std::cout << R"--(//-- CRTP --//)--" << '\n';
	std::cout << R"--(//////////////)--" << '\n';
	{
		auto session = MemTracker::Get().BeginScopeBasedSession();

		SEPARATOR("va");
		CRTP::Vector va{ {1, 2, 3} };
		SEPARATOR("vb");
		CRTP::Vector vb{ {3, 2, 1} };
		SEPARATOR("vc");
		CRTP::Vector vc{ {1, 1, 1} };

		SEPARATOR("vr");
		CRTP::Vector vr = va + vb + vc + va;

		SEPARATOR("~");
	}

	std::cin.get();
}