#include <iostream>
#include <iomanip>

enum class MonthEnum : uint16_t
{
	JAN, FEB, MAR,
	APR, MAY, JUN,
	JUL, AUG, SEP,
	OCT, NOV, DEC
};

struct DateTime
{
	uint32_t  Year;
	MonthEnum Month;
	uint16_t  Day;

	friend std::strong_ordering operator<=>(const DateTime&, const DateTime&) = default;
};

class ZBDateFormat
{
public:
	constexpr
	explicit
	ZBDateFormat(const DateTime& target)
		: m_Target{ target }
	{}

	friend std::ostream& operator<<(std::ostream&, ZBDateFormat);

private:
	const DateTime& m_Target;
};

std::ostream&
operator<<(
	std::ostream& os,
	ZBDateFormat date)
{
	static char monthNames[][4] = {
		"JAN", "FEB", "MAR",
		"APR", "MAY", "JUN",
		"JUL", "AUG", "SEP",
		"OCT", "NOV", "DEC"
	};

	const auto& target = date.m_Target;

	return os 
		<< target.Year << '-'
		<< monthNames[(uint16_t)target.Month] << '-'
		<< std::setw(2) << std::setfill('0') << target.Day;
}

class EUDateFormat
{
public:
	constexpr
	explicit
	EUDateFormat(const DateTime& target)
		: m_Target{ target }
	{}

	friend std::ostream& operator<<(std::ostream&, EUDateFormat);

private:
	const DateTime& m_Target;
};

std::ostream&
operator<<(
	std::ostream& os,
	EUDateFormat date)
{
	const auto& target{ date.m_Target };

	return os
		<< std::setw(2) << std::setfill('0') << target.Day << '/'
		<< std::setw(2) << std::setfill('0') << (uint16_t)target.Month + 1 << '/'
		<< target.Year;
}

int main()
{
	constexpr DateTime birthdayA{ 1987, MonthEnum::SEP, 26 };
	constexpr DateTime birthdayJ{ 1990, MonthEnum::JUN, 04 };
	constexpr DateTime birthdayM{ 1961, MonthEnum::JAN, 25 };
	constexpr DateTime birthdayP{ 1961, MonthEnum::JUL, 25 };

	static_assert(birthdayA < birthdayJ);
	static_assert(birthdayM < birthdayP);

	static_assert(DateTime{ 2022, MonthEnum::DEC, 31 } < DateTime{ 2023, MonthEnum::JAN, 01 });
	static_assert(DateTime{ 2023, MonthEnum::JAN, 01 } < DateTime{ 2023, MonthEnum::JAN, 02 });
	static_assert(DateTime{ 2023, MonthEnum::JAN, 01 } < DateTime{ 2023, MonthEnum::FEB, 01 });

	static_assert(birthdayA == DateTime{ 1987, MonthEnum::SEP, 26 });

	std::cout << ZBDateFormat{ birthdayA } << " - " << EUDateFormat{ birthdayA } << std::endl;
	std::cout << ZBDateFormat{ birthdayJ } << " - " << EUDateFormat{ birthdayJ } << std::endl;
	std::cout << ZBDateFormat{ birthdayM } << " - " << EUDateFormat{ birthdayM } << std::endl;
	std::cout << ZBDateFormat{ birthdayP } << " - " << EUDateFormat{ birthdayP } << std::endl;

	std::cin.get();
}