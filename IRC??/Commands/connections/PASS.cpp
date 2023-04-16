# include "../Command.hpp"
# include "../../Client.hpp"
# include "../../Server.hpp"
# include "../RPL.hpp"
# include "../../parsers/Message.hpp"
# include "../../Status.hpp"

namespace Commands{
	void PASS(Command *command)
	{
		Debug::debug("PASS: " + command->getMessage()->getPassword() + " =?= " + command->getServer()->getPass());
		if (command->getMessage()->getPassword() == command->getServer()->getPass())
		{
			command->getClient()->setStatus(Status::REGISTER);
			Debug::debug("REGISTER");
		}
		else
			command->reply(IRC::ERR_PASSWDMISMATCH, "Password incorrect");
	}
}
