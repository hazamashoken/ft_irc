# include "../utils/utils.hpp"
# include "Command.hpp"
# include "../Client.hpp"
# include "../Server.hpp"
# include "../parsers/Message.hpp"

Command::Command(Client *client, Server *server, Message* message)
: __client(client), __server(server), __message(message)
{
}

Command::~Command()
{
	delete __message;
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

Message *Command::getMessage() const
{
	return __message;
}

