/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expression.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tliangso <earth78203@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 10:06:37 by abossel           #+#    #+#             */
/*   Updated: 2023/04/16 00:12:04 by tliangso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _EXPRESSION_HPP_
# define _EXPRESSION_HPP_

# include <string>
# include <vector>
# include <stdint.h>

class Expression
{
	public:
		Expression();
		Expression(std::string &matched);
		Expression(Expression const &other);
		Expression(Expression const &other, std::string &matched);
		virtual ~Expression();
		Expression &operator=(Expression const &other);

		Expression &all(std::string const pattern, size_t min, size_t max = SIZE_MAX);
		Expression &all(std::string const pattern);
		Expression &any(std::string const pattern, size_t min, size_t max = SIZE_MAX);
		Expression &any(std::string const pattern);
		Expression &add(std::string const pattern);
		Expression &inv(std::string const pattern, size_t min, size_t max = SIZE_MAX);
		Expression &inv(std::string const pattern);
		Expression &exp(Expression const &expression, size_t min, size_t max = SIZE_MAX);
		Expression &exp(Expression const &expression);
		Expression &exp(Expression const &expression, std::string &matched, size_t min, size_t max = SIZE_MAX);
		Expression &exp(Expression const &expression, std::string &matched);
		Expression &jmp();
		Expression &jmpe();
		Expression &con();
		Expression &end();
		Expression &digit(size_t min, size_t max = SIZE_MAX);
		Expression &digit();
		Expression &xdigit(size_t min, size_t max = SIZE_MAX);
		Expression &xdigit();
		Expression &upper(size_t min, size_t max = SIZE_MAX);
		Expression &upper();
		Expression &lower(size_t min, size_t max = SIZE_MAX);
		Expression &lower();
		Expression &alpha(size_t min, size_t max = SIZE_MAX);
		Expression &alpha();
		Expression &alnum(size_t min, size_t max = SIZE_MAX);
		Expression &alnum();
		bool match(std::string string);
		std::string get_matched() const;
		std::string get_remainder() const;

	private:
		struct Pattern
		{
			Expression *expression;
			std::string pattern;
			size_t min;
			size_t max;
			int type;
		};

		typedef std::vector<struct Pattern> container_type;
		typedef container_type::iterator iterator_type;

		void clear();
		void clone_pattern(struct Pattern const &p);
		int next_type(iterator_type it);

		container_type _pattern_list;
		std::string _matched_string;
		std::string _remainder_string;
		std::string *_extra_matched_string;
};

#endif
