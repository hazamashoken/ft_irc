#include "utils.hpp"

using namespace ANSI;

void	debug(const std::string &str)
{
	if (!DEBUG)
		return ;

	std::cout << red << bold << "[ DEBUG ]:\t" << reset << red
		<< str << reset << std::endl;
}

void	debug(const std::string &src, const std::string &str)
{
	if (!DEBUG)
		return ;

	std::cout << red << bold << "[ DEBUG ]:\t"
		<< reset << ANSI::src << bold << '[' << src << "]:"
		<< std::string(((long)(INDENT_SRC - src.length()) >= 0 ?
					INDENT_SRC - src.length() : 0), ' ')
		<< reset << red << "\t" << str
		<< reset << std::endl;
}

void	debug(const std::ostream& str)
{
	if (!DEBUG)
		return ;

	std::cout << red << bold << "[ DEBUG ]:\t" << reset << red
		<< str.rdbuf() << reset << std::endl;
}

std::string	s_debug(const std::string &str)
{
	std::stringstream	o;

	o << red << bold << "[ DEBUG ]:\t" << reset << red << str;

	return (o.str());
}

std::string	s_debug(const std::string &src, const std::string &str)
{
	std::stringstream	o;

	o << red << bold << "[ DEBUG ]:\t"
		<< reset << ANSI::src << bold << '[' << src << "]:"
		<< std::string(((long)(INDENT_SRC - src.length()) >= 0 ?
					INDENT_SRC - src.length() : 0), ' ')
		<< reset << red << "\t" << str;

	return (o.str());
}
