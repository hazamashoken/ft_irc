# include "utils.hpp"
# include "ANSI.hpp"
# include <string>

int	error(const std::string &str, const int ret)
{
	std::cerr << ANSI::red << ANSI::bold << "ERROR:\t" << ANSI::reset << ANSI::red
		<< str << ANSI::reset << std::endl;
	return(ret);
}

void	error(const std::string &str)
{
	std::cerr << ANSI::red << ANSI::bold << "ERROR:\t" << ANSI::reset << ANSI::red
		<< str << ANSI::reset << std::endl;
}

const std::string	irc_tolower(const std::string &str)
{
	std::string	res(str);

	for (size_t i = 0; i < str.size(); ++i)
	{
		if (res[i] >= 'A' && res[i] <= ']')
			res[i] = str[i] + 32;
	}
	return (res);
}

static int	ft_nbrlen(const int nb)
{
	unsigned int	n;
	size_t			i;

	if (nb == 0)
		return (1);
	i = 0;
	if (nb < 0)
	{
		n = -nb;
		i++;
	}
	else
		n = nb;
	while (n)
	{
		n /= 10;
		i++;
	}
	return (i);
}

void	time(const int time)
{
	int	indent;
	int	size;

	size = ft_nbrlen(time);
	indent = (INDENT_TIME - size) / 2;
	if (indent < 0)
		indent = 0;

	std::cout << ANSI::time << ANSI::bold << ANSI::italic <<
		'<' << std::string(indent, ' ') << time
		<< std::string((size % 2 == 0 ? indent + 1 : indent), ' ') << '>'
		<< ANSI::reset << std::endl;
}

std::string	s_time(const int time)
{
	std::stringstream	o;
	int					indent;
	int					size;

	size = ft_nbrlen(time);
	indent = (INDENT_TIME - size) / 2;
	if (indent < 0)
		indent = 0;

	o << ANSI::time << ANSI::bold << ANSI::italic <<
		'<' << std::string(indent, ' ') << time
		<< std::string((size % 2 == 0 ? indent + 1 : indent), ' ') << '>'
		<< ANSI::reset;

	return (o.str());
}

std::vector<std::string>	ft_split(const std::string &str, const std::string &sep)
{
	std::string					cpy = str;
	std::vector<std::string>	ret;
	size_t						pos;

	pos = cpy.find(sep);
	while (pos != std::string::npos)
	{
		ret.push_back(cpy.substr(0, pos));
		cpy.erase(0, pos + sep.length());
		pos = cpy.find(sep);
	}

	if (ret.empty() == true && cpy.empty() == false)
		ret.push_back(str);	// might not be needed

	return (ret);
}

const std::string	itostring(int i)
{
	std::stringstream	o;

	o << i;
	return (o.str());
}
