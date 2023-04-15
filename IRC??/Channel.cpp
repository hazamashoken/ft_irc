# include "Channel.hpp"
# include "Client.hpp"

Channel::Channel(const std::string& name)
: __name(name)
{
}

Channel::~Channel() {
    // Destructor implementation
}

const std::string& Channel::getName() const {
    // Implementation
	return __name;
}

void Channel::addClient(Client* client) {
    // Implementation
	__clients.insert(std::pair<std::string, Client*>(client->getNickname(), client));
}

void Channel::removeClient(Client* client) {
    // Implementation
	__clients.erase(client->getNickname());
}

bool Channel::hasClient(Client* client) const {
    // Implementation
	std::map<std::string, Client*>::const_iterator it = __clients.find(client->getNickname());
	if (it == __clients.end()) {
		return false;
	}
	return true;
}

void Channel::sendMessage(Client* sender, const std::string& message) {
    // Implementation
	std::map<std::string, Client*>::iterator it = __clients.begin();
	while (it != __clients.end()) {
		it->second->sendMessage(sender, message);
		it++;
	}
}

const std::map<const std::string, Client*>& Channel::getClients() const {
	// Implementation
	return __clients;
}

std::map<const std::string, Client*>::iterator Channel::getClientsBegin() {
	// Implementation
	return __clients.begin();
}
