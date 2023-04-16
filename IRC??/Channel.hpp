# pragma once

// Prototypes for the Channel class in an IRC server implementation in C++ (std=98)

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Client;

class Channel {
	public:
		Channel(const std::string& name);
		~Channel();

		const std::string& getName() const;
		void addClient(Client* client);
		void removeClient(Client* client);
		bool hasClient(Client* client) const;
		void sendMessage(Client* sender, const std::string& message);
		void sendToAllClients(const std::string& message);
		const std::map<std::string, Client*>& getClients() const;
		std::map<std::string, Client*>::iterator getClientsBegin();

	private:
		std::string __name;
		std::map<std::string, Client *> __clients;
};
