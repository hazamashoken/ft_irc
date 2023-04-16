# include "../Command.hpp"
# include "../../Client.hpp"
# include "../../Server.hpp"
# include "../RPL.hpp"
# include "../../parsers/Message.hpp"

# define IRC_NONUSER std::string("\0\r\n @", 5)
# define IRC_SPECIAL std::string("-[]\\`^{}")
namespace Commands {

	void	NICK(Command *command)
	{
		Debug::debug("NICK: " + command->getMessage()->getNickname());
		// TODO: check if nickname is valid
		const std::string &nickname = command->getMessage()->getNickname();

		std::string message = "NICK :" + nickname;
		Debug::debug("REPLY: ", message);
		send(command->getClient()->getFd(), message.c_str(), message.size(), 0);
		command->getClient()->setNickname(nickname);
		Debug::debug("Nickname changed to " + nickname);
	}
}
