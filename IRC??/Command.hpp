# pragma once

# include <iostream>
# include <string>

class Command
{
	public:
		Command();
		~Command();

		const std::string	&get_command(void) const;
		const std::string	&get_prefix(void) const;
		const std::string	&get_params(void) const;
		const std::string	&get_trailing(void) const;

		void				set_command(const std::string &command);
		void				set_prefix(const std::string &prefix);
		void				set_params(const std::string &params);
		void				set_trailing(const std::string &trailing);

	private:
		std::string			_command;
		std::string			_prefix;
		std::string			_params;
		std::string			_trailing;
};
