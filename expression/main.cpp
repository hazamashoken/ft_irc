/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 10:53:19 by abossel           #+#    #+#             */
/*   Updated: 2023/04/07 20:54:48 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Expression.hpp"
#include <iostream>
#include <string>

void test(Expression &e, std::string s)
{
	if (e.match(s))
		std::cout << "Match: " << e.get_matched() << std::endl;
	else
		std::cout << "No match" << std::endl;
}

int main()
{
	Expression ip_address;
	ip_address.digit(1, 3).exp(Expression().any(".").digit(1, 3), 3, 3);

	test(ip_address, "1.1.1.1");
	test(ip_address, "192.168.8.2");
	test(ip_address, "abd124");

	Expression hostname;
	hostname.alnum(1).exp(Expression().any(".").alnum(1), 0);

	test(hostname, "abc123");
	test(hostname, "www.google.com");
	test(hostname, "www.");

	std::string nick;
	std::string user;
	std::string host;

	Expression prefix;
	prefix.exp(Expression(&nick).alpha().alnumspec(1)).
		exp(Expression().any("!").exp(Expression(&user).nonwhite(1)), 0).
		exp(Expression().any("@").exp(Expression(&host).alnum(1).exp(Expression().any(".").alnum(1), 0)), 0);

	test(prefix, "nickname1!username1@example.com");
	std::cout << "nick: " << nick << std::endl;
	std::cout << "user: " << user << std::endl;
	std::cout << "host: " << host << std::endl;
	test(prefix, "nickname2!username2");
	std::cout << "nick: " << nick << std::endl;
	std::cout << "user: " << user << std::endl;
	std::cout << "host: " << host << std::endl;
	test(prefix, "nickname1@10.10.10.255");
	std::cout << "nick: " << nick << std::endl;
	std::cout << "user: " << user << std::endl;
	std::cout << "host: " << host << std::endl;

	return (0);
}
