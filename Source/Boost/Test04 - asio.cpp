#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

namespace basio = boost::asio;

std::string buffer(1024 * 20, 0);

void AsyncGrabData(basio::ip::tcp::socket& socket)
{
	socket.async_read_some(
		basio::buffer(buffer.data(), buffer.size()),
		[&](std::error_code ec, std::size_t length)
		{
			if (ec)
				return;

			std::cout << buffer;

			AsyncGrabData(socket);
		});
}

int main()
{
	try
	{
		basio::io_context context;

		//auto ipAddress = basio::ip::make_address("93.184.216.34");
		auto ipAddress = basio::ip::make_address("51.38.81.49");

		basio::ip::tcp::endpoint endpoint{ipAddress , 80 };
		basio::ip::tcp::socket socket{ context };

		socket.connect(endpoint);

		std::cout << "Connected !" << std::endl;

		if (!socket.is_open())
			throw std::exception{ "Socket is not open !" };

		std::string request =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";

		socket.write_some(basio::buffer(request.data(), request.size()));
		socket.wait(basio::socket_base::wait_type::wait_read);

		size_t bytes = socket.available();

		if (!bytes)
			throw std::exception{ "No byte available !" };

		std::string response(bytes, 0);

		socket.read_some(basio::buffer(response.data(), response.size()));

		std::cout << "Response : " << response << std::endl;
	}
	catch (const boost::system::system_error& se)
	{
		std::cerr << "Boost Exception : " << se.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception : " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Unknown error caught..." << std::endl;
	}

	std::cin.get();
}