/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 10:53:19 by abossel           #+#    #+#             */
/*   Updated: 2023/04/14 19:51:21 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Expression.hpp"
#include "Message.hpp"
#include <iostream>
#include <string>

#include "valid.hpp"
#include "invalid.hpp"

#define IRC_SPECIAL std::string("-[]\\`^{}")
#define IRC_NONUSER std::string("\0\r\n @", 5)

void test(Expression &e, std::string s)
{
	if (e.match(s))
		std::cout << "Match: " << e.get_matched() << std::endl;
	else
		std::cout << "No match: " << s << std::endl;
}

bool readPrefix(std::string prefix, std::string &server, std::string &nick, std::string &user, std::string &host)
{
	Expression nick_ex;
	nick_ex.alpha().add(IRC_SPECIAL).exp(Expression().alnum().add(IRC_SPECIAL).add("-"), 0, 8);

	Expression user_ex;
	user_ex.inv(IRC_NONUSER, 1);

	Expression short_ex;
	short_ex.alnum(1).exp(Expression().any("-", 0).alnum(1), 0);

	Expression host_ex;
	host_ex.exp(short_ex).exp(Expression().any(".").exp(short_ex, 1), 0);

	Expression server_ex(server);
	server_ex.exp(short_ex).exp(Expression().any(".").exp(short_ex, 1), 0);

	Expression prefix_ex;
	prefix_ex.exp(Expression().exp(nick_ex, nick).any("!").exp(user_ex, user).any("@").exp(host_ex, host)).jmp();
	prefix_ex.exp(Expression().exp(nick_ex, nick).any("@").exp(host_ex, host)).jmp();
	prefix_ex.exp(nick_ex).jmp();
	//prefix_ex.exp(server_ex);

	return (prefix_ex.match(prefix));
}

void testPrefix(std::string s)
{
	std::string server;
	std::string nick;
	std::string user;
	std::string host;

	if (readPrefix(s, server, nick, user, host))
	{
		std::cout << "Match: " << s << std::endl;
		std::cout << "server: " << server << std::endl;
		std::cout << "nick: " << nick << std::endl;
		std::cout << "user: " << user << std::endl;
		std::cout << "host: " << host << std::endl;
	}
	else
	{
		std::cout << "No match: " << s << std::endl;
	}
	std::cout << std::endl;
}

int main()
{
	// Expression ip_address;
	// ip_address.digit(1, 3).exp(Expression().any(".").digit(1, 3), 3, 3);

	// test(ip_address, "1.1.1.1");
	// test(ip_address, "192.168.8.2");
	// test(ip_address, "abd124");

	// std::cout << std::endl;

	// Expression shortname;
	// Expression hostname;
	// shortname.alnum(1).exp(Expression().any("-", 0).alnum(1), 0);
	// hostname.exp(shortname).exp(Expression().any(".").exp(shortname, 1), 0);

	// test(hostname, "abc123");
	// test(hostname, "www.google.com");
	// test(hostname, "a-b-c.example.com");
	// test(hostname, "example-.com");
	// test(hostname, "www.");

	// std::cout << std::endl;

	// Expression email;
	// email.alpha().alnum(0).add("+.").all("@").exp(hostname);

	// test(email, "abc123@example.com");
	// test(email, "ab.c+123@example.com");
	// test(email, "1abc123@example.com");
	// test(email, "abc123@@example.com");

	// std::cout << std::endl;

	// testPrefix("irc1.example.com");
	// testPrefix("[nick2]");
	// testPrefix("nick3!username3@irc3.example.com");
	// testPrefix("nick4@irc4.example.com");
	// testPrefix("nick5!username5");
	// testPrefix("nick6@10.6.6.255");

	Message m;
	if (m.getMsgType("PASS hellohello\r\n") == IRC_PASS)
		std::cout << "msg: " << m.getMsgStr() << " pass: " << m.getPassword() << std::endl;
	if (m.getMsgType("NICK Wizard\r\n") == IRC_NICK)
		std::cout << "msg: " << m.getMsgStr() << " nick: " << m.getNickname() << std::endl;
	if (m.getMsgType("USER guest 0 * :Ronnie Reagan\r\n") == IRC_USER)
		std::cout << "msg: " << m.getMsgStr() << " user: " << m.getUser() << "|" << m.getBitmode() << "|" << m.getRealname() << std::endl;
	if (m.getMsgType("OPER foo barbar\r\n") == IRC_OPER)
		std::cout << "msg: " << m.getMsgStr() << " oper: " << m.getUser() << "|" << m.getPassword() << std::endl;
	if (m.getMsgType("MODE +w\r\n") == IRC_MODE)
		std::cout << "msg: " << m.getMsgStr() << " mode: " << m.getMode() << std::endl;

	return (0);
}
