# include "Server.hpp"
# include "Client.hpp"
# include "Channel.hpp"

std::map<std::string, Channel *> Server::__channels;

Server::Server(const char *port, const char *pass)
: __port(std::string(port)), __pass(std::string(pass)), __addrlen(sizeof(__server_addr))
{
	initializeServer();
}

Server::~Server()
{
	{
		std::map<int, Client *>::iterator it = __clients.begin();
		while (it != __clients.end())
		{
			delete it->second;
			++it;
		}
	}
	{
		std::map<std::string, Channel *>::iterator it = __channels.begin();
		while (it != __channels.end())
		{
			delete it->second;
			++it;
		}
	}
	close(__server_fd);
}

void Server::initializeServer()
{
	if ((__server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		exit(error("socket() failed", 1));

	int enable = 1;

	if (setsockopt(__server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0)
		exit(error("setsockopt(SO_REUSEADDR) failed", 1));
	if (fcntl(__server_fd, F_SETFL, O_NONBLOCK) < 0)
		exit(error("fcntl() failed", 1));

	__server_addr.sin_family = AF_INET;
	__server_addr.sin_addr.s_addr = INADDR_ANY;
	std::cout << "port: " << __port << std::endl;
	__server_addr.sin_port = htons(atoi(__port.c_str()));

	std::cout << "port: " << __server_addr.sin_port << std::endl;
	if (bind(__server_fd, (struct sockaddr *)&__server_addr, __addrlen) < 0)
		exit(error("bind() failed", 1));
	if (listen(__server_fd, __server_addr.sin_port) < 0)
		exit(error("listen() failed", 1));

	__pfds.push_back((pollfd){__server_fd, POLLIN, 0});

}

bool Server::acceptNewClient()
{
	int new_client_fd = 0;
	sockaddr_in new_client_addr;
	socklen_t new_client_addr_size = sizeof(new_client_addr);

	new_client_fd = accept(__server_fd, (struct sockaddr *)&new_client_addr, &new_client_addr_size);
	if (new_client_fd == -1)
		return(error("accept() failed", 1));
	__clients.insert(std::pair<int, Client *>(new_client_fd, new Client(new_client_fd, new_client_addr)));
	if (DEBUG)
	{
		std::stringstream ss;
		ss <<  new_client_fd;
		std::string str = ss.str();
		debug(std::string("new client: ") + str);
	}
	__pfds.push_back((pollfd){new_client_fd, POLLIN | POLLHUP, 0});
	return (0);
}

void Server::processClients()
{
	int ret = poll(&__pfds[0], __pfds.size(), -1);
	if (ret < 0) {
		perror("poll failed");
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < __pfds.size(); ++i) {
		// if (DEBUG)
		// {
		// 	std::ostringstream o;
		// 	o << "__pfds: " << __pfds[i].fd << " " << __pfds[i].revents;
		// 	debug(o);
		// }
		if (__pfds[i].revents & POLLIN) {
			if (__pfds[i].fd == __server_fd) {
				if (acceptNewClient())
					error("acceptNewClient() failed");
			} else {
				debug("client detected");
				if (__clients[__pfds[i].fd]->isAliveClient() == false)
				{
					debug("client closed");
					disconnectClient(__clients[__pfds[i].fd]);
					close(__pfds[i].fd);
					__pfds.erase(__pfds.begin() + i);
					continue ;
				}
				debug("client handled");
				handleClient(__clients[__pfds[i].fd]);
			}
		} else if (__pfds[i].revents & POLLHUP) {
			// Connection closed
			debug("client closed");
			disconnectClient(__clients[__pfds[i].fd]);
			close(__pfds[i].fd);
			__pfds.erase(__pfds.begin() + i);
			break ;
		}
	}
}

void Server::handleClient(Client *client)
{
	std::string command;
	std::vector<std::string> args;
	std::string line;

	if (client->getReadBuffer().empty())
	{
		debug("empty read buffer");
		return ;
	}
	line = client->getReadBuffer();
	debug("line: " + line);
	if (line.find("\r\n") == std::string::npos)
		return ;
	client->setReadBuffer(line.substr(line.find("\r\n") + 2, line.size()));
	while (line.find("\r\n") != std::string::npos)
	{
		debug("line: " + line);
		line = line.substr(line.find("\r\n") + 2, line.size());
		command = line.substr(0, line.find(" "));
		args = ft_split(line.substr(line.find(" ") + 1, line.size()), " ");
		debug("command: " + command);
		debug("args: " + args[0]);
		executeCommand(client, command, args);
	}

	// debugSendToAllClients(line);

}

Client* Server::getClientByNickname(const std::string& nickname)
{
	std::map<int, Client *>::iterator it = __clients.begin();
	while (it != __clients.end())
	{
		if (it->second->getNickname() == nickname)
			return (it->second);
		++it;
	}
	return (NULL);
}

Channel* Server::getChannelByName(const std::string& channelName)
{
	std::map<std::string, Channel *>::iterator it = __channels.begin();
	while (it != __channels.end())
	{
		if (it->second->getName() == channelName)
			return (it->second);
		++it;
	}
	return (NULL);
}

void Server::executeCommand(Client* client, const std::string& command, const std::vector<std::string>& args)
{
	// (void)client;
	// (void)command;
	// (void)args;
	if (command == "PING")
	{
		debug("cmd: PING");
		client->sendReply("PONG", args[0]);
	} else if (command == "USER")
	{
		debug("cmd: USER");
		client->setUsername(args[0]);
		debug("cmd: USER 1" + args[0]);
		client->setHostname(args[1]);
		debug("cmd: USER 2" + args[1]);
		client->setServername(args[2]);
		debug("cmd: USER 3" + args[2]);
		client->setRealname(args[3]);
		debug("cmd: USER 4" + args[3]);
		client->sendReply("001", client->getNickname() + " :Welcome to the Internet Relay Network " + client->getNickname());
		client->sendReply("002", client->getNickname() + " :Your host is " + client->getServername());
	} else if (command == "NICK")
	{
		debug("cmd: NICK");
		client->setNickname(args[0]);
	} else if (command == "JOIN")
	{
		debug("cmd: JOIN");
		std::string channelName = args[0];
		if (channelName[0] != '#')
			return ;
		Channel *channel = getChannelByName(channelName);
		if (channel == NULL)
		{
			channel = new Channel(channelName);
			__channels.insert(std::pair<std::string, Channel *>(channelName, channel));
		}
		channel->addClient(client);
	} else if (command == "PRIVMSG")
	{
		debug("cmd: PRIVMSG");
		std::string channelName = args[0];
		if (channelName[0] != '#')
			return ;
		Channel *channel = getChannelByName(channelName);
		if (channel == NULL)
			return ;
		channel->sendToAllClients(client->getNickname() + " PRIVMSG " + channelName + " :" + args[1]);
	} else if (command == "PART")
	{
		debug("cmd: PART");
		std::string channelName = args[0];
		if (channelName[0] != '#')
			return ;
		Channel *channel = getChannelByName(channelName);
		if (channel == NULL)
			return ;
		channel->removeClient(client);
	}
}

void Server::disconnectClient(Client *client)
{
	std::map<std::string, Channel *>::iterator it = __channels.begin();
	while (it != __channels.end())
	{
		it->second->removeClient(client);
		++it;
	}
	__clients.erase(client->getFd());
}

void Server::debugSendToAllClients(const std::string& message)
{
	std::map<int, Client *>::iterator it = __clients.begin();
	while (it != __clients.end())
	{
		send(it->second->getFd(), message.c_str(), message.size(), 0);
		++it;
	}
}
