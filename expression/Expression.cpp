/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expression.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 10:19:58 by abossel           #+#    #+#             */
/*   Updated: 2023/04/09 00:01:01 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Expression.hpp"
#include <algorithm>

#define EXPRESSION_ANY 1
#define EXPRESSION_ALL 2
#define EXPRESSION_EXP 3
#define EXPRESSION_END 4

Expression::Expression()
{
	set_matched(NULL);
}

Expression::Expression(std::string *matched)
{
	set_matched(matched);
}

Expression::Expression(Expression const &other)
{
	*this = other;
}

Expression::Expression(Expression const &other, std::string *matched)
{
	*this = other;
	set_matched(matched);
}

Expression::~Expression()
{
	iterator_type it;

	for (it = _expression_list.begin(); it != _expression_list.end(); it++)
		if (it->expression != NULL)
			delete it->expression;
	_expression_list.clear();
}

Expression &Expression::operator=(Expression const &other)
{
	struct Pattern p;
	size_t i;

	if (this != &other)
	{
		_expression_list.clear();
		for (i = 0; i < other._expression_list.size(); i++)
		{
			p.expression = NULL;
			if (other._expression_list[i].expression != NULL)
				p.expression = new Expression(*other._expression_list[i].expression);
			p.pattern = other._expression_list[i].pattern;
			p.min = other._expression_list[i].min;
			p.max = other._expression_list[i].max;
			p.type = other._expression_list[i].type;
			_expression_list.push_back(p);
		}
		_matched_string = other._matched_string;
		_remainder_string = other._remainder_string;
		_extra_matched_string = other._extra_matched_string;
	}
	return (*this);
}

/*
 * match the string pattern min to max times
 */
Expression &Expression::all(std::string pattern, size_t min, size_t max)
{
	struct Pattern p;

	p.expression = NULL;
	p.pattern = pattern;
	p.min = min;
	p.max = max;
	p.type = EXPRESSION_ALL;
	_expression_list.push_back(p);
	return (*this);
}

/*
 * match the string pattern 1 time
 */
Expression &Expression::all(std::string pattern)
{
	return (all(pattern, 1, 1));
}

/*
 * match any char in the string pattern min to max times
 */
Expression &Expression::any(std::string pattern, size_t min, size_t max)
{
	struct Pattern p;

	p.expression = NULL;
	p.pattern = pattern;
	p.min = min;
	p.max = max;
	p.type = EXPRESSION_ANY;
	_expression_list.push_back(p);
	return (*this);
}

/*
 * match any char in the string pattern 1 time
 */
Expression &Expression::any(std::string pattern)
{
	return (any(pattern, 1, 1));
}

/*
 * match any char not in the string pattern min to max times.
 */
Expression &Expression::inv(std::string pattern, size_t min, size_t max)
{
	std::string str;
	char c;
	int i;

	for (i = 0; i < 256; i++)
	{
		if (i > 127)
			c = static_cast<char>(i - 256);
		else
			c = static_cast<char>(i);
		if (pattern.find_first_of(c) == std::string::npos)
			str.push_back(c);
	}
	return (any(str, min, max));
}

/*
 * match any char not in the string pattern 1 time
 */
Expression &Expression::inv(std::string pattern)
{
	return (inv(pattern, 1, 1));
}

/*
 * match the expression min to max times
 */
Expression &Expression::exp(Expression const &expression, size_t min, size_t max)
{
	struct Pattern p;

	p.expression = new Expression(expression);
	p.expression->end();
	p.min = min;
	p.max = max;
	p.type = EXPRESSION_EXP;
	_expression_list.push_back(p);
	return (*this);	
}

/*
 * match the expression 1 time
 */
Expression &Expression::exp(Expression const &expression)
{
	return (exp(expression, 1, 1));
}

/*
 * stop processing the expression
 */
Expression &Expression::end()
{
	struct Pattern p;

	if (_expression_list.size() != 0 && _expression_list.back().type == EXPRESSION_END)
		return (*this);
	p.expression = NULL;
	p.min = 0;
	p.max = 0;
	p.type = EXPRESSION_END;
	_expression_list.push_back(p);
	return (*this);	
}

Expression &Expression::digit(size_t min, size_t max)
{
	return (any("0123456789", min, max));
}

Expression &Expression::digit()
{
	return (digit(1, 1));
}

Expression &Expression::upper(size_t min, size_t max)
{
	return (any("ABCDEFGHIJKLMNOPQRSTUVWXYZ", min, max));
}

Expression &Expression::upper()
{
	return (upper(1, 1));
}

Expression &Expression::lower(size_t min, size_t max)
{
	return (any("abcdefghijklmnopqrstuvwxyz", min, max));
}

Expression &Expression::lower()
{
	return (lower(1, 1));
}

Expression &Expression::alpha(size_t min, size_t max)
{
	return (any("abcdefghijklmnopqrstuvwxyz"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ", min, max));
}

Expression &Expression::alpha()
{
	return (alpha(1, 1));
}

Expression &Expression::alnum(size_t min, size_t max)
{
	return (any("0123456789"
				"abcdefghijklmnopqrstuvwxyz"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ", min, max));
}

Expression &Expression::alnum()
{
	return (alnum(1, 1));
}

/*
 * alpha numeric and irc special chars
 */
Expression &Expression::alnumspec(size_t min, size_t max)
{
	return (any("0123456789"
				"abcdefghijklmnopqrstuvwxyz"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"-[]\\`^{}", min, max));
}

Expression &Expression::alnumspec()
{
	return (alnumspec(1, 1));
}

/*
 * any 8bit code except SPACE, NUL, CR and LF
 */
Expression &Expression::nonwhite(size_t min, size_t max)
{
	return (inv(" \0\r\n", min, max));
}

Expression &Expression::nonwhite()
{
	return (nonwhite(1, 1));
}

/*
 * any 8bit code except SPACE, BEL, NUL, CR, LF and comma
 */
Expression &Expression::chstring(size_t min, size_t max)
{
	return (inv(" \a\0\r\n,", min, max));
}

Expression &Expression::chstring()
{
	return (chstring(1, 1));
}

/*
 * try to match the expression to string
 * returns true if the expression matches the string
 * the matched string and remainder are only stored on success
 */
bool Expression::match(std::string string)
{
	iterator_type it;
	std::string string2;
	std::string remainder2;
	size_t count;

	_matched_string = "";
	string2 = string;
	for (it = _expression_list.begin(); it != _expression_list.end(); it++)
	{
		count = 0;
		if (it->type == EXPRESSION_ALL)
		{
			while (string2.find(it->pattern) == 0 && count < it->max)
			{
				string2 = string2.substr(it->pattern.size());
				count++;
			}
		}
		else if (it->type == EXPRESSION_ANY)
		{
			if (string2.size() != 0)
				count = string2.find_first_not_of(it->pattern);
			if (count > it->max || count == std::string::npos)
				count = std::min(it->max, string2.size());
			string2 = string2.substr(count);
		}
		else if (it->type == EXPRESSION_EXP)
		{
			while (it->expression->match(string2) && count < it->max)
			{
				string2 = it->expression->get_remainder();
				count++;
			}
		}
		else if (it->type == EXPRESSION_END)
		{
			_remainder_string = string2;
			_matched_string = string.substr(0, string.size() - string2.size());
			if (_extra_matched_string != NULL)
				*_extra_matched_string = _matched_string;
			return (true);
		}
		if (count < it->min)
			return (false);
	}
	if (string2.size() != 0)
		return (false);
	_remainder_string = "";
	_matched_string = string;
	if (_extra_matched_string != NULL)
		*_extra_matched_string = _matched_string;
	return (true);
}

void Expression::set_matched(std::string *matched)
{
	_extra_matched_string = matched;
}

std::string Expression::get_matched()
{
	return (_matched_string);
}

std::string Expression::get_remainder()
{
	return (_remainder_string);
}
