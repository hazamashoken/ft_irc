# include "Client.hpp"
# include "Server.hpp"
# include "Channel.hpp"
# include "Status.hpp"

Client::Client(const int fd, struct sockaddr_in addr)
: __socket(fd), __address(addr), __status(Status::PASSWORD), __registered(false)
{}

Client::~Client() {}

void Client::sendMessage(Client *target, const std::string& message) {
	// Implementation
	send(target->__socket, message.c_str(), message.length(), 0);
}

void Client::sendMessage(Channel *channel, const std::string& message) {
	// Implementation
	std::map<std::string, Client *>::iterator it = channel->getClientsBegin();
	while (it != channel->getClients().end()) {
		send(it->second->__socket, message.c_str(), message.length(), 0);
		it++;
	}
}

void Client::joinChannel(const std::string& channelName) {
	// check if client is already in channel
	{
		std::map<std::string, Channel *>::iterator it = __currentChannels.find(channelName);
		if (it != __currentChannels.end()) {
			return;
		}
	}
	// check if channel exists
	{
		std::map<std::string, Channel *>::iterator it = Server::__channels.find(channelName);
		if (it == Server::__channels.end()) {
			Channel *newChannel = new Channel(channelName);
			Server::__channels.insert(std::pair<std::string, Channel *>(channelName, newChannel));
			newChannel->addClient(this);
		} else {
			it->second->addClient(this);
		}
	}
	// add channel to client's currentChannels
	__currentChannels.insert(std::pair<std::string, Channel *>(channelName, Server::__channels[channelName]));
}

void Client::partChannel(const std::string& channelName) {
	// check if client is in channel
	std::map<std::string, Channel *>::iterator it = __currentChannels.find(channelName);
	if (it == __currentChannels.end()) {
		return;
	}
	// remove channel from client's currentChannels
	__currentChannels.erase(it);
	// remove client from channel's clients
	Server::__channels[channelName]->removeClient(this);
	// check if channel is empty
	if (Server::__channels[channelName]->getClients().size() == 0) {
		// delete channel
		delete Server::__channels[channelName];
		// remove channel from Server::__channels
		Server::__channels.erase(channelName);
	}
}

void Client::setNickname(const std::string& newNickname) {
	__nickname = newNickname;
}

void Client::setUsername(const std::string& newUsername) {
	__username = newUsername;
}

void Client::setRealname(const std::string& newRealname) {
	__realname = newRealname;
}

void Client::setHostname(const std::string& newHostname) {
	__hostname = newHostname;
}

void Client::setServername(const std::string& newServername) {
	__servername = newServername;
}

const std::string& Client::getNickname() const {
	return __nickname;
}

const std::string& Client::getUsername() const {
	return __username;
}

const std::string& Client::getRealname() const {
	return __realname;
}

const std::string& Client::getHostname() const {
	return __hostname;
}

const std::string& Client::getServername() const {
	return __servername;
}

const std::string& Client::getReadBuffer() const {
	return __readBuffer;
}

bool Client::isAliveClient() {
	char buffer[512];
	int bytes = recv(__socket, buffer, 512, 0);
	if (bytes == 0) {
		return false;
	}
	__readBuffer.append(buffer, bytes);
	return true;
}

int Client::getFd() const {
	return __socket;
}

void Client::sendReply(short code, const std::string& reply) {
	if (DEBUG)
	{
		std::stringstream o;
		o << "fd: " << __socket << " > " << getPrefix() + " " + itostring(code) + " " + reply;
		Debug::debug("REPLY", o.str());
	}
	send(__socket, (getPrefix() + " " + itostring(code) + " " + reply + "\r\n").c_str(), reply.length(), 0);
}

void Client::sendReply(short code, const std::string& reply, const std::string& message) {
	if (DEBUG)
	{
		std::stringstream o;
		o << "fd: " << __socket << " > " << getPrefix() + " " + itostring(code) + " " + reply + " " + message + "\r\n";
		Debug::debug("REPLY", o.str());
	}
	send(__socket, (getPrefix() + " " + itostring(code) + " " + reply + " " + message + "\r\n").c_str(), reply.length() + message.length() + 1, 0);
}

void Client::setReadBuffer(const std::string& newReadBuffer) {
	__readBuffer = newReadBuffer;
}

void Client::setRegistered(bool registered) {
	__registered = registered;
}

const std::string  Client::getPrefix() const {
	return ":"	+ __nickname + "!" + __username + "@" + __hostname;
}

void Client::setStatus(int status) {
	__status = status;
}

const std::string& Client::getSendBuffer() const {
	return __sendBuffer;
}

void Client::appendSendBuffer(const std::string& message) {
	__sendBuffer.append(message);
}

void Client::clearSendBuffer() {
	__sendBuffer.clear();
}
