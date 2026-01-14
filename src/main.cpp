#include "IRC.hpp"
#include <csignal>
#include <cstdlib>

Server* g_server = NULL;

void signalHandler(int signum)
{
	const char* msg = "\nSignal received, shutting down server...\n";
	write(STDOUT_FILENO, msg, 45);
	
	if (g_server != NULL)
		g_server->stop();
	
	(void)signum;
}

void setupSignalHandlers()
{
	struct sigaction sa;
	
	// Configuration for SIGINT and SIGTERM
	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		std::cerr << "Error: Failed to set SIGINT handler" << std::endl;
		exit(1);
	}
	
	if (sigaction(SIGTERM, &sa, NULL) == -1)
	{
		std::cerr << "Error: Failed to set SIGTERM handler" << std::endl;
		exit(1);
	}
	
	signal(SIGPIPE, SIG_IGN);
}

bool validateArguments(int argc, char** argv)
{
	// Check the number of arguments
	if (argc != 3)
	{
		std::cerr << "Error: Invalid number of arguments" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		std::cerr << "Example: " << argv[0] << " 6667 mypassword" << std::endl;
		return false;
	}
	
	// Check that the port is a valid number
	std::string portStr = argv[1];
	for (size_t i = 0; i < portStr.length(); i++)
	{
		if (!std::isdigit(portStr[i]))
		{
			std::cerr << "Error: Port must be a number" << std::endl;
			return false;
		}
	}
	
	// Convert and validate the port
	long port = std::atol(argv[1]);
	
	if (port < 1 || port > 65535)
	{
		std::cerr << "Error: Port must be between 1 and 65535" << std::endl;
		return false;
	}
	
	if (port < 1024)
	{
		std::cerr << "Warning: Ports below 1024 require root privileges" << std::endl;
	}
	
	// Check that the password is not empty
	std::string password = argv[2];
	if (password.empty())
	{
		std::cerr << "Error: Password cannot be empty" << std::endl;
		return false;
	}
	
	return true;
}

int main(int argc, char** argv)
{
	// 1. Validate the arguments
	if (!validateArguments(argc, argv))
		return 1;
	
	// 2. Parse the arguments
	int port = std::atoi(argv[1]);
	std::string password = argv[2];
	
	// 3. Setup signal handlers
	setupSignalHandlers();
	
	// 4. Create the server
	Server server(port, password);
	g_server = &server;  // For the signal handler
	
	// 5. Initialize the server
	if (!server.init())
	{
		std::cerr << "Failed to initialize server" << std::endl;
		g_server = NULL;
		return 1;
	}
	
	// 6. Display a startup message
	std::cout << "========================================" << std::endl;
	std::cout << "ft_irc server started successfully!" << std::endl;
	std::cout << "Port: " << port << std::endl;
	std::cout << "Server name: " << server.getServerName() << std::endl;
	std::cout << "Press Ctrl+C to stop the server" << std::endl;
	std::cout << "========================================" << std::endl;
	
	// 7. Start the main loop
	server.run();  // Blocks until the server stops
	
	// 8. Clean up and exit
	std::cout << "\n========================================" << std::endl;
	std::cout << "Server stopped cleanly" << std::endl;
	std::cout << "========================================" << std::endl;
	
	g_server = NULL;
	
	return 0;
}