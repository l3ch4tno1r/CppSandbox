#include <iostream>
#include <iomanip>
#include <type_traits>
#include <chrono>
#include <ctime>
#include <cmath>
#include <thread>
#include <atomic>

#include <LCN_Math/Source/Utilities/UnitsDistance.h>
#include <LCN_Math/Source/Utilities/UnitsAngle.h>

using namespace std::chrono_literals;

using namespace LCN::Units::Literals;

int main()
{
	constexpr auto distance = 1km;
	constexpr auto newDist = LCN::Units::UnitCast<LCN::Units::CentiMeterLL>(distance);
	constexpr LCN::Units::MilliMeterLL milli = 10inch;

	constexpr auto test1 = 1km + 1hm;
	constexpr auto test2 = 1dm - 1cm;
	constexpr LCN::Units::MicroMeterLL test3 = 1inch;
	constexpr LCN::Units::MilliMeterLL test4 = 1ft;

	constexpr LCN::Units::InchLL height = 180cm;
	constexpr LCN::Units::FootLL feet = height;
	constexpr LCN::Units::InchLL inch = height - feet;

	std::cout << "I am " << feet.Count() << "ft " << inch.Count() << "\" tall !\n";

	constexpr LCN::Units::InchLL f2i = 1ft;
	constexpr LCN::Units::FootLL y2f = 1yd;
	constexpr LCN::Units::YardLL m2y = 1mi;

	constexpr LCN::Units::CentiMeterLL height2 = 6ft + 3inch;

	constexpr LCN::Units::KiloMeterLL andIwouldwalk = 500mi;

	constexpr auto test5 = 5ft + 11inch;
	constexpr LCN::Units::CentiMeterLL test6(test5);

	constexpr LCN::Units::RadianLD rad = 180.0deg + 3.14159rad;
	constexpr LCN::Units::DegreeLD deg(rad);

	constexpr LCN::Units::DegreeLD deg2 = 45.0deg + 30.0amin;
	constexpr auto test7 = 45.0deg + 30.0amin;

	constexpr LCN::Units::RadianLD rad2 = LCN::Units::UnitCast<LCN::Units::DegreeLD>(180deg);

	std::cout << "cos(30deg) = " << std::cos(60deg / 2) << std::endl;
	std::cout << "sin(30deg) = " << std::sin(60deg / 2) << std::endl;
	std::cout << "tan(30deg) = " << std::tan(60deg / 2) << std::endl;

	auto machin1 = 2 * 5m;
	auto machin2 = 3m * 5;

	std::cout << (machin1 += 2dam).Count() << '\n';
	std::cout << (machin2 -= 2dam).Count() << '\n';

	constexpr auto machin3 = 10m / 2;

	std::cout << (++(1km)).Count() << '\n';
	std::cout << ((1km)++).Count() << '\n';

	auto displayArcSecond = [](const LCN::Units::ArcSecondLL& value)
	{
		LCN::Units::ArcSecondLL temp = value;

		auto degrees = LCN::Units::UnitCast<LCN::Units::DegreeLL>(temp);
		auto minutes = LCN::Units::UnitCast<LCN::Units::ArcMinuteLL>(temp -= degrees);
		auto seconds = LCN::Units::UnitCast<LCN::Units::ArcSecondLL>(temp -= minutes);

		std::cout
			<< degrees.Count() << '°'
			<< std::setw(2) << std::setfill('0') << minutes.Count() << '\''
			<< std::setw(2) << std::setfill('0') << seconds.Count() << '\"';
	};

	displayArcSecond(1.0rad);          std::cout << '\n';
	displayArcSecond(1.0rnd / 4);      std::cout << '\n';
	displayArcSecond(400deg % 360deg); std::cout << '\n';
	displayArcSecond(29.51deg);        std::cout << '\n';

	constexpr LCN::Units::RoundLD round = 1080.0deg;

	std::cout << std::cos(0.25rnd) << ", " << std::sin(0.25rnd) << '\n';

	std::cin.get();
}