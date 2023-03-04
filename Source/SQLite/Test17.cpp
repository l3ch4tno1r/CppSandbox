#include <iostream>

#define SEPARATOR(X) std::cout << "---------- " << X << " ----------\n"

#include "Utilities/Source/NameOfType.h"
#include "Utilities/Experimental/LiteralStringGenerator.h"

#include <Utilities/External/sqlite3/sqlite3.h>

namespace Test01
{
	template<typename _Type>
	struct GeneratorFor
	{
		inline static constexpr std::string_view sv = _Type::GetStrView();

		static constexpr size_t Size() { return sv.size(); }

		static constexpr char Value(size_t Idx) { return sv[Idx]; }
	};

	///////////////
	//-- CType --//
	///////////////

	struct CType
	{
		static constexpr std::string_view GetStrView() { return { "C" }; }

		using Generator = GeneratorFor<CType>;
	};

	constexpr CType C;

	template<typename _LHS>
	struct CExpr
	{
		using Generator = LCN::ConcatLiterals<CType::Generator, typename _LHS::Generator>;
	};

	template<typename _LHS>
	constexpr
	CExpr<_LHS>
	operator|(
		const _LHS&,
		const CType&)
	{
		return {};
	}

	///////////////
	//-- BType --//
	///////////////

	struct BType
	{
		static constexpr std::string_view GetStrView() { return { "B" }; }

		using Generator = GeneratorFor<BType>;
	};

	constexpr BType B;

	template<typename _LHS>
	struct BExpr
	{
		//using Generator = LCN::ConcatLiterals<BType::Generator, typename _LHS::Generator>;
		using Generator = LCN::ConcatLiterals<typename _LHS::Generator, BType::Generator>;
	};

	template<typename _LHS>
	constexpr
	BExpr<_LHS>
	operator|(
		const _LHS&,
		const BType&)
	{
		return {};
	}

	///////////////
	//-- AType --//
	///////////////

	struct AType
	{
		static constexpr std::string_view GetStrView() { return { "A" }; }

		using Generator = GeneratorFor<AType>;
	};

	constexpr AType A;

	template<typename _LHS>
	struct AExpr
	{};
}

namespace SQLite
{
	namespace Private
	{
		struct FromType
		{
			static constexpr std::string_view GetStrView() { return { "select * from " }; }

			using Generator = Test01::GeneratorFor<FromType>;
		};

		struct CommaSeparatorType
		{
			static constexpr std::string_view GetStrView() { return { ", " }; }

			using Generator = Test01::GeneratorFor<CommaSeparatorType>;
		};

		template<typename Entity>
		struct NameOfTable
		{
			static constexpr std::string_view GetStrView()
			{
				auto pos = LCN::NameOf<Entity>.find("::");

				return LCN::NameOf<Entity>.substr(pos + 2);
			}

			using Generator = Test01::GeneratorFor<NameOfTable>;
		};
	}

	template<typename _Entity>
	struct Table;

	template<typename ... _Entities>
	struct TablePack
	{
		using Generator = LCN::ConcatLiterals<
			Private::FromType::Generator,
			LCN::JoinLiteral<
				Private::CommaSeparatorType::Generator,
				LCN::TypeList<typename Private::NameOfTable<_Entities>::Generator...>>>;

		static inline constexpr auto View = LCN::Literal<Generator>;
	};

	template<typename ... _Entities>
	auto From(const Table<_Entities>& ...)
	{
		return TablePack<_Entities...>{};
	}
}

namespace Impl
{
	struct Contact{};

	struct PhoneNumber{};
}

namespace SQLite
{
	template<>
	struct Table<Impl::Contact> {};

	constexpr Table<Impl::Contact> Contacts;

	template<>
	struct Table<Impl::PhoneNumber> {};

	constexpr Table<Impl::PhoneNumber> PhoneNumbers;
}

template<typename _Callable>
void
GetSQLiteStuff(
	const std::string_view query,
	_Callable callable)
{
	sqlite3* _db;

	if (SQLITE_OK != sqlite3_open("E:/SQLite3/Directory.db", &_db))
		throw std::exception{ sqlite3_errmsg(_db) };

	std::shared_ptr<sqlite3> db(_db, [](sqlite3* ptr) { sqlite3_close(ptr); });

	char* errmsg{ nullptr };

	sqlite3_exec(
		db.get(),
		query.data(),
		callable,
		nullptr,
		&errmsg);

	/*
	if (errmsg)
		throw std::exception{ errmsg };
	*/
}

int main()
{
	SEPARATOR("Test 01");
	{
		using namespace Test01;

		// Type = CExpr<BExpr<AType>>
		constexpr auto expr = A | B | C;
		std::cout << LCN::Literal<decltype(expr)::Generator> << std::endl;
	}

	SEPARATOR("Test 02");
	{
		try
		{
			sqlite3* _db;

			if (SQLITE_OK != sqlite3_open("E:/SQLite3/Directory.db", &_db))
				throw std::exception{ sqlite3_errmsg(_db) };

			std::shared_ptr<sqlite3> db(_db, [](sqlite3* ptr) { sqlite3_close(ptr); });

			const char* query =
				"select c.FirstName, c.LastName from Contact c\n"
				"where c.FirstName like \"A%\"";

			char* errmsg{ nullptr };

			sqlite3_exec(
				db.get(),
				query,
				[](void* a_param, int argc, char** argv, char** column) -> int
				{
					for (int i = 0; i < argc; ++i)
						std::cout << argv[i] << ' ';

					std::cout << '\n';

					return 0;
				},
				nullptr,
				&errmsg);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Exception : " << e.what() << std::endl;
		}
	}

	SEPARATOR("Test Impl");
	{
		try
		{
			using namespace SQLite;
			auto query = From(Contacts);

			std::cout << query.View << std::endl;

			GetSQLiteStuff(
				query.View,
				[](void* a_param, int argc, char** argv, char** column) -> int
				{
					for (int i = 0; i < argc; ++i)
						std::cout << (argv[i] ? argv[i] : "N/A") << ' ';

					std::cout << '\n';

					return 0;
				});
		}
		catch (const std::exception& e)
		{
			std::cerr << "Exception : " << e.what() << std::endl;
		}
	}

	std::cin.get();
}