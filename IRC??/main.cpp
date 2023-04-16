# include "Server.hpp"

int main(int argc, char **argv)
{
	/// @brief check if the number of arguments is correct ( port and password )
	/// @todo check if the port is a number
	if (argc != 3)
		return (error("Usage: ./ircserv <port> <password>", 1));

	/// @brief create a server object and start the server
	Server server(argv[1], argv[2]);
	while (1)
	{
		/// @brief check if there is a new client to accept
		server.processClients();
	}
	return (0);
}
