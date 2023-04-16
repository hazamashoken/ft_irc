# include "../Command.hpp"
# include "../../Client.hpp"
# include "../../Server.hpp"
# include "../RPL.hpp"
# include "../../parsers/Message.hpp"
namespace Commands {
	void USER(Command *command)
	{
		// TODO check if bitmode is needed ?
		command->getClient()->setUsername(command->getMessage()->getUser());
		command->getClient()->setHostname(command->getMessage()->getHostname());
		command->getClient()->setRealname(command->getMessage()->getRealname());
	}
}
