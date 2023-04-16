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

# include "utils/utils.hpp"

class Server;
class Channel;

class Client {
public:
	Client(const int fd, struct sockaddr_in addr);
	~Client();

	void sendMessage(Client *target, const std::string& message);
	void sendMessage(Channel *channel, const std::string& message);
	void sendReply(short code, const std::string& reply);
	void sendReply(short code, const std::string& reply, const std::string& message);
	void joinChannel(const std::string& channelName);
	void partChannel(const std::string& channelName);
	void setNickname(const std::string& newNickname);
	void setUsername(const std::string& newUsername);
	void setRealname(const std::string& newRealname);
	void setHostname(const std::string& newHostname);
	void setServername(const std::string& newServername);
	void setStatus(int status);
	const std::string& getNickname() const;
	const std::string& getUsername() const;
	const std::string& getRealname() const;
	const std::string& getHostname() const;
	const std::string& getServername() const;
	const std::string& getReadBuffer() const;
	void setReadBuffer(const std::string& message);
	int getFd() const;
	bool isAliveClient();
	void setRegistered(bool registered);
	const std::string getPrefix() const;

private:
	int __socket;
	struct sockaddr_in __address;
	std::string __nickname;
	std::string __username;
	std::string __realname;
	std::string __hostname;
	std::string __servername;
	int	__status;
	std::map<std::string, Channel*> __currentChannels;
	std::string __readBuffer;
	bool __registered;
};


