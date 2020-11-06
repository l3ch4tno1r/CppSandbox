#include <iostream>

class Logger
{
public:
	~Logger()
	{
		std::cout << std::endl;

		m_IsDead = true;
	}

	template<typename T>
	Logger& Print(const T& data)
	{
		if (m_IsDead)
			throw std::exception("Is ded !");

		std::cout << data;

		return *this;
	}

private:
	Logger() = default;

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	friend Logger Log();

private:
	bool m_IsDead = false;
};

Logger Log()
{
	return Logger();
}

template<typename T>
Logger& operator<<(Logger& logger, const T& data)
{
	return logger.Print(data);;
}

template<typename T>
Logger& operator<<(Logger&& logger, const T& data)
{
	return logger.Print(data);
}

int main()
{
	try
	{
		Log() << 2 << " Hello world !";

		Log().Print(3).Print(" Hello world again !");

		Logger* log = &Log();

		log->Print("I must be dead !");
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}	

	std::cin.get();
}