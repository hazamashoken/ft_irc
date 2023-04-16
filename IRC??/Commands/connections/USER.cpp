# include "../Command.hpp"
# include "../../Client.hpp"
# include "../../Server.hpp"
# include "../RPL.hpp"

namespace Commands {
	void USER(Command *command)
	{
		Client*	client = command->getClient();
		Server*	server = command->getServer();
		std::vector<std::string> params = command->get_params();
		if (params.size() < 4)
		{
			client->sendReply(IRC::ERR_NEEDMOREPARAMS, "Not enough parameters");
			return ;
		}
		client->setUsername(params[0]);
		client->setHostname(params[1]);
		client->setServername(params[2]);
		client->setRealname(params[3]);
		client->sendReply(IRC::RPL_WELCOME, "Welcome to the Internet Relay Network " + client->getNickname() + "!");
		client->sendReply(IRC::RPL_YOURHOST, "Your host is " + server->getHostname() + ", running version " + server->getVersion());
		client->sendReply(IRC::RPL_CREATED, "This server was created " + server->getCreationDate());
		client->sendReply(IRC::RPL_MYINFO, server->getHostname() + " " + server->getVersion()); //+ " " + server->getAvailableUserModes() + " " + server->getAvailableChannelModes());
	}
}
