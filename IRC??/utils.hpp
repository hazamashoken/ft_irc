# pragma once

# ifndef DEBUG
#  define DEBUG	1
# endif

# define INDENT_TIME	7
# define INDENT_SRC		6

# include "ANSI.hpp"

# include <iostream>
# include <sstream>
# include <fstream>
# include <string>
# include <vector>
# include <ctime>

std::vector<std::string>	ft_split(const std::string &str, const std::string &sep);

int					error(const std::string &str, const int ret);
void				error(const std::string &str);
void				debug(const std::string &str);
void				debug(const std::string &src, const std::string &str);
void				debug(const std::ostream& str);
std::string			s_debug(const std::string &str);
std::string			s_debug(const std::string &src, const std::string &str);
void				time(const int time);
std::string			s_time(const int time);
const std::string	irc_tolower(const std::string& str);
