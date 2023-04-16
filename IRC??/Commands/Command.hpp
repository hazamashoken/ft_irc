# pragma once

# include <iostream>
# include <string>
# include <vector>

class Client;
class Server;
class Command
{
	public:
		Command(Client *client, Server *server, std::string command);
		~Command();

		std::string	get_command(void) const;
		std::string	get_prefix(void) const;
		std::vector<std::string>	get_params(void) const;
		std::string	get_trailing(void) const;
		Client		*getClient(void) const;
		Server		*getServer(void) const;

		void reply(short code, const std::string& message) const;
		void reply(Client &client, short code, const std::string& message) const;


	private:
		Client				*__client;
		Server				*__server;
		std::string			__command;
		std::string			__prefix;
		std::vector<std::string>	__params;
		std::string			__trailing;
};

namespace Commands{
	void	Nick(Command *command);

}
