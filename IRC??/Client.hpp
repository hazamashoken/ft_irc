#pragma once

# include "utils.hpp"
// # include "../cmd/Cmd.hpp"

# include <map>
# include <iostream>
# include <ctime>
# include <string>
# include <vector>
# include <netinet/in.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>

# define KILLED		-2
# define INIT		-1
# define DELETE		0
# define PASSWORD	1
# define REGISTER	2
# define ONLINE		3

class Server;
class Client
{
	private:
		bool		_op;
		int			_fd;
		int			_status;
		int			_last_ping;

		std::string	_buffer_receive;
		std::string	_buffer_to_send;

		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_hostaddr;
		std::string	_hostname;

	public:
		Client(const int fd, struct sockaddr_in addr);
		virtual ~Client();

		void	write_buffer(const std::string &str);
		void	write_to(Client &c, const std::string &msg);
		// virtual ssize_t	send_buffer(void);
		// virtual void	receive(Server *server);

		void	set_status(const int status);
		void	set_last_ping(const int last_ping);
		void	set_nickname(const std::string &nickname);
		void	set_username(const std::string &username);
		void	set_realname(const std::string &realname);

		bool	is_operator();
		void	promote_as_op();

		int					get_fd(void) const;
		int					get_status(void) const;
		int					get_last_ping(void) const;
		const std::string	&get_nickname(void) const;
		const std::string	&get_host(void) const;
		std::string			get_prefix(void) const;
		const std::string	&get_username(void) const;
		const std::string	&get_realname(void) const;
};


