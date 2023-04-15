#pragma once

# include <iostream>
# include <string>
# include <map>
# include <cstring>
# include <cstdlib>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <fcntl.h>
# include <arpa/inet.h>

# include "utils.hpp"

class Server;
class Channel;

class Client {
public:
	Client(const int fd, struct sockaddr_in addr);
	~Client();

	void sendMessage(Client *target, const std::string& message);
	void sendMessage(Channel *channel, const std::string& message);
	void joinChannel(const std::string& channelName);
	void partChannel(const std::string& channelName);
	void setNickname(const std::string& newNickname);
	void setUsername(const std::string& newUsername);
	void setRealname(const std::string& newRealname);
	const std::string& getNickname() const;
	const std::string& getUsername() const;
	const std::string& getRealname() const;
	const std::string& getReadBuffer() const;
	int getFd() const;
	bool isAliveClient();

private:
	int __socket;
	struct sockaddr_in __address;
	std::string __nickname;
	std::string __username;
	std::string __realname;
	std::map<std::string, Channel*> __currentChannels;
	std::string __readBuffer;
};


