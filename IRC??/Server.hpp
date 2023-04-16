# pragma once

# include <iostream>
# include <ostream>
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

# include "utils/utils.hpp"
# include "utils/ANSI.hpp"

class Command;
class Client;
class Channel;
class Message;
class Server
{
	public:
		Server(const char *port, const char *pass);
		~Server();

		void initializeServer();
		void initializeCommandPallet();
		bool acceptNewClient();
		void processClients();
		void disconnectClient(Client *client);
		void handleClient(Client *client);
		Client* getClientByNickname(const std::string& nickname);
		Channel* getChannelByName(const std::string& channelName);
		void executeCommand(Client *client, std::string &message);

		static std::map<std::string, Channel *> __channels;

	private:
		int __server_fd;
		std::string __port;
		std::string __pass;
		struct sockaddr_in __server_addr;
		int __addrlen;
		std::vector<pollfd> __pfds;
		std::map<int, Client *> __clients;
		std::map<std::string, void (*)(Command *)> __commands;

		void debugSendToAllClients(const std::string& message);
};
