# include "Client.hpp"

Client::Client(const int fd, struct sockaddr_in addr)
: _op(false), _fd(fd), _status(INIT), _last_ping(std::time(NULL))
{
	fcntl(fd, F_SETFL, O_NONBLOCK);

	_hostaddr = inet_ntoa(addr.sin_addr);

	char tmp[NI_MAXHOST];
	if (getnameinfo((struct sockaddr *)&addr, sizeof(addr), tmp, NI_MAXHOST,
				NULL, 0, NI_NUMERICSERV))
		error("getnameinfo() failed");
	else
		_hostname = tmp;

	debug("CLIENT", "created");
}

Client::~Client()
{
	close(_fd);

	debug("CLIENT", "destroyed");
}

void	Client::set_status(const int status)
{
	if (DEBUG)
		std::cerr << s_debug("CLIENT", "set_status\t(")
			<< this->_status << " -> " << status << ')'
			<< ANSI::reset << std::endl;
	this->_status = status;
}

void	Client::set_last_ping(const int last_ping)
{ this->_last_ping = last_ping; }

void	Client::set_nickname(const std::string &nickname)
{ this->_nickname = nickname; }

void	Client::set_username(const std::string &username)
{ this->_username = username; }

void	Client::set_realname(const std::string &realname)
{ this->_realname = realname; }

int		Client::get_fd(void) const
{ return (this->_fd); }

int		Client::get_status(void) const
{ return (this->_status); }

int		Client::get_last_ping(void) const
{ return (this->_last_ping); }

const std::string	&Client::get_nickname(void) const
{ return (this->_nickname); }

const std::string	&Client::get_username(void) const
{ return (this->_username); }

const std::string	&Client::get_realname(void) const
{ return (this->_realname); }

const std::string	&Client::get_host(void) const
{
	if (!this->_hostname.size())
		return (this->_hostaddr);
	return (this->_hostname);
}

std::string Client::get_prefix(void) const
{
	if (_status != ONLINE || !get_host().length())
	{
		if (_nickname.size())
			return (_nickname);
		return (std::string(""));
	}

	std::string prefix = _nickname;

	if (_username.length())
		prefix += "!" + _username;
	prefix += "@" + get_host();

	return (prefix);
}

void	Client::write_buffer(const std::string &str)
{
	_buffer_to_send += str + "\r\n";
}

void	Client::write_to(Client &c, const std::string &msg)
{
	c.write_buffer(":" + get_prefix() + " " + msg);
}

bool Client::is_operator()
{ return (_op); }

void Client::promote_as_op()
{ _op = true; }

