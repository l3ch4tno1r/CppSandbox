#include <iostream>
#include <iomanip>
#include <type_traits>
#include <chrono>
#include <ctime>
#include <ratio>
#include <cmath>

using namespace std::chrono_literals;

namespace Measurement
{
	template<typename _To, typename _From>
	constexpr _To MeasureCast(const _From&);

	template<
		typename _MeasurementType,
		typename _Type,
		typename _Ratio2,
		std::enable_if_t<std::is_arithmetic_v<_Type>, bool> = true>
	class Measure
	{
	public:
		using Type    = _MeasurementType;
		using ValType = _Type;
		using Ratio   = _Ratio2;

	public:
		constexpr Measure() = default;

		template<typename _Type2>
		constexpr Measure(const _Type2& value) :
			m_Measure{ static_cast<_Type>(value) }
		{}

		template<typename _Type2, typename _Ratio2>
		constexpr Measure(const Measure<_MeasurementType, _Type2, _Ratio2>& other) :
			m_Measure{ MeasureCast<Measure>(other).Count() }
		{}

		constexpr _Type Count() const { return m_Measure; }

		constexpr Measure operator+() const { return Measure(*this); }

		constexpr Measure operator-() const { return Measure(-m_Measure); }

		constexpr Measure& operator++()
		{
			++m_Measure;
			return *this;
		}

		constexpr Measure operator++(int) { return Measure(m_Measure++); }

		constexpr Measure& operator--()
		{
			--m_Measure;
			return *this;
		}

		constexpr Measure operator--(int) { return Measure(m_Measure--); }

		constexpr Measure& operator+=(const Measure& other)
		{
			m_Measure += other.m_Measure;
			return *this;
		}

		constexpr Measure& operator-=(const Measure& other)
		{
			m_Measure -= other.m_Measure;
			return *this;
		}

	private:
		_Type m_Measure{ _Type(0) };
	};

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2,
		typename _Ratio2>
	struct std::common_type<
		Measure<_MeasurementType, _Type1, _Ratio1>,
		Measure<_MeasurementType, _Type2, _Ratio2>>
	{
		using type = Measurement::Measure<
			_MeasurementType,
			std::common_type_t<_Type1, _Type2>,
			std::ratio<
				std::_Gcd<_Ratio1::num, _Ratio2::num>::value,
				std::_Lcm<_Ratio1::den, _Ratio2::den>::value>>;
	};

	template<
		typename _To,
		typename _MeasurementType,
		typename _Type,
		typename _Ratio2>
	constexpr
	std::enable_if_t<std::is_same_v<_MeasurementType, typename _To::Type>, _To>
	MeasureCast(
		const Measure<_MeasurementType, _Type, _Ratio2>& distance)
	{
		using FromRatio = _Ratio2;
		using ToRatio   = typename _To::Ratio;

		using NewRatio = std::ratio_divide<FromRatio, ToRatio>;

		return _To{ NewRatio::num * distance.Count() / NewRatio::den };
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2,
		typename _Ratio2>
	constexpr
	std::common_type_t<Measure<_MeasurementType, _Type1, _Ratio1>, Measure<_MeasurementType, _Type2, _Ratio2>>
	operator+(
		const Measure<_MeasurementType, _Type1, _Ratio1>& measure1,
		const Measure<_MeasurementType, _Type2, _Ratio2>& measure2)
	{
		using ReturnType = typename std::common_type<Measure<_MeasurementType, _Type1, _Ratio1>, Measure<_MeasurementType, _Type2, _Ratio2>>::type;

		return ReturnType(ReturnType(measure1).Count() + ReturnType(measure2).Count());
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2,
		typename _Ratio2>
	constexpr
	std::common_type_t<Measure<_MeasurementType, _Type1, _Ratio1>, Measure<_MeasurementType, _Type2, _Ratio2>>
	operator-(
		const Measure<_MeasurementType, _Type1, _Ratio1>& measure1,
		const Measure<_MeasurementType, _Type2, _Ratio2>& measure2)
	{
		using ReturnType = typename std::common_type<Measure<_MeasurementType, _Type1, _Ratio1>, Measure<_MeasurementType, _Type2, _Ratio2>>::type;

		return ReturnType(ReturnType(measure1).Count() - ReturnType(measure2).Count());
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Type2,
		typename _Ratio2>
	constexpr
	Measure<_MeasurementType, std::common_type_t<_Type1, _Type2>, _Ratio2>
	operator*(
		const _Type1& factor,
		const Measure<_MeasurementType, _Type2, _Ratio2>& measure)
	{
		using CommonType = std::common_type_t<_Type1, _Type2>;
		using ReturnType = Measure<_MeasurementType, CommonType, _Ratio2>;

		return ReturnType(factor * ReturnType(measure).Count());
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2>
	inline
	constexpr
	Measure<_MeasurementType, std::common_type_t<_Type1, _Type2>, _Ratio1>
	operator*(
		const Measure<_MeasurementType, _Type1, _Ratio1>& measure,
		const _Type2& factor)
	{
		return factor * measure;
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2>
	constexpr
	Measure<_MeasurementType, std::common_type_t<_Type1, _Type2>, _Ratio1>
	operator/(
		const Measure<_MeasurementType, _Type1, _Ratio1>& measure,
		const _Type2& factor)
	{
		using CommonType = std::common_type_t<_Type1, _Type2>;
		using ReturnType = Measure<_MeasurementType, CommonType, _Ratio1>;

		return ReturnType(ReturnType(measure).Count() / factor);
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2>
	constexpr
	Measure<_MeasurementType, std::common_type_t<_Type1, _Type2>, _Ratio1>
	operator%(
		const Measure<_MeasurementType, _Type1, _Ratio1>& measure,
		const _Type2& divider)
	{
		using CommonType = std::common_type_t<_Type1, _Type2>;
		using ReturnType = Measure<_MeasurementType, CommonType, _Ratio1>;

		return ReturnType(ReturnType(measure).Count() % divider);
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2,
		typename _Ratio2>
	constexpr
	std::common_type_t<
		Measure<_MeasurementType, _Type1, _Ratio1>,
		Measure<_MeasurementType, _Type2, _Ratio2>>
	operator%(
		const Measure<_MeasurementType, _Type1, _Ratio1>& measure1,
		const Measure<_MeasurementType, _Type2, _Ratio2>& measure2)
	{
		using ReturnType = std::common_type_t<
			Measure<_MeasurementType, _Type1, _Ratio1>,
			Measure<_MeasurementType, _Type2, _Ratio2>>;

		return ReturnType(ReturnType(measure1).Count() % ReturnType(measure2).Count());
	}

	class Distance;

	using KiloMeter  = Measure<Distance, long long, std::kilo>;
	using HectoMeter = Measure<Distance, long long, std::hecto>;
	using DecaMeter  = Measure<Distance, long long, std::deca>;
	using Meter      = Measure<Distance, long long, std::ratio<1>>;
	using DeciMeter  = Measure<Distance, long long, std::deci>;
	using CentiMeter = Measure<Distance, long long, std::centi>;
	using MilliMeter = Measure<Distance, long long, std::milli>;
	using MicroMeter = Measure<Distance, long long, std::micro>;

	using Inch = Measure<Distance, long long, std::ratio<254, 10000>>;
	using Foot = Measure<Distance, long long, std::ratio<3048, 10000>>;
	using Yard = Measure<Distance, long long, std::ratio<9144, 10000>>;
	using Mile = Measure<Distance, long long, std::ratio<160934, 100>>;

	class Angle;

	using Radian    = Measure<Angle, float, std::ratio<18000000, 314159>>;
	using Degree    = Measure<Angle, float, std::ratio<1>>;
	using ArcMinute = Measure<Angle, float, std::ratio<1, 60>>;
	using ArcSecond = Measure<Angle, float, std::ratio<1, 3600>>;

	using Degreei    = Measure<Angle, int, std::ratio<1>>;
	using ArcMinutei = Measure<Angle, int, std::ratio<1, 60>>;
	using ArcSecondi = Measure<Angle, int, std::ratio<1, 3600>>;

	namespace Literals
	{
		constexpr KiloMeter operator"" km(unsigned long long value) noexcept { return KiloMeter{ value }; }

		constexpr HectoMeter operator"" hm(unsigned long long value) noexcept { return HectoMeter{ value }; }

		constexpr DecaMeter operator"" dam(unsigned long long value) noexcept { return DecaMeter{ value }; }

		constexpr Meter operator"" m(unsigned long long value) noexcept { return Meter{ value }; }

		constexpr DeciMeter operator"" dm(unsigned long long value) noexcept { return DeciMeter{ value }; }

		constexpr CentiMeter operator"" cm(unsigned long long value) noexcept { return CentiMeter{ value }; }

		constexpr MilliMeter operator"" mm(unsigned long long value) noexcept { return MilliMeter{ value }; }

		constexpr MicroMeter operator"" um(unsigned long long value) noexcept { return MicroMeter{ value }; }


		constexpr Inch operator"" inch(unsigned long long value) noexcept { return Inch{ value }; }

		constexpr Foot operator"" ft(unsigned long long value) noexcept { return Foot{ value }; }

		constexpr Yard operator"" yd(unsigned long long value) noexcept { return Yard{ value }; }

		constexpr Mile operator"" mi(unsigned long long value) noexcept { return Mile{ value }; }


		constexpr Radian operator"" rad(long double value) noexcept { return Radian{ value }; }

		constexpr Degree operator"" deg(long double value) noexcept { return Degree{ value }; }

		constexpr Degreei operator"" deg(unsigned long long value) noexcept { return Degreei{ value }; }

		constexpr ArcMinute operator"" amin(long double value) noexcept { return ArcMinute{ value }; }

		constexpr ArcSecond operator"" asec(long double value) noexcept { return ArcSecond{ value }; }
	}
}

namespace std
{
	template<
		typename _Type,
		typename _Ratio>
	constexpr _Type cos(const Measurement::Measure<Measurement::Angle, _Type, _Ratio>& angle)
	{
		using Rad = Measurement::Measure<Measurement::Angle, _Type, std::ratio<18000000, 314159>>;

		return std::cos(Measurement::MeasureCast<Rad>(angle).Count());
	}

	template<
		typename _Type,
		typename _Ratio>
		constexpr _Type sin(const Measurement::Measure<Measurement::Angle, _Type, _Ratio>& angle)
	{
		using Rad = Measurement::Measure<Measurement::Angle, _Type, std::ratio<18000000, 314159>>;

		return std::sin(Measurement::MeasureCast<Rad>(angle).Count());
	}

	template<
		typename _Type,
		typename _Ratio>
		constexpr _Type tan(const Measurement::Measure<Measurement::Angle, _Type, _Ratio>& angle)
	{
		using Rad = Measurement::Measure<Measurement::Angle, _Type, std::ratio<18000000, 314159>>;

		return std::tan(Measurement::MeasureCast<Rad>(angle).Count());
	}
}

using namespace Measurement::Literals;

int main()
{
	constexpr auto distance = 1km;
	constexpr auto newDist = Measurement::MeasureCast<Measurement::CentiMeter>(distance);
	constexpr Measurement::MilliMeter milli = 10inch;

	constexpr auto test1 = 1km + 1hm;
	constexpr auto test2 = 1dm - 1cm;
	constexpr Measurement::MicroMeter test3 = 1inch;
	constexpr Measurement::MilliMeter test4 = 1ft;

	constexpr Measurement::Inch height = 180cm;
	constexpr Measurement::Foot feet = height;
	constexpr Measurement::Inch inch = height - feet;

	std::cout << "I am " << feet.Count() << "ft " << inch.Count() << "\" tall !\n";

	constexpr Measurement::Inch f2i = 1ft;
	constexpr Measurement::Foot y2f = 1yd;
	constexpr Measurement::Yard m2y = 1mi;

	constexpr Measurement::CentiMeter height2 = 6ft + 3inch;

	constexpr Measurement::KiloMeter andIwouldwalk = 500mi;

	constexpr auto test5 = 5ft + 11inch;
	constexpr Measurement::CentiMeter test6(test5);

	constexpr Measurement::Radian rad = 180.0deg + 3.14159rad;
	constexpr Measurement::Degree deg(rad);

	constexpr Measurement::Degree deg2 = 45.0deg + 30.0amin;
	constexpr auto test7 = 45.0deg + 30.0amin;

	std::cout << "cos(30deg) = " << std::cos(60.0deg / 2) << std::endl;
	std::cout << "sin(30deg) = " << std::sin(60.0deg / 2) << std::endl;
	std::cout << "tan(30deg) = " << std::tan(60.0deg / 2) << std::endl;

	auto machin1 = 2 * 5m;
	auto machin2 = 3m * 5;

	std::cout << (machin1 += 2dam).Count() << '\n';
	std::cout << (machin2 -= 2dam).Count() << '\n';

	constexpr auto machin3 = 10m / 2;

	std::cout << (++(1km)).Count() << '\n';
	std::cout << ((1km)++).Count() << '\n';

	auto displayArcSecond = [](const Measurement::ArcSecondi& value)
	{
		Measurement::ArcSecondi temp = value;

		auto degrees = Measurement::MeasureCast<Measurement::Degreei>(temp);
		auto minutes = Measurement::MeasureCast<Measurement::ArcMinutei>(temp -= degrees);
		auto seconds = Measurement::MeasureCast<Measurement::ArcSecondi>(temp -= minutes);

		std::cout
			<< degrees.Count() << '°'
			<< std::setw(2) << std::setfill('0') << minutes.Count() << '\''
			<< std::setw(2) << std::setfill('0') << seconds.Count() << "\"\n";
	};

	displayArcSecond(1.0rad);
	displayArcSecond(400deg % 360deg);
	displayArcSecond(29.51deg);

	std::cin.get();
}