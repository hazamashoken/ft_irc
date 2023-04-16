# include "Command.hpp"
# include "../Client.hpp"
# include "../Server.hpp"
# include "RPL.hpp"

namespace Commands {
	void	Nick(Command *command)
	{
		Client*	client = command->getClient();
		Server*	server = command->getServer();
		std::string	nickname = command->get_trailing();
		if (nickname.empty())
		{
			client->sendReply(IRC::ERR_NONICKNAMEGIVEN, "No nickname given");
			return ;
		}
		if (nickname.find_first_of(" \t\r\n") != std::string::npos)
		{
			client->sendReply(IRC::ERR_ERRONEUSNICKNAME, "Erroneus nickname");
			return ;
		}
		if (nickname.length() > 9)
		{
			client->sendReply(IRC::ERR_ERRONEUSNICKNAME, "Erroneus nickname");
			return ;
		}
		if (server->getClientByNickname(nickname))
		{
			client->sendReply(IRC::ERR_NICKNAMEINUSE, "Nickname is already in use");
			return ;
		}
		client->setNickname(nickname);
		// client->sendReply(IRC::RPL_NICKNAME, "Nickname changed to " + nickname);
	}
}
