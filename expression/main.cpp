/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abossel <abossel@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 10:53:19 by abossel           #+#    #+#             */
/*   Updated: 2023/04/16 16:55:36 by abossel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Expression.hpp"
#include "Message.hpp"
#include "Split.hpp"
#include <iostream>
#include <string>

#include "valid.hpp"
#include "invalid.hpp"

#define SEP " : "

void test(Expression &e, std::string s)
{
	if (e.match(s))
		std::cout << "Match: " << e.get_matched() << std::endl;
	else
		std::cout << "No match: " << s << std::endl;
}

int main()
{
	Split sp;
	sp.split("   This     is-a * test.    ", " *-");
	for (size_t i = 0; i < sp.size(); i++)
		std::cout << "<" << sp[i] << ">";
	std::cout << std::endl;
	sp.split("   This     is a  test.    ", ' ');
	for (Split::iterator it = sp.begin(); it != sp.end(); it++)
		std::cout << "<" << *it << ">";
	std::cout << std::endl << std::endl;

	Message m;
	if (m.getMsgType("PASS hellohello\r\n") == IRC_PASS)
		std::cout << m.getMsgStr() << " pass: " << m.getPassword() << std::endl << std::endl;
	if (m.getMsgType("NICK Wizard\r\n") == IRC_NICK)
		std::cout << m.getMsgStr() << " nick: " << m.getNickname() << std::endl << std::endl;
	if (m.getMsgType("USER guest 0 * :Ronnie Reagan\r\n") == IRC_USER)
		std::cout << m.getMsgStr() << " user: " << m.getUser() << SEP << m.getBitmode() << SEP << m.getRealname() << std::endl;
	if (m.getMsgType("USER tliangso tliangso localhost :tliangso\r\n") == IRC_USER)
		std::cout << m.getMsgStr() << " user: " << m.getUser() << SEP << m.getHostname() << SEP << m.getServername() << SEP << m.getRealname() << std::endl << std::endl;
	if (m.getMsgType("OPER foo barbar\r\n") == IRC_OPER)
		std::cout << m.getMsgStr() << " oper: " << m.getUser() << SEP << m.getPassword() << std::endl << std::endl;
	if (m.getMsgType("MODE +w\r\n") == IRC_MODE)
		std::cout << m.getMsgStr() << " mode: " << m.getMode() << std::endl << std::endl;
	if (m.getMsgType("SERVICE dict * *.fr 0 0 :French Dictionary\r\n") == IRC_SERVICE)
		std::cout << m.getMsgStr() << " service: " << m.getNickname() << SEP << m.getTarget() << SEP << m.getInfo() << std::endl << std::endl;
	if (m.getMsgType("QUIT :Gone to have lunch\r\n") == IRC_QUIT)
		std::cout << m.getMsgStr() << " quit: " << m.getInfo() << std::endl << std::endl;
	if (m.getMsgType("SQUIT tolsun.oulu.fi :Bad Link\r\n") == IRC_SQUIT)
		std::cout << m.getMsgStr() << " squit: " << m.getServername() << SEP << m.getInfo() << std::endl << std::endl;

	if (m.getMsgType("JOIN #foobar\r\n") == IRC_JOIN)
		std::cout << m.getMsgStr() << " join: " << m.getChannelList() << SEP << m.getKeyList() << std::endl;
	if (m.getMsgType("JOIN &foo,#goo,!ABCDEhoo fubar,abcde\r\n") == IRC_JOIN)
		std::cout << m.getMsgStr() << " join: " << m.getChannelList() << SEP << m.getKeyList() << std::endl;
	if (m.getMsgType("JOIN 0\r\n") == IRC_JOIN)
		std::cout << m.getMsgStr() << " join: " << m.getChannelList() << SEP << m.getKeyList() << std::endl << std::endl;

	if (m.getMsgType("PART #the_channel\r\n") == IRC_PART)
		std::cout << m.getMsgStr() << " part: " << m.getChannelList() << SEP << m.getInfo() << std::endl;
	if (m.getMsgType("PART #oz-ops,&group5 :Leaving channels\r\n") == IRC_PART)
		std::cout << m.getMsgStr() << " part: " << m.getChannelList() << SEP << m.getInfo() << std::endl << std::endl;

	if (m.getMsgType("MODE #eu-opers +l 10\r\n") == IRC_CMODE)
		std::cout << m.getMsgStr() << " cmode: " << m.getChannelModeList() << std::endl;
	if (m.getMsgType("MODE &oulu +b *!*@*.edu +e *!*@*.bu.edu\r\n") == IRC_CMODE)
		std::cout << m.getMsgStr() << " cmode: " << m.getChannel() << SEP << m.getChannelModeList() << std::endl;
	if (m.getMsgType("MODE #bu +be *!*@*.edu *!*@*.bu.edu\r\n") == IRC_CMODE)
		std::cout << m.getMsgStr() << " cmode: " << m.getChannel() << SEP << m.getChannelModeList() << std::endl;
	if (m.getMsgType("MODE #meditation e\r\n") == IRC_CMODE)
		std::cout << m.getMsgStr() << " cmode: " << m.getChannel() << SEP << m.getChannelModeList() << std::endl << std::endl;

	if (m.getMsgType("TOPIC :New topic\r\n") == IRC_TOPIC)
		std::cout << m.getMsgStr() << " topic: " << m.getTopic() << std::endl;
	if (m.getMsgType("TOPIC :\r\n") == IRC_TOPIC)
		std::cout << m.getMsgStr() << " topic: " << m.getTopic() << std::endl;
	if (m.getMsgType("TOPIC\r\n") == IRC_TOPIC)
		std::cout << m.getMsgStr() << " topic: " << m.getTopic() << std::endl << std::endl;

	if (m.getMsgType("NAMES #twilight_zone,#42\r\n") == IRC_NAMES)
		std::cout << m.getMsgStr() << " names: " << m.getChannelList() << SEP << m.getTarget() << std::endl;
	if (m.getMsgType("NAMES #twilight_zone,#42 *.example.com\r\n") == IRC_NAMES)
		std::cout << m.getMsgStr() << " names: " << m.getChannelList() << SEP << m.getTarget() << std::endl;
	if (m.getMsgType("NAMES\r\n") == IRC_NAMES)
		std::cout << m.getMsgStr() << " names: " << m.getChannelList() << SEP << m.getTarget() << std::endl << std::endl;

	if (m.getMsgType("LIST #twilight_zone,#42\r\n") == IRC_LIST)
		std::cout << m.getMsgStr() << " list: " << m.getChannelList() << SEP << m.getTarget() << std::endl;
	if (m.getMsgType("LIST #twilight_zone,#42 *.example.com\r\n") == IRC_LIST)
		std::cout << m.getMsgStr() << " list: " << m.getChannelList() << SEP << m.getTarget() << std::endl;
	if (m.getMsgType("LIST\r\n") == IRC_LIST)
		std::cout << m.getMsgStr() << " list: " << m.getChannelList() << SEP << m.getTarget() << std::endl << std::endl;

	if (m.getMsgType("INVITE [abossel] #test\r\n") == IRC_INVITE)
		std::cout << m.getMsgStr() << " invite: " << m.getNickname() << SEP << m.getChannel() << std::endl << std::endl;

	if (m.getMsgType("KICK &Melbourne Matthew\r\n") == IRC_KICK)
		std::cout << m.getMsgStr() << " kick: " << m.getChannelList() << SEP << m.getUserList() << SEP << m.getInfo() << std::endl;
	if (m.getMsgType("KICK &Melbourne,#Sydney Matthew,Tom\r\n") == IRC_KICK)
		std::cout << m.getMsgStr() << " kick: " << m.getChannelList() << SEP << m.getUserList() << SEP << m.getInfo() << std::endl;
	if (m.getMsgType("KICK &Melbourne Matthew :Test message\r\n") == IRC_KICK)
		std::cout << m.getMsgStr() << " kick: " << m.getChannelList() << SEP << m.getUserList() << SEP << m.getInfo() << std::endl << std::endl;

	return (0);
}
