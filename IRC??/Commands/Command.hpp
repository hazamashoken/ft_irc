# pragma once

# include <iostream>
# include <string>
# include <vector>

class Message;
class Client;
class Server;
class Command
{
	public:
		Command(Client *client, Server *server, Message *message);
		~Command();

		Client		*getClient(void) const;
		Server		*getServer(void) const;
		Message		*getMessage(void) const;

		void reply(short code, const std::string& message) const;
		void reply(Client &client, short code, const std::string& message) const;


	private:
		Client				*__client;
		Server				*__server;
		Message				*__message;
};

namespace Commands{
	void	NICK(Command *command);
	void	USER(Command *command);

}
