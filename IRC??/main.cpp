# include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
		return (error("Usage: ./ircserv <port> <password>", 1));
	Server server(argv[1], argv[2]);
	while (1)
	{
		server.processClients();
	}
	return (0);
}
