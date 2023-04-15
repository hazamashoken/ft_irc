# pragma once

# include <iostream>
# include <string>
# include <map>
# include <vector>
# include <cstring>
# include <cstdlib>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>

# include "utils.hpp"
# include "ANSI.hpp"

class Client;
class Server
{
	public:
		Server(const char *port, const char *pass);
		~Server();

		void initializeServer();
		bool acceptNewClient();
		void processClients();
		// void handleClient(Client *client);
		// Client* getClientByNickname(const std::string& nickname);
		// Channel* getChannelByName(const std::string& channelName);
		// void executeCommand(Client* client, const std::string& command, const std::vector<std::string>& args);

	private:
		int __server_fd;
		int __addrlen;
		struct sockaddr_in __server_addr;
		std::string __port;
		std::string __pass;
		std::vector<pollfd> __pfds;
		std::map<int, Client *> __clients;
		// std::map<std::string, Channel *> __channels;

};
