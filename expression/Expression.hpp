/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expression.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 10:06:37 by abossel           #+#    #+#             */
/*   Updated: 2023/04/08 23:58:43 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _EXPRESSION_HPP_
# define _EXPRESSION_HPP_

# include <string>
# include <vector>

class Expression
{
	public:
		Expression();
		Expression(std::string *matched);
		Expression(Expression const &other);
		Expression(Expression const &other, std::string *matched);
		virtual ~Expression();
		Expression &operator=(Expression const &other);

		Expression &all(std::string pattern, size_t min, size_t max = SIZE_MAX);
		Expression &all(std::string pattern);
		Expression &any(std::string pattern, size_t min, size_t max = SIZE_MAX);
		Expression &any(std::string pattern);
		Expression &inv(std::string pattern, size_t min, size_t max = SIZE_MAX);
		Expression &inv(std::string pattern);
		Expression &exp(Expression const &expression, size_t min, size_t max = SIZE_MAX);
		Expression &exp(Expression const &expression);
		Expression &end();
		Expression &digit(size_t min, size_t max = SIZE_MAX);
		Expression &digit();
		Expression &upper(size_t min, size_t max = SIZE_MAX);
		Expression &upper();
		Expression &lower(size_t min, size_t max = SIZE_MAX);
		Expression &lower();
		Expression &alpha(size_t min, size_t max = SIZE_MAX);
		Expression &alpha();
		Expression &alnum(size_t min, size_t max = SIZE_MAX);
		Expression &alnum();
		Expression &alnumspec(size_t min, size_t max = SIZE_MAX);
		Expression &alnumspec();
		Expression &nonwhite(size_t min, size_t max = SIZE_MAX);
		Expression &nonwhite();
		Expression &chstring(size_t min, size_t max = SIZE_MAX);
		Expression &chstring();
		bool match(std::string string);
		void set_matched(std::string *matched);
		std::string get_matched();
		std::string get_remainder();

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

		container_type _expression_list;
		std::string _matched_string;
		std::string _remainder_string;
		std::string *_extra_matched_string;
};

#endif
