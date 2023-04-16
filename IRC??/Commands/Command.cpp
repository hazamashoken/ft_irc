# include "../utils.hpp"
# include "Command.hpp"
# include "../Client.hpp"
# include "../Server.hpp"

Command::Command(Client *client, Server *server, std::string command)
: __client(client), __server(server), __command(command)
{
	std::string delimiter(":");
	size_t position;
	if ((position = command.find(delimiter)) != std::string::npos)
	{
		std::string tmp = command.substr(0, position);
		command.erase(0, position + delimiter.length());
		__trailing = command;
		command = tmp;
	}

	__params = ft_split(command, " ");
	__prefix = *(__params.begin());
	__params.erase(__params.begin());

	for (size_t index = 0; index < __prefix.length(); ++index)
		__prefix[index] = std::toupper(__prefix[index]);
}

Command::~Command()
{
}

std::string	Command::get_command(void) const
{
	return __command;
}

std::string	Command::get_prefix(void) const
{
	return __prefix;
}

std::vector<std::string>	Command::get_params(void) const
{
	return __params;
}

std::string	Command::get_trailing(void) const
{
	return __trailing;
}

void Command::reply(short code, const std::string& message) const
{
	__client->sendReply(code, message);
}

void Command::reply(Client &client, short code, const std::string& message) const
{
	client.sendReply(code, message);
}

Client	*Command::getClient(void) const
{
	return __client;
}

Server	*Command::getServer(void) const
{
	return __server;
}

