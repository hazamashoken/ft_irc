/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expression.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 10:19:58 by abossel           #+#    #+#             */
/*   Updated: 2023/04/10 12:38:53 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Expression.hpp"
#include <algorithm>

#define EXPRESSION_ANY 1
#define EXPRESSION_ALL 2
#define EXPRESSION_EXP 3
#define EXPRESSION_JMP 4
#define EXPRESSION_CON 5
#define EXPRESSION_END 6

Expression::Expression()
{
	_extra_matched_string = NULL;
}

Expression::Expression(std::string &matched)
{
	_extra_matched_string = &matched;
}

Expression::Expression(Expression const &other)
{
	*this = other;
}

Expression::Expression(Expression const &other, std::string &matched)
{
	*this = other;
	_extra_matched_string = &matched;
}

Expression::~Expression()
{
	iterator_type it;

	for (it = _pattern_list.begin(); it != _pattern_list.end(); it++)
		if (it->expression != NULL)
			delete it->expression;
	_pattern_list.clear();
}

Expression &Expression::operator=(Expression const &other)
{
	struct Pattern p;
	size_t i;

	if (this != &other)
	{
		_pattern_list.clear();
		for (i = 0; i < other._pattern_list.size(); i++)
		{
			p.expression = NULL;
			if (other._pattern_list[i].expression != NULL)
				p.expression = new Expression(*other._pattern_list[i].expression);
			p.pattern = other._pattern_list[i].pattern;
			p.min = other._pattern_list[i].min;
			p.max = other._pattern_list[i].max;
			p.type = other._pattern_list[i].type;
			_pattern_list.push_back(p);
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
Expression &Expression::all(std::string const pattern, size_t min, size_t max)
{
	struct Pattern p;

	p.expression = NULL;
	p.pattern = pattern;
	p.min = min;
	p.max = max;
	p.type = EXPRESSION_ALL;
	_pattern_list.push_back(p);
	return (*this);
}

/*
 * match the string pattern 1 time
 */
Expression &Expression::all(std::string const pattern)
{
	return (all(pattern, 1, 1));
}

/*
 * match any char in the string pattern min to max times
 */
Expression &Expression::any(std::string const pattern, size_t min, size_t max)
{
	struct Pattern p;

	p.expression = NULL;
	p.pattern = pattern;
	p.min = min;
	p.max = max;
	p.type = EXPRESSION_ANY;
	_pattern_list.push_back(p);
	return (*this);
}

/*
 * match any char in the string pattern 1 time
 */
Expression &Expression::any(std::string const pattern)
{
	return (any(pattern, 1, 1));
}

/*
 * adds the characters in string pattern to the last added pattern
 * only works if the last added is an ANY type pattern
 */
Expression &Expression::add(std::string const pattern)
{
	iterator_type it;

	if (!_pattern_list.empty()
		&& _pattern_list.back().type == EXPRESSION_ANY)
	{
		_pattern_list.back().pattern.append(pattern);
	}
	return (*this);
}

/*
 * match any char not in the string pattern min to max times.
 */
Expression &Expression::inv(std::string const pattern, size_t min, size_t max)
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
Expression &Expression::inv(std::string const pattern)
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
	_pattern_list.push_back(p);
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
 * match the expression min to max times with a different matched string
 */
Expression &Expression::exp(Expression const &expression, std::string &matched, size_t min, size_t max)
{
	struct Pattern p;

	p.expression = new Expression(expression, matched);
	p.expression->end();
	p.min = min;
	p.max = max;
	p.type = EXPRESSION_EXP;
	_pattern_list.push_back(p);
	return (*this);	
}

/*
 * match the expression 1 time with a different matched string
 */
Expression &Expression::exp(Expression const &expression, std::string &matched)
{
	return (exp(expression, matched, 1, 1));
}

/*
 * stop processing the expression
 */
Expression &Expression::end()
{
	struct Pattern p;

	if (_pattern_list.size() != 0 && _pattern_list.back().type == EXPRESSION_END)
		return (*this);
	p.expression = NULL;
	p.min = 0;
	p.max = 0;
	p.type = EXPRESSION_END;
	_pattern_list.push_back(p);
	return (*this);	
}

/*
 * on success of the previous match jump to the next con
 */
Expression &Expression::jmp()
{
	struct Pattern p;

	p.expression = NULL;
	p.min = 0;
	p.max = 0;
	p.type = EXPRESSION_JMP;
	_pattern_list.push_back(p);
	return (*this);	
}

/*
 * continue processing from the previous jump
 */
Expression &Expression::con()
{
	struct Pattern p;

	p.expression = NULL;
	p.min = 0;
	p.max = 0;
	p.type = EXPRESSION_CON;
	_pattern_list.push_back(p);
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

Expression &Expression::xdigit(size_t min, size_t max)
{
	return (any("0123456789abcdefABCDEF", min, max));
}

Expression &Expression::xdigit()
{
	return (xdigit(1, 1));
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

// /*
//  * alpha numeric and irc special chars
//  */
// Expression &Expression::alnumspec(size_t min, size_t max)
// {
// 	return (alnum(min, max).add("-[]\\`^{}"));
// }

// Expression &Expression::alnumspec()
// {
// 	return (alnumspec(1, 1));
// }

// /*
//  * any 8bit code except SPACE, NUL, CR and LF
//  */
// Expression &Expression::nonwhite(size_t min, size_t max)
// {
// 	return (inv(" \0\r\n", min, max));
// }

// Expression &Expression::nonwhite()
// {
// 	return (nonwhite(1, 1));
// }

// /*
//  * any 8bit code except SPACE, BEL, NUL, CR, LF and comma
//  */
// Expression &Expression::chstring(size_t min, size_t max)
// {
// 	return (inv(" \a\0\r\n,", min, max));
// }

// Expression &Expression::chstring()
// {
// 	return (chstring(1, 1));
// }

/*
 * try to match the expression to string
 * returns true if the expression matches the string
 * the matched string and remainder are only stored on success
 */
bool Expression::match(std::string string)
{
	iterator_type it;
	std::string previous;
	std::string current;
	bool jump;
	size_t count;

	// set all matched strings to empty
	_matched_string.clear();
	_remainder_string.clear();
	if (_extra_matched_string != NULL)
		_extra_matched_string->clear();
	jump = false;
	previous = string;
	current = string;
	for (it = _pattern_list.begin(); it != _pattern_list.end(); it++)
	{
		count = 0;
		if (it->type == EXPRESSION_ALL && !jump)
		{
			while (count < it->max && current.find(it->pattern) == 0)
			{
				current = current.substr(it->pattern.size());
				count++;
			}
		}
		else if (it->type == EXPRESSION_ANY && !jump)
		{
			if (current.size() != 0)
				count = current.find_first_not_of(it->pattern);
			if (count > it->max || count == std::string::npos)
				count = std::min(it->max, current.size());
			current = current.substr(count);
		}
		else if (it->type == EXPRESSION_EXP && !jump)
		{
			while (count < it->max && it->expression->match(current))
			{
				current = it->expression->get_remainder();
				count++;
			}
		}
		else if (it->type == EXPRESSION_JMP)
		{
			jump = true;
		}
		else if (it->type == EXPRESSION_CON)
		{
			jump = false;
		}
		else if (it->type == EXPRESSION_END)
		{
			_remainder_string = current;
			_matched_string = string.substr(0, string.size() - current.size());
			if (_extra_matched_string != NULL)
				*_extra_matched_string = _matched_string;
			return (true);
		}

		// minimum patterns not read so the expression failed
		// don't test if expression is currently jumping
		if (count < it->min && !jump)
		{
			// if next pattern is not jump then fail
			if (next_type(it) != EXPRESSION_JMP)
				return (false);
			// skip the next jump instruction
			it++;
			// roll back the changes to the string
			current = previous;
		}
		// reading this pattern is done so commit changes
		previous = current;
	}
	if (current.size() != 0)
		return (false);
	_remainder_string.clear();
	_matched_string = string;
	if (_extra_matched_string != NULL)
		*_extra_matched_string = _matched_string;
	return (true);
}

std::string Expression::get_matched() const
{
	return (_matched_string);
}

std::string Expression::get_remainder() const
{
	return (_remainder_string);
}

int Expression::next_type(iterator_type it)
{
	if (it != _pattern_list.end() && (it + 1) != _pattern_list.end())
		return ((it + 1)->type);
	return (EXPRESSION_END);
}
