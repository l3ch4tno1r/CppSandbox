#include <filesystem>
#include <iostream>
#include <vector>
#include <map>
#include <ranges>
#include <sstream>
#include <optional>

namespace fs = std::filesystem;

class FileSystemBrowser
{
private:
	enum class Commands : uint8_t
	{
		Exit,
		Goto,
		Select
	};

public:
	std::optional<fs::path>
	operator()(
		fs::path path = fs::current_path())
	{
		std::string buffer;

		static const std::map<std::string, Commands> commands = {
			{ "exit",   Commands::Exit },
			{ "cancel", Commands::Exit },
			{ "goto",   Commands::Goto },
			{ "g",      Commands::Goto },
			{ "select", Commands::Select },
			{ "s",      Commands::Select },
		};

		while (true)
		{
			if (fs::is_regular_file(path))
				return path;

			std::cout << path << std::endl;

			_Populate(path);
			_Display();

			if (!std::getline(std::cin, buffer))
				break;

			std::istringstream sstr{ buffer };

			std::string cmd;
			sstr >> cmd;

			auto it = commands.find(cmd);

			if (it == commands.end())
				continue;

			if (it->second == Commands::Exit)
				return std::nullopt;

			switch (it->second)
			{
			case Commands::Goto:
			{
				unsigned idx;
				
				if (!(sstr >> idx))
					break;

				if (idx > m_dirContent.size())
					break;

				path = idx == 0
					? path.parent_path()
					: path / m_dirContent[idx - 1];

				break;
			}
			default:
				break;
			}
		}

		return path;
	}

private:
	void
	_Populate(
		const fs::path& parent)
	{
		m_dirContent.clear();

		std::copy(
			fs::directory_iterator{ parent },
			fs::directory_iterator{},
			std::back_inserter(m_dirContent));

		std::ranges::stable_partition(
			m_dirContent,
			[](const fs::path& path)
			{
				return fs::is_directory(path);
			});
	}

	void _Display()
	{
		size_t numDigit = 0;
		for (size_t i = m_dirContent.size(); i != 0; i /= 10, ++numDigit);

		for (size_t i = 0; i < m_dirContent.size(); ++i)
		{
			const auto& path{ m_dirContent[i] };

			std::cout 
				<< std::setw(numDigit) << i + 1
				<< (fs::is_directory(path) ? " > " : "   ") << path.filename() << std::endl;
		}
	}

private:
	std::vector<fs::path> m_dirContent;
};

int main()
{
	FileSystemBrowser browser;

	std::cout << "Selected path : " << browser().value_or("None") << std::endl;

	std::cin.get();
}